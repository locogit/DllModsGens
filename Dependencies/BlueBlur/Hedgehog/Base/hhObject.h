#pragma once

#include <Hedgehog/Base/System/hhAllocator.h>

namespace Hedgehog::Base
{
    class CObject
    {
    public:
        CObject() {}
        CObject(const bb_null_ctor&) {}

        void* operator new(const size_t size)
        {
            return __HH_ALLOC(size);
        }

        void* operator new(const size_t size, const size_t align)
        {
            return __HH_ALLOCALIGN(size, align);
        }

        void operator delete(void* pMem)
        {
            return __HH_FREE(pMem);
        }
    };
}