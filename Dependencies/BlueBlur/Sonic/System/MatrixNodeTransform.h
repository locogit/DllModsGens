#pragma once

#include <Hedgehog/MirageCore/MatrixNode/hhMatrixNode.h>
#include <Hedgehog/MirageCore/Misc/hhTransform.h>

namespace Sonic
{
    class CMatrixNodeTransform : public Hedgehog::Mirage::CMatrixNode
    {
    private:
        static __declspec(noinline) void* fpCMatrixNodeTransformCtor(void* This)
        {
            void* result = nullptr;
            uint32_t func = 0x00D172B0;
            __asm
            {
                mov edi, This
                call func
                mov result, eax
            }

            return result;
        }
    public:
        Hedgehog::Mirage::CTransform m_Transform;
        Hedgehog::Math::CMatrix m_WorldMatrix;

        CMatrixNodeTransform()
        {
            fpCMatrixNodeTransformCtor(this);
        }

        void SetParent(CMatrixNode* pParentNode)
        {
            BB_FUNCTION_PTR(void, __thiscall, func, 0x6F46B0, CMatrixNode*, CMatrixNode*);
            func(pParentNode, this);
        }
        void SetChild(CMatrixNode* pChildNode)
        {
            BB_FUNCTION_PTR(void, __thiscall, func, 0x6F46B0, CMatrixNode*, CMatrixNode*);
            func(this, pChildNode);
        }

        void UpdateMatrix(CMatrixNode* pParentMatrixNode) override
        {
            BB_FUNCTION_PTR(void, __thiscall, func, 0xD17170, CMatrixNodeTransform*, CMatrixNode*);
            func(this, pParentMatrixNode);
        }

        Hedgehog::Math::CMatrix& GetLocalMatrix() override
        {
            return m_Transform.m_Matrix;
        }
        Hedgehog::Math::CMatrix& GetWorldMatrix() override
        {
            return m_WorldMatrix;
        }
    };

    BB_ASSERT_OFFSETOF(CMatrixNodeTransform, m_Transform, 0x60);
    BB_ASSERT_OFFSETOF(CMatrixNodeTransform, m_WorldMatrix, 0xC0);
    BB_ASSERT_SIZEOF(CMatrixNodeTransform, 0x100);
}