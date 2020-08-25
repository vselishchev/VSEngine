#pragma once

namespace VSEngine {
namespace System {

inline size_t GetAlignedSize(size_t size)
{
    constexpr size_t sizeTSize = sizeof(size_t) - 1;
    return (size + sizeTSize) & ~(sizeTSize);
}

struct Block
{
    struct Header
    {
        char*        pNext = nullptr;
        char*        pPrev = nullptr;
        unsigned int size = 0;
        bool         isUsed = false;
    };
    // 24 bytes

    Header header;
    size_t pointer[1]; // Payload pointer.
};
// 32 Bytes

constexpr size_t DefaultArenaSize = 4194304;

// Arena takes 4 MB by default.
template<size_t ARENA_SIZE = DefaultArenaSize>
class Arena
{
public:
    Arena()
        : m_pHead(reinterpret_cast<char*>(malloc(ARENA_SIZE)))
        , m_pCurrentTop(m_pHead)
        , m_pEnd(m_pHead + ARENA_SIZE)
    {
        ((Block*)(m_pCurrentTop))->header.size = 0;
    }

    ~Arena()
    {
        free(m_pHead);
    }

    void* Alloc(size_t size)
    {
        void* ptr = Alloc_Internal(size);
        if (ptr)
            ++m_blocksAllocated;

        return ptr;
    }

    void Free(void* ptr)
    {
        --m_blocksAllocated;
        Block* pBlock = GetBlockFromPointer(ptr);
        pBlock->header.isUsed = false;
        char* pCharBlock = (char*)pBlock;

        // If free list is empty, set it's pointer here.
        if (m_pFreeList == nullptr)
        {
            m_pFreeList = pCharBlock;
            pBlock->header.pNext = nullptr;
            pBlock->header.pPrev = nullptr;
            return;
        }

        // If block address is prior to free list first element, add block as free list head.
        if (m_pFreeList > pCharBlock)
        {
            pBlock->header.pNext = m_pFreeList;
            pBlock->header.pPrev = nullptr;
            m_pFreeList = pCharBlock;
            return;
        }

        // Otherwise find the place where to fit it.
        char* pCurrentBlock = m_pFreeList;
        char* pPreviousBlock = nullptr;
        while (pCurrentBlock && pCharBlock > pCurrentBlock)
        {
            pPreviousBlock = pCurrentBlock;
            pCurrentBlock = ((Block*)pCurrentBlock)->header.pNext;
        }

        ((Block*)pPreviousBlock)->header.pNext = pCharBlock;
        pBlock->header.pNext = pCurrentBlock;
        pBlock->header.pPrev = pPreviousBlock;
    }

    void Recycle()
    {
        // Merge adjacent blocks and reset max block size.
        m_maxBlockSize = 0;
        if (m_pFreeList == nullptr)
            return;

        Block* pPrev = (Block*)m_pFreeList;
        while (pPrev->header.pNext)
        {
            // Continue if blocks not adjacent.
            if (pPrev->header.pNext != ((char*)pPrev + pPrev->header.size()))
                continue;

            Block* pNext = (Block*)(pPrev->header.pNext);
            const size_t newSize = pPrev->header.size + pNext->header.size;
            pPrev->header.size += newSize;
            pPrev->header.pNext = pNext->header.pNext;
            ((Block*)pNext->header.pNext)->header.pPrev = (char*)pPrev;

            if (newSize > m_maxBlockSize)
                m_maxBlockSize = newSize;

            pPrev = (Block*)pPrev->header.pNext;
        }
    }

    inline size_t GetArenaSize() const
    {
        return ARENA_SIZE;
    }

    inline bool Owns(void* ptr)
    {
        return ptr >= m_pHead && ptr < m_pEnd;
    }

    // After recycling need to check if we still need this arena.
    inline bool IsEmpty() const
    {
        return m_blocksAllocated == 0;
    }

    inline bool IsFull() const // Quick check if the arena is full.
    {
        constexpr size_t blockSize = sizeof(Block);
        return m_pFreeList == nullptr && (m_pEnd - m_pCurrentTop < blockSize);
    }

private:
    void* Alloc_Internal(size_t size)
    {
        const size_t alignedSize = GetAlignedSize(GetTotalSize(size));
        if (alignedSize > m_maxBlockSize)
        {
            return nullptr;
        }

        // Fast 1: Try to give memory of the first free block.
        bool wasSplit = false;
        void* pPointer = ReuseBlock((Block*)m_pFreeList, alignedSize, wasSplit);
        if (pPointer)
        {
            if (wasSplit)
            {
                m_pFreeList += alignedSize;
            }
            else
            {
                m_pFreeList = ((Block*)m_pFreeList)->header.pNext;
            }

            return pPointer;
        }

        // Fast 2: allocate after the current top if there is still enough space.
        const size_t lastBlockSize = reinterpret_cast<Block*>(m_pCurrentTop)->header.size;

        const size_t endSize = (m_pEnd - m_pCurrentTop) - lastBlockSize;
        if (endSize > alignedSize)
        {
            m_pCurrentTop += lastBlockSize;

            Block* pNewBlock = reinterpret_cast<Block*>(m_pCurrentTop);
            pNewBlock->header.size = alignedSize;
            pNewBlock->header.isUsed = true;

            return pNewBlock->pointer;
        }

        // If (m_pEnd - m_pCurrentTop < alignedSize) find the block with appropriate size in the free list.
        Block* pCurrentBlock = reinterpret_cast<Block*>(m_pFreeList);
        while (pCurrentBlock->header.isUsed && pCurrentBlock->header.size < alignedSize)
        {
            pCurrentBlock = (Block*)pCurrentBlock->header.pNext;
            if (pCurrentBlock == nullptr)
                return nullptr;
        }

        wasSplit = false;
        return ReuseBlock(pCurrentBlock, alignedSize, wasSplit);
    }

    // Accepts value from actual payload pointer.
    inline Block* GetBlockFromPointer(void* ptr)
    {
        constexpr size_t headerSize = sizeof(Block::Header);
        return (Block*)((char*)ptr - headerSize);
    }

    // Size of memory requested considering the size of Block.
    inline size_t GetTotalSize(size_t size)
    {
        // Block already has sizeof(size_t) bytes which can be given to the user.
        constexpr size_t blockSize = sizeof(Block) - sizeof(size_t);
        return size + blockSize;
    }

    // Reuse block: if block size is higher than (newSize + Block size) then split blocks,
    // otherwise just reset "used" flag.
    void* ReuseBlock(Block* pBlock, size_t newSize, bool& wasSplit)
    {
        wasSplit = false;
        if (pBlock == nullptr || pBlock->header.size < newSize)
            return nullptr;

        // If current block size smaller than (alignedSize + blockSize), just return current block pointer.
        constexpr size_t blockSize = sizeof(Block);
        const size_t reminderSize = pBlock->header.size - newSize;
        if (reminderSize < blockSize)
        {
            pBlock->header.isUsed = true;
            return pBlock->pointer;
        }

        // Split blocks otherwise.
        char* pNewCharBlock = (char*)pBlock + newSize;
        Block* pNewBlock = (Block*)(pNewCharBlock);
        pNewBlock->header.size = reminderSize;
        pNewBlock->header.pNext = pBlock->header.pNext;

        //pBlock->header.pNext = nullptr;
        pBlock->header.size = newSize;

        Block* pPrevBlock = (Block*)pBlock->header.pPrev;
        if (pPrevBlock)
        {
            pPrevBlock->header.pNext = pNewCharBlock;
            pNewBlock->header.pPrev = (char*)pPrevBlock;
        }

        wasSplit = true;

        return pBlock->pointer;
    }

private:
    char*             m_pHead;
    char*             m_pCurrentTop;
    char*             m_pFreeList = nullptr;
    const char* const m_pEnd;
    unsigned int      m_maxBlockSize = ARENA_SIZE;
    int               m_blocksAllocated = 0;
};

} // ~System
} // ~VSEngine