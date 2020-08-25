#pragma once 

#include "Arena.h"

namespace VSEngine {
namespace System {

constexpr size_t ArenaCount = 4;

template<size_t ARENA_SIZE = DefaultArenaSize, size_t ARENA_COUNT = ArenaCount>
class BucketAllocator
{
    struct ArenaHolder
    {
        Arena<ARENA_SIZE> arena;
        ArenaHolder* pNextHolder = nullptr;
    };

public:
    static BucketAllocator& GetAllocator()
    {
        static BucketAllocator allocator;
        return allocator;
    }

    void* Alloc(size_t size)
    {
        // Fast 1: Try allocate in active arena.
        void* ptr = m_pActiveArenaHolder->arena.Alloc(size);
        if (ptr)
            return ptr;

        // Look over existing arenas if possible to allocate.
        ArenaHolder* pHolder = m_pArenaHolders;
        while (pHolder)
        {
            ptr = pHolder->arena.Alloc(size);
            if (ptr == nullptr)
            {
                pHolder = pHolder->pNextHolder;
                continue;
            }

            m_pActiveArenaHolder = pHolder;
            return ptr;
        }

        if (size > DefaultArenaSize)
        {
            // Requested memory is too big. Just request it from OS.
            return malloc(size);
        }

        // Create new arena.
        pHolder = CreateArena();
        return pHolder->arena.Alloc(size);
    }

    void Free(void* ptr)
    {
        // Fast 1: Check if active arena owns memory.
        if (m_pActiveArenaHolder->arena.Owns(ptr))
        {
            m_pActiveArenaHolder->arena.Free(ptr);
            return;
        }

        ArenaHolder* pHolder = m_pArenaHolders;
        while (pHolder)
        {
            if (pHolder->arena.Owns(ptr) == false)
            {
                pHolder = pHolder->pNextHolder;
                continue;
            }

            pHolder->arena.Free(ptr);
            return;
        }

        // Someone else owns this memory. Free it
        free(ptr);
    }

    void Recycle()
    {
        
    }

private:
    BucketAllocator()
    {
        // Allocate ARENA_COUNT arena holders as initial set of arenas.
        constexpr size_t holderSize = sizeof(ArenaHolder);
        m_pArenaHolders = (ArenaHolder*)malloc(holderSize * ARENA_COUNT);
        new(&m_pArenaHolders[0]) ArenaHolder();

        for (size_t i = 1; i < ARENA_COUNT; ++i)
        {
            new(&m_pArenaHolders[i]) ArenaHolder();

            m_pArenaHolders[i - 1].pNextHolder = &m_pArenaHolders[i];
        }
        m_pActiveArenaHolder = m_pArenaHolders;
    }

    ArenaHolder* CreateArena()
    {
        constexpr size_t holderSize = sizeof(ArenaHolder);
        ArenaHolder* pNewHolder = (ArenaHolder*)malloc(holderSize);
        new(pNewHolder) ArenaHolder();

        pNewHolder->pNextHolder = m_pArenaHolders;
        m_pArenaHolders = pNewHolder;
        m_pActiveArenaHolder = pNewHolder;

        ++m_arenaCount;

        return pNewHolder;
    }

private:
    ArenaHolder* m_pArenaHolders;
    ArenaHolder* m_pActiveArenaHolder;
    size_t       m_arenaCount = ARENA_COUNT;
};

using Allocator = BucketAllocator<DefaultArenaSize, ArenaCount>;

inline Allocator& GetAllocator()
{
    return BucketAllocator<DefaultArenaSize, ArenaCount>::GetAllocator();
}

} // ~System
} // ~VSEngine