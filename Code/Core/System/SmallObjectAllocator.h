#pragma once

#include <vector>
#include <limits>

template<typename TYPE>
class Mallocator
{
public:
    using value_type = TYPE;

    Mallocator() = default;
    template<typename U>
    constexpr Mallocator(const Mallocator<U>&) noexcept {}

    TYPE* allocate(size_t n) noexcept
    {
        constexpr size_t typeSize = sizeof(TYPE);
        constexpr size_t maxNumber = std::numeric_limits<size_t>::max() / typeSize;
        if (n > maxNumber)
            return nullptr;

        return static_cast<TYPE*>(malloc(n * typeSize));
    }

    void deallocate(TYPE* ptr, size_t n) noexcept
    {
        free(ptr);
    }
};

namespace VSEngine {
namespace System {

struct Chunk
{
    void Initialize(unsigned char blockSize, unsigned char blockCount);
    void Release();

    void* Allocate(unsigned char blockSize);
    void  Deallocate(void* ptr, unsigned char blockSize);

    unsigned char* pData;
    unsigned char  firstAvailableBlock;
    unsigned char  availableBlocks;
};

constexpr unsigned char DEFAULT_BLOCK_COUNT = 255;

class FixedSizeAllocator
{
public:
    FixedSizeAllocator(size_t blockSize = 1, unsigned char blockCount = DEFAULT_BLOCK_COUNT);
    FixedSizeAllocator(const FixedSizeAllocator& other) = delete;
    FixedSizeAllocator(FixedSizeAllocator&& other) noexcept;
    ~FixedSizeAllocator();

    FixedSizeAllocator& operator=(const FixedSizeAllocator& other) = delete;
    FixedSizeAllocator& operator=(FixedSizeAllocator&& other) noexcept;

    void*  Allocate();
    void   Deallocate(void* ptr);

    inline size_t GetBlockSize() const { return m_blockSize; }

private:
    Chunk* GetDeallocationChunk(void* ptr);
    // Only adjusts inner members, but not actually releases the memory.
    void   DoDeallocate(void* ptr);

    bool   Belongs(Chunk& chunk, void* ptr) const;

private:
    std::vector<Chunk, Mallocator<Chunk>>  m_chunks;
    Chunk*              m_pAllocChunk = nullptr;
    Chunk*              m_pDeallocChunk = nullptr;
    size_t              m_blockSize;
    unsigned char       m_blockCount;
};

constexpr size_t DEFAULT_MAX_BLOCK_SIZE = 64;

class SmallObjectAllocator
{
public:
    SmallObjectAllocator(size_t maxObjectSize = DEFAULT_MAX_BLOCK_SIZE);
    SmallObjectAllocator(const SmallObjectAllocator& other) = delete;
    SmallObjectAllocator(SmallObjectAllocator&& other) = delete;

    SmallObjectAllocator& operator=(const SmallObjectAllocator& other) = delete;
    SmallObjectAllocator& operator=(SmallObjectAllocator&& other) = delete;

    // It's supposed size is lower than m_maxObjectSize;
    void* Allocate(size_t size);
    void  Deallocate(void* ptr, size_t size);

private:
    std::vector<FixedSizeAllocator, Mallocator<FixedSizeAllocator>> m_allocators;
    FixedSizeAllocator*             m_pLastAllocator;
    FixedSizeAllocator*             m_pLastDeallocator;
    
    const size_t                    m_maxObjectSize;
};

SmallObjectAllocator& GetSmallObjectAllocator();

class SmallObject
{
public:
    SmallObject() = default;
    virtual ~SmallObject() {}

    static void* operator new(std::size_t size)
    {
        return GetSmallObjectAllocator().Allocate(size);
    }

    static void operator delete(void* p, std::size_t size)
    {
        GetSmallObjectAllocator().Deallocate(p, size);
    }
};

} // ~System
} // ~VSEngine