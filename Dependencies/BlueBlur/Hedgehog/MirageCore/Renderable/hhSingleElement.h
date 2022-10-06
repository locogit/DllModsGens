#pragma once

#include <Hedgehog/Base/Type/hhSharedString.h>
#include <Hedgehog/MirageCore/Renderable/hhElement.h>

namespace Hedgehog::Mirage
{
    class CInstanceInfo;
    class CSingleElement;
    class CModelData;
    class CMatrixNodeSingleElementNode;

    static inline BB_FUNCTION_PTR(CSingleElement*, __thiscall, fpCSingleElementCtor, 0x701D00,
        CSingleElement* This, const boost::shared_ptr<CModelData>& spModelData);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCSingleElementGetNode, 0x700B70,
        const CSingleElement* This, boost::shared_ptr<CMatrixNodeSingleElementNode>& out_spNode, const Hedgehog::Base::CSharedString& in_rName);

    class CSingleElement : public CElement
    {
    public:
        boost::shared_ptr<CInstanceInfo> m_spInstanceInfo;
        BB_INSERT_PADDING(0x14);

        CSingleElement(const bb_null_ctor&) : CElement(bb_null_ctor{}) {}

        CSingleElement(const boost::shared_ptr<CModelData>& spModelData) : CSingleElement(bb_null_ctor{})
        {
            fpCSingleElementCtor(this, spModelData);
        }

        boost::shared_ptr<CMatrixNodeSingleElementNode> GetNode(const Hedgehog::Base::CSharedString& in_rName) const
        {
            boost::shared_ptr<CMatrixNodeSingleElementNode> spNode;
            fpCSingleElementGetNode(this, spNode, in_rName);
            return spNode;
        }
    };

    BB_ASSERT_OFFSETOF(CSingleElement, m_spInstanceInfo, 0x8C);
    BB_ASSERT_SIZEOF(CSingleElement, 0xA8);
}