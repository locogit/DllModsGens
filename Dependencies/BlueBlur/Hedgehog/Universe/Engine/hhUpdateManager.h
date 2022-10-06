#pragma once

#include <BlueBlur.inl>

namespace Hedgehog::Base
{
    class CSharedString;
}

namespace Hedgehog::Universe
{
    class CUpdateUnit;
    class CUpdateManager;

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCUpdateManagerAdd, 0x77AF20,
        CUpdateManager* This, const Hedgehog::Base::CSharedString& category, CUpdateUnit* pUpdateUnit);

    class CUpdateManager
    {
    public:
        void Add(const Hedgehog::Base::CSharedString& category, CUpdateUnit* pUpdateUnit)
        {
            fpCUpdateManagerAdd(this, category, pUpdateUnit);
        }
    };
}