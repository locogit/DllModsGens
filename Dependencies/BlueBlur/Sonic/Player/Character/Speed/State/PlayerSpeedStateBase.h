#pragma once

#include <Hedgehog/Universe/Engine/hhStateMachine.h>
#include <Sonic/Player/Character/Speed/PlayerSpeedContext.h>

namespace Sonic::Player
{
    class CPlayerSpeedContext::CStateSpeedBase : public Hedgehog::Universe::TStateMachine<CPlayerContext>::TState
    {
    public:
        BB_INSERT_PADDING(0x8);

        static inline uint32_t pCtor = 0xE497E0;

        static void fCtor(CStateSpeedBase* This)
        {
            __asm
            {
                mov edi, This
                call [pCtor]
            }
        }

        CStateSpeedBase(const bb_null_ctor&) : TState(bb_null_ctor{}) {}

        CStateSpeedBase() : CStateSpeedBase(bb_null_ctor{})
        {
            fCtor(this);
        }

        void SetCondition(const Hedgehog::Base::CSharedString& name, const bool& condition)
        {
            uint32_t func = 0x0050DD40;

            __asm
            {
                push condition
                mov eax, this
                mov edi, name
                call func
            }
        }

        CPlayerSpeedContext* GetContext() const
        {
            return static_cast<CPlayerSpeedContext*>(m_pContext);
        }

        virtual void ProcMsgOnGround() {}
    };

    BB_ASSERT_SIZEOF(CPlayerSpeedContext::CStateSpeedBase, 0x68);
}