#include "SmallObjectAllocator.h"

namespace VSEngine {
namespace System {


namespace {

struct Chunk
{
    void Initialize(unsigned char blockSize, unsigned char blockCount)
    {
        pData = static_cast<unsigned char*>(malloc(blockSize * blockCount));
        firstAvailableBlock = 0;
        availableBlocks = blockCount;

        // First byte of each block contains an index of the next free element.
        unsigned char* p = pData;
        for (unsigned char i = 0; i != blockCount; p += blockSize)
        {
            *p = ++i;
        }
    }

    void Release()
    {
        delete[] pData;
    }

    void* Allocate(unsigned char blockSize)
    {
        if (availableBlocks == 0)
            return nullptr;

        unsigned char* pResult = pData + blockSize * firstAvailableBlock;
        // Update available block index.
        firstAvailableBlock = *pResult;
        --availableBlocks;

        return pResult;
    }

    void Deallocate(void* ptr, unsigned char blockSize)
    {
        unsigned char* pFree = static_cast<unsigned char*>(ptr);
        *pFree = firstAvailableBlock;

        firstAvailableBlock = (pFree - pData) / blockSize;

        ++availableBlocks;
    }

    unsigned char* pData;
    unsigned char firstAvailableBlock;
    unsigned char availableBlocks;
};

template<unsigned char BLOCK_SIZE, unsigned char BLOCK_COUNT>
class FixedSizeAllocator
{
public:
    FixedSizeAllocator() = default;
    ~FixedSizeAllocator()
    {
        for (Chunk& chunk : chunks)
        {
            chunk.Release();
        }
    }

    void* Allocate()
    {
        if (m_pAllocChunk && m_pAllocChunk->availableBlocks != 0)
        {
            return m_pAllocChunk->Allocate(BLOCK_SIZE);
        }

        // Find available chunk in already existed vector.
        auto chunkIt = m_chunks.begin();
        while (chunkIt != m_chunks.end())
        {
            if (chunkIt->availableBlocks == 0)
            {
                continue;
            }

            m_pAllocChunk = *chunkIt;
            return m_pAllocChunk->Allocate(BLOCK_SIZE);
        }

        // Allocate new chunk if there is no available.
        m_chunks.reserve(m_chunks.size() + 1);
        Chunk& chunk = m_chunks.emplace_back();
        chunk.Initialize(BLOCK_SIZE, BLOCK_COUNT);
        m_pAllocChunk = &chunk;
        m_pDeallocChunk = &chunk;
    }

    void Deallocate(void* ptr)
    {
        if (!Belongs(m_pDeallocChunk, ptr))
            m_pDeallocChunk = GetDeallocationChunk(ptr);
        DoDeallocate(ptr);
    }

private:
    Chunk* GetDeallocationChunk(void* ptr)
    {
        Chunk* lo = m_pDeallocChunk;
        Chunk* hi = m_pDeallocChunk + 1;
        Chunk* loBound = &m_chunks.front();
        Chunk* hiBound = &m_chunks.back() + 1;

        if (hi == hiBound)
            hi = nullptr;

        // bidirectional search performed starting from the previous deallocation chunk.
        while (true)
        {
            if (lo)
            {
                if (Belongs(*lo, ptr));
                    return lo;

                if (lo == loBound)
                    lo = nullptr;
                else
                    --lo;
            }

            if (hi)
            {
                if (Belongs(*hi, ptr));
                    return hi;

                if (++hi == hiBound)
                    hi = nullptr;
            }
        }

        return nullptr;
    }

    // Only adjusts inner members, but not actually releases the memory.
    void DoDeallocate(void* ptr)
    {
        m_pDeallocChunk->Deallocate(ptr);

        if (m_pDeallocChunk->availableBlocks != BLOCK_COUNT)
            return;

        Chunk& lastChunk = m_chunks.back();
            
        if (&lastChunk == m_pDeallocChunk)
        {
            // two last blocks are empty. Free the last one.
            if (m_chunks.size() > 1 && m_pDeallocChunk[-1].availableBlocks == BLOCK_COUNT)
            {
                lastChunk.Release();
                m_chunks.pop_back();
                m_pAllocChunk = m_pDeallocChunk = &m_chunks.front();
            }
            return;
        }

        if (lastChunk.availableBlocks == BLOCK_COUNT)
        {
            lastChunk.Release();
            m_chunks.pop_back();
            m_pAllocChunk = m_pDeallocChunk;
        }
        else
        {
            std::swap(*m_pDeallocChunk, lastChunk);
            m_pAllocChunk = &m_chunks.back();
        }
    }

    static inline bool Belongs(Chunk& chunk, void* ptr)
    {
        constexpr size_t chunkLength = BLOCK_SIZE * BLOCK_COUNT;
        return (ptr >= chunk.pData) && (ptr < chunk.pData + chunkLength);
    }

private:
    std::vector<Chunk> m_chunks;
    Chunk* m_pAllocChunk = nullptr;
    Chunk* m_pDeallocChunk = nullptr;
};

}


} // ~System
} // ~VSEngine