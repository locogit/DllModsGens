﻿#pragma once

#include <Sonic/Tool/EditParam/ParamBase.h>

namespace Sonic
{
    template<typename T>
    class CParamValue : public CParamBase
    {
    public:
        static void* const ms_pVfTable;

        class FuncData : public Hedgehog::Base::CRefCountObject
        {
        public:
            T* m_pValue;
            T m_Value;
            BB_INSERT_PADDING(0x8);
            void (*m_pUpdater)();
            BB_INSERT_PADDING(0x14);
            Hedgehog::Base::CSharedString m_Description;
            BB_INSERT_PADDING(0x4);
        };

        FuncData* m_pFuncData;
        T m_DefaultValue;

        BB_ASSERT_OFFSETOF(FuncData, m_pValue, 0x8);
        BB_ASSERT_OFFSETOF(FuncData, m_Value, 0xC);
        BB_ASSERT_OFFSETOF(FuncData, m_pUpdater, 0x18);
        BB_ASSERT_OFFSETOF(FuncData, m_Description, 0x30);
        BB_ASSERT_SIZEOF(FuncData, 0x38);

        BB_ASSERT_OFFSETOF(CParamValue, m_pFuncData, 0x14);
        BB_ASSERT_OFFSETOF(CParamValue, m_DefaultValue, 0x18);
        BB_ASSERT_SIZEOF(CParamValue, 0x1C);
    };

#if 0
    void* const CParamValue<float>::ms_pVfTable = (void*)0x16E5CDC;
    void* const CParamValue<int>::ms_pVfTable = (void*)0x16E5E14;
    void* const CParamValue<long>::ms_pVfTable = (void*)0x16E5DAC;
    void* const CParamValue<unsigned long>::ms_pVfTable = (void*)0x16E5D44;
#endif
}
