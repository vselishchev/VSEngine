#include "SmallObjectAllocator.h"

namespace VSEngine {
namespace System {

SmallObjectAllocator& GetSmallObjectAllocator()
{
    // Temporary hack. Make it smarter.
    static SmallObjectAllocator* smallObjectAllocator = nullptr;
    if (smallObjectAllocator == nullptr)
    {
        smallObjectAllocator = static_cast<SmallObjectAllocator*>(malloc(sizeof(SmallObjectAllocator)));
        new(smallObjectAllocator) SmallObjectAllocator();
    }

    return *smallObjectAllocator;
}

void Chunk::Initialize(unsigned char blockSize, unsigned char blockCount)
{
    pData = static_cast<unsigned char*>(malloc(size_t(blockSize) * blockCount));
    firstAvailableBlock = 0;
    availableBlocks = blockCount;

    // First byte of each block contains an index of the next free element.
    unsigned char* p = pData;
    for (unsigned char i = 0; i != blockCount; p += blockSize)
    {
        *p = ++i;
    }
}

void Chunk::Release()
{
    delete[] pData;
}

void* Chunk::Allocate(unsigned char blockSize)
{
    if (availableBlocks == 0)
        return nullptr;

    unsigned char* pResult = pData + size_t(blockSize) * firstAvailableBlock;
    // Update available block index.
    firstAvailableBlock = *pResult;
    --availableBlocks;

    return pResult;
}

void Chunk::Deallocate(void* ptr, unsigned char blockSize)
{
    unsigned char* pFree = static_cast<unsigned char*>(ptr);
    *pFree = firstAvailableBlock;

    firstAvailableBlock = (pFree - pData) / blockSize;

    ++availableBlocks;
}

FixedSizeAllocator::FixedSizeAllocator(size_t blockSize, unsigned char blockCount)
    : m_blockSize(blockSize)
    , m_blockCount(blockCount)
{
}

FixedSizeAllocator::~FixedSizeAllocator()
{
    for (Chunk& chunk : m_chunks)
    {
        chunk.Release();
    }
}

FixedSizeAllocator::FixedSizeAllocator(FixedSizeAllocator&& other) noexcept
    : m_chunks(std::move(other.m_chunks))
    , m_pAllocChunk(other.m_pAllocChunk)
    , m_pDeallocChunk(other.m_pDeallocChunk)
    , m_blockSize(other.m_blockSize)
    , m_blockCount(other.m_blockCount)
{
    other.m_pAllocChunk = nullptr;
    other.m_pDeallocChunk = nullptr;
}

FixedSizeAllocator& FixedSizeAllocator::operator=(FixedSizeAllocator&& other) noexcept
{
    if (this != &other)
    {
        m_chunks = std::move(other.m_chunks);
        m_pAllocChunk = other.m_pAllocChunk;
        m_pDeallocChunk = other.m_pDeallocChunk;
        m_blockSize = other.m_blockSize;
        m_blockCount = other.m_blockCount;

        other.m_pAllocChunk = nullptr;
        other.m_pDeallocChunk = nullptr;
    }

    return *this;
}

void* FixedSizeAllocator::Allocate()
{
    if (m_pAllocChunk && m_pAllocChunk->availableBlocks != 0)
    {
        return m_pAllocChunk->Allocate(m_blockSize);
    }

    // Find available chunk in already existed vector.
    auto chunkIt = m_chunks.begin();
    while (chunkIt != m_chunks.end())
    {
        if (chunkIt->availableBlocks == 0)
        {
            ++chunkIt;
            continue;
        }

        m_pAllocChunk = &(*chunkIt);
        return m_pAllocChunk->Allocate(m_blockSize);
    }

    // Allocate new chunk if there is no available.
    m_chunks.reserve(m_chunks.size() + 1);
    Chunk& chunk = m_chunks.emplace_back();
    chunk.Initialize(m_blockSize, m_blockCount);
    m_pAllocChunk = &chunk;
    m_pDeallocChunk = &chunk;

    return chunk.Allocate(m_blockSize);
}

void FixedSizeAllocator::Deallocate(void* ptr)
{
    if (!Belongs(*m_pDeallocChunk, ptr))
        m_pDeallocChunk = GetDeallocationChunk(ptr);
    DoDeallocate(ptr);
}

Chunk* FixedSizeAllocator::GetDeallocationChunk(void* ptr)
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
            if (Belongs(*lo, ptr))
                return lo;

            if (lo == loBound)
                lo = nullptr;
            else
                --lo;
        }

        if (hi)
        {
            if (Belongs(*hi, ptr))
                return hi;

            if (++hi == hiBound)
                hi = nullptr;
        }
    }

    return nullptr;
}

void FixedSizeAllocator::DoDeallocate(void* ptr)
{
    m_pDeallocChunk->Deallocate(ptr, m_blockSize);

    if (m_pDeallocChunk->availableBlocks != m_blockCount)
        return;

    Chunk& lastChunk = m_chunks.back();

    // Remove chunk only if there already 2 chunks free.
    if (&lastChunk == m_pDeallocChunk)
    {
        // two last blocks are empty. Free the last one.
        if (m_chunks.size() > 1 && m_pDeallocChunk[-1].availableBlocks == m_blockCount)
        {
            lastChunk.Release();
            m_chunks.pop_back();
            m_pAllocChunk = m_pDeallocChunk = &m_chunks.front();
        }
        return;
    }

    if (lastChunk.availableBlocks == m_blockCount)
    {
        lastChunk.Release();
        m_chunks.pop_back();
    }

    if (m_chunks.size() > 1)
        std::swap(*m_pDeallocChunk, m_chunks.back());
    m_pAllocChunk = m_pDeallocChunk = &m_chunks.back();
}

inline bool FixedSizeAllocator::Belongs(Chunk& chunk, void* ptr) const
{
    const size_t chunkLength = m_blockSize * m_blockCount;
    return (ptr >= chunk.pData) && (ptr < chunk.pData + chunkLength);
}

SmallObjectAllocator::SmallObjectAllocator(size_t maxObjectSize)
    : m_pLastAllocator(nullptr)
    , m_pLastDeallocator(nullptr)
    , m_maxObjectSize(maxObjectSize)
{
}

bool AllocatorComparator(const FixedSizeAllocator& allocator, size_t size)
{
    return allocator.GetBlockSize() < size;
}

void* SmallObjectAllocator::Allocate(size_t size)
{
    if (size > m_maxObjectSize)
        return malloc(size);

    if (m_pLastAllocator && m_pLastAllocator->GetBlockSize() == size)
        return m_pLastAllocator->Allocate();

    auto allocIter = std::lower_bound(m_allocators.begin(), m_allocators.end(), size, AllocatorComparator);

    if (allocIter == m_allocators.end() || allocIter->GetBlockSize() != size)
    {
        allocIter = m_allocators.insert(allocIter, FixedSizeAllocator(size));
        m_pLastDeallocator = &*m_allocators.begin();
    }

    m_pLastAllocator = &*allocIter;

    return m_pLastAllocator->Allocate();
}

void SmallObjectAllocator::Deallocate(void* ptr, size_t size)
{
    if (size > m_maxObjectSize)
    {
        delete ptr;
        return;
    }

    if (m_pLastDeallocator && m_pLastDeallocator->GetBlockSize() == size)
    {
        m_pLastDeallocator->Deallocate(ptr);
        return;
    }

    auto deallocIter = std::find_if(m_allocators.begin(), m_allocators.end(), [&](const FixedSizeAllocator& allocator) -> bool
    {
        return allocator.GetBlockSize() == size;
    });

    if (deallocIter == m_allocators.end())
    {
        // Out of sync. There is no small object allocator corresponding to the size "size".
        return;
    }

    m_pLastDeallocator = &*deallocIter;
    m_pLastDeallocator->Deallocate(ptr);
}

} // ~System
} // ~VSEngine