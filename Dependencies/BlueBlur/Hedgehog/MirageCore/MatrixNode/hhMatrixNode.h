#pragma once

#include <Hedgehog/Base/hhObject.h>

namespace Hedgehog::Mirage
{
    class CMatrixNode;

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCMatrixNodeNotifyChanged, 0x6F3CB0, CMatrixNode* This);

    class CMatrixNode : public Base::CObject
    {
    public:
        BB_INSERT_PADDING(0x5C);

        virtual ~CMatrixNode() = default;
        virtual void UpdateMatrix(CMatrixNode* pParentMatrixNode) = 0;
        virtual Hedgehog::Math::CMatrix& GetLocalMatrix() = 0;
        virtual Hedgehog::Math::CMatrix& GetWorldMatrix() = 0;

        CMatrixNode(const bb_null_ctor&) : CObject(bb_null_ctor{}) {}

        CMatrixNode()
        {
            BB_FUNCTION_PTR(void, __thiscall, fpCtor, 0x006F4530, CMatrixNode*);
            fpCtor(this);
        }

        void NotifyChanged()
        {
            fpCMatrixNodeNotifyChanged(this);
        }
    };

    BB_ASSERT_SIZEOF(CMatrixNode, 0x60);
}