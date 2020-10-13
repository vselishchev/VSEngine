#include "BucketAllocator.h"
#include "SmallObjectAllocator.h"

void* operator new(size_t size)
{
    if (size > VSEngine::System::DEFAULT_MAX_BLOCK_SIZE)
    {
        return malloc(size);
    }

    return VSEngine::System::GetSmallObjectAllocator().Allocate(size);
}

void* operator new[](size_t size)
{
    if (size > VSEngine::System::DEFAULT_MAX_BLOCK_SIZE)
    {
        return malloc(size);
    }

    return VSEngine::System::GetSmallObjectAllocator().Allocate(size);
}

void operator delete(void* ptr, size_t size) noexcept
{
    if (size > VSEngine::System::DEFAULT_MAX_BLOCK_SIZE)
    {
        free(ptr);
    }

    VSEngine::System::GetSmallObjectAllocator().Deallocate(ptr, size);
}

void operator delete[](void* ptr, size_t size) noexcept
{
    if (size > VSEngine::System::DEFAULT_MAX_BLOCK_SIZE)
    {
        free(ptr);
    }

    VSEngine::System::GetSmallObjectAllocator().Deallocate(ptr, size);
}