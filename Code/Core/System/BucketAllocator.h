#pragma once

namespace VSEngine {
namespace System {

inline size_t GetAlignedSize(size_t size)
{
    
}

struct Block
{
    struct Header
    {
        size_t size = 0;
        Block* pNext = nullptr;
        bool   isUsed = false;
    };

    Header header;
    size_t pointer[1]; // Payload pointer.
};

// Arena takes 16 MB by default.
template<size_t ARENA_SIZE = 16777216>
class Arena
{
public:
    Arena() = default;
    bool IsOccupied() const // Quick check if the arena is full.
    {
        return m_isOccupied;
    }

    void*  Alloc(size_t size);
    void   Free(void* ptr);

    size_t GetArenaSize() const
    {
        return ARENA_SIZE;
    }

private:
    // Accepts value from actual payload pointer.
    Block* GetBlockFromPointer(void* ptr)
    {
        constexpr size_t headerSize = sizeof(Block::Header);
        return static_cast<Block*>(ptr - headerSize);
    }

private:
    char   m_data[ARENA_SIZE];
    char*  m_pCurrentTop = &m_data[0];
    size_t m_maximumAvailableBlock = ARENA_SIZE;
    bool   m_isOccupied = false;
};

} // ~System
} // ~VSEngine