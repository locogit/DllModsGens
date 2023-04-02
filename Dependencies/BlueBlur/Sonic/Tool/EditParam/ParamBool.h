﻿#pragma once

#include <Sonic/Tool/EditParam/ParamBase.h>

namespace Sonic
{
    class CParamBool : public CParamBase
    {
    public:
        static constexpr void* ms_pVfTable = (void*)0x016E58E4;

        class FuncData : public Hedgehog::Base::CRefCountObject
        {
            // TODO
        };

        FuncData* m_pFuncData;
        bool m_DefaultValue;
    };

    BB_ASSERT_OFFSETOF(CParamBool, m_pFuncData, 0x14);
    BB_ASSERT_OFFSETOF(CParamBool, m_DefaultValue, 0x18);
    BB_ASSERT_SIZEOF(CParamBool, 0x1C);
}
