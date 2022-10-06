﻿#pragma once

#include <Hedgehog/Base/hhRefCountObject.h>
#include <Hedgehog/Base/Container/hhVector.h>

namespace Sonic
{
    class CParamBase;

    class CEditParam : public Hedgehog::Base::CRefCountObject
    {
    public:
        hh::vector<CParamBase*> m_Parameters;
        BB_INSERT_PADDING(0x20);
    };

    BB_ASSERT_OFFSETOF(CEditParam, m_Parameters, 0x8);
    BB_ASSERT_SIZEOF(CEditParam, 0x38);
}