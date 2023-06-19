#pragma once

#include <Hedgehog/Base/hhObject.h>
#include <Hedgehog/Base/Type/hhSharedString.h>
#include <Hedgehog/Universe/Engine/hhStateMachine.h>

#undef PlaySound
enum class ePlayerParameter;
// TODO: Move somewhere else, or just map out all the states I guess????
// Enum of every state name.
enum class StateAction
{
    Stand,
    Walk,
    JumpShort,
    Jump,
    MoveStop,
    Brake,
    BrakeEnd,
    BrakeTurn,
    Fall,
    Land,
    LandJumpShort,
    DeadLanding,
    JumpSpring,
    JumpSpringHeadLand,
    HangOn,
    ReactionJump,    // THIS CRASHES THE GAME AND IS REALLY FUCKED UP
    ReactionLand,
    TrickJump,
    CrashWall,
    CrashWallDown,
    PressDead,
    PressDamage,
    WallJumpReady,
    WallJump,
    Battery,    // Game crashes lol
    Sliding,
    ShoesSliding,
    GoalAir,
    Goal,
    ExternalControl_Crash,
    TrickJumpSuccess,
    FinishExternalControlAir,
    PushingWall,
    PushObject,
    KickBox,
    PlayAnimation,
    NormalDamage,
    NormalDamageStandUp,
    NormalDamageAir,
    NormalDamageDead,
    NormalDamageDeadAir,
    DamageCancel,
    DamageOnRunning,
    DamageOnStomping,
    TrickAttackLand,
    TrickAttack,
    SpecialJump,
    DrowningDead,
    MoveToPositionAndWait,
    WallRunDamage,
    LookUp,
    Stagger,
    TakeBreath_Crash,
    Suffocate_Crash,
    HipSliding,
    AdlibTrick,
    AdlibTrickFailure,
    Pipe,
    AirBoost,
    Drift,
    NoStanding,
    HomingAttack,
    HomingAttackAfter,
    Grind,
    GrindJumpSide_Crash,
    GrindSquat_Crash,
    GrindJumpShort_Crash,
    GrindLandJumpShort_Crash,
    GrindSwitch_Crash,
    GrindDamageMiddle_Crash,
    GrindToWallWalk_Crash,
    OnSlipFloor,
    Stepping_Crash,
    TramRiding_Crash,
    BeforeBoundJump,
    BoundJump,
    StumbleAir,
    StumbleGround,
    QuickStep,
    RunQuickStep,
    StartDash,
    StartEvent,
    StartCrouching,
    BoardWalk,
    BoardGetOn,
    BoardGetOff,
    BoardNormalDamage,
    BoardJumpShort,
    BoardJump,
    BoardAdlibTrick,
    BoardQuickStep,
    BoardRunQuickStep,
    BoardFall,
    BoardGrind,
    BoardGrindJumpShort,
    BoardGrindJumpSide,
    BoardGrindLandJumpShort,
    BoardLandJumpShort,
    BoardAirBoost,
    BoardJumpSpring,
    BoardDrift,
    JumpHurdle,
    Squat,
    SlidingEnd,
    SelectJump,
    JumpSelector,
    Stomping_Crash,
    StompingLand,
    LightSpeedDash,
    SelectJumpAfter,
    DivingFloat,
    DivingDive,
    DivingDamage,
    SquatKick,
    OnIce,
    DamageFreeze,
    DamageShock,
    DamageNearFar,
    Spin,
    SpinCharge,
    SpinChargeSliding,
    SpinSliding,
    SquatCharge,
    FloatingBoost,
    TransformRocket,
    TransformSpike,
    TransformSp,
    TransformStandard,
    RocketIdle,
    RocketLaunch,
    RocketEnd,
    RocketOverHeat,
    SpikeIdle,
    SpikeWalk,
    SpikeFall,
    SpikeJump,
    SpikeLand,
    SpikeCharge,
    SpikeBoost,
    SpikeJumpSpring,
    SpikeSpecialJump,
    SpikeDamageShock,
    SpikeHomingAttack,
    SpikeHomingAttackAfter,
    NONE
};

namespace Hedgehog::Mirage
{
    class CMatrixNodeNormal;
}

namespace Hedgehog::Sound
{
    class CSoundHandle;
}

namespace Sonic
{
    class CMatrixNodeTransform;
    class CRayCastCollision;
    class CShapeCastCollision;
    class CCharacterProxy;
}

namespace Sonic::Player
{
    class CParameter;
    class CPlayer;
    class CPlayerContext;
    class CInputPad;

    static inline BB_FUNCTION_PTR(const Hedgehog::Base::CSharedString&, __stdcall,
        fpCPlayerContextGetCurrentAnimationName, 0xE72CB0, CPlayerContext* This);

    static uint32_t pCPlayerContextChangeState = 0xE4FF30;

    static Hedgehog::Universe::TStateMachine<CPlayerContext>::TState* fCPlayerContextChangeState(
        CPlayerContext* This, const Hedgehog::Base::CSharedString* pType)
    {
        Hedgehog::Universe::TStateMachine<CPlayerContext>::TState* pResult;

        __asm
        {
            mov eax, pType
            mov ecx, This
            call[pCPlayerContextChangeState]
            mov pResult, eax
        }

        return pResult;
    }

    static float __cdecl fpGetFloatParameter(const void* paramList, const int param)
    {
#ifdef _USE_STATIC
        getFloatParameter_param = param;
        getFloatParameter_paramList = (uint32_t)paramList;

        fpGetFloatParameter_asm();

        return getFloatParameter_return;
#else
        uint32_t func = 0x0053A9F0;
        volatile float result = 0.0f;
        uint32_t _paramList = (uint32_t)paramList;
        uint32_t _param = param;

        __asm
        {
            push     _param
            mov eax, _paramList
            call[func]
            movss dword ptr[result], xmm0
        }

        return result;
#endif
    }

    class CPlayerContext : public Hedgehog::Base::CObject
    {
    private:
        // TODO: PLEASE FOR THE LOVE OF GOD PUT SOMEWHERE ELSE
        inline const char* StringFromActionEnum(StateAction state)
        {
            const char* result;

            switch (state)
            {
            default:
            case StateAction::Stand:
            {
                result = "Stand";
                break;
            }
            case StateAction::Walk:
            {
                result = "Walk";
                break;
            }
            case StateAction::JumpShort:
            {
                result = "JumpShort";
                break;
            }
            case StateAction::Jump:
            {
                result = "Jump";
                break;
            }
            case StateAction::MoveStop:
            {
                result = "MoveStop";
                break;
            }
            case StateAction::Brake:
            {
                result = "Brake";
                break;
            }
            case StateAction::BrakeEnd:
            {
                result = "BrakeEnd";
                break;
            }
            case StateAction::BrakeTurn:
            {
                result = "BrakeTurn";
                break;
            }
            case StateAction::Fall:
            {
                result = "Fall";
                break;
            }
            case StateAction::Land:
            {
                result = "Land";
                break;
            }
            case StateAction::LandJumpShort:
            {
                result = "LandJumpShort";
                break;
            }
            case StateAction::DeadLanding:
            {
                result = "DeadLanding";
                break;
            }
            case StateAction::JumpSpring:
            {
                result = "JumpSpring";
                break;
            }
            case StateAction::JumpSpringHeadLand:
            {
                result = "JumpSpringHeadLand";
                break;
            }
            case StateAction::HangOn:
            {
                result = "HangOn";
                break;
            }
            case StateAction::ReactionJump:
            {
                result = "ReactionJump";
                break;
            }
            case StateAction::ReactionLand:
            {
                result = "ReactionLand";
                break;
            }
            case StateAction::TrickJump:
            {
                result = "TrickJump";
                break;
            }
            case StateAction::CrashWall:
            {
                result = "CrashWall";
                break;
            }
            case StateAction::CrashWallDown:
            {
                result = "CrashWallDown";
                break;
            }
            case StateAction::PressDead:
            {
                result = "PressDead";
                break;
            }
            case StateAction::PressDamage:
            {
                result = "PressDamage";
                break;
            }
            case StateAction::WallJumpReady:
            {
                result = "WallJumpReady";
                break;
            }
            case StateAction::WallJump:
            {
                result = "WallJump";
                break;
            }
            case StateAction::Battery:
            {
                result = "Battery";
                break;
            }
            case StateAction::Sliding:
            {
                result = "Sliding";
                break;
            }
            case StateAction::ShoesSliding:
            {
                result = "ShoesSliding";
                break;
            }
            case StateAction::GoalAir:
            {
                result = "GoalAir";
                break;
            }
            case StateAction::Goal:
            {
                result = "Goal";
                break;
            }
            case StateAction::ExternalControl_Crash:
            {
                result = "ExternalControl_Crash";
                break;
            }
            case StateAction::TrickJumpSuccess:
            {
                result = "TrickJumpSuccess";
                break;
            }
            case StateAction::FinishExternalControlAir:
            {
                result = "FinishExternalControlAir";
                break;
            }
            case StateAction::PushingWall:
            {
                result = "PushingWall";
                break;
            }
            case StateAction::PushObject:
            {
                result = "PushObject";
                break;
            }
            case StateAction::KickBox:
            {
                result = "KickBox";
                break;
            }
            case StateAction::PlayAnimation:
            {
                result = "PlayAnimation";
                break;
            }
            case StateAction::NormalDamage:
            {
                result = "NormalDamage";
                break;
            }
            case StateAction::NormalDamageStandUp:
            {
                result = "NormalDamageStandUp";
                break;
            }
            case StateAction::NormalDamageAir:
            {
                result = "NormalDamageAir";
                break;
            }
            case StateAction::NormalDamageDead:
            {
                result = "NormalDamageDead";
                break;
            }
            case StateAction::NormalDamageDeadAir:
            {
                result = "NormalDamageDeadAir";
                break;
            }
            case StateAction::DamageCancel:
            {
                result = "DamageCancel";
                break;
            }
            case StateAction::DamageOnRunning:
            {
                result = "DamageOnRunning";
                break;
            }
            case StateAction::DamageOnStomping:
            {
                result = "DamageOnStomping";
                break;
            }
            case StateAction::TrickAttackLand:
            {
                result = "TrickAttackLand";
                break;
            }
            case StateAction::TrickAttack:
            {
                result = "TrickAttack";
                break;
            }
            case StateAction::SpecialJump:
            {
                result = "SpecialJump";
                break;
            }
            case StateAction::DrowningDead:
            {
                result = "DrowningDead";
                break;
            }
            case StateAction::MoveToPositionAndWait:
            {
                result = "MoveToPositionAndWait";
                break;
            }
            case StateAction::WallRunDamage:
            {
                result = "WallRunDamage";
                break;
            }
            case StateAction::LookUp:
            {
                result = "LookUp";
                break;
            }
            case StateAction::Stagger:
            {
                result = "Stagger";
                break;
            }
            case StateAction::TakeBreath_Crash:
            {
                result = "TakeBreath_Crash";
                break;
            }
            case StateAction::Suffocate_Crash:
            {
                result = "Suffocate_Crash";
                break;
            }
            case StateAction::HipSliding:
            {
                result = "HipSliding";
                break;
            }
            case StateAction::AdlibTrick:
            {
                result = "AdlibTrick";
                break;
            }
            case StateAction::AdlibTrickFailure:
            {
                result = "AdlibTrickFailure";
                break;
            }
            case StateAction::Pipe:
            {
                result = "Pipe";
                break;
            }
            case StateAction::AirBoost:
            {
                result = "AirBoost";
                break;
            }
            case StateAction::Drift:
            {
                result = "Drift";
                break;
            }
            case StateAction::NoStanding:
            {
                result = "NoStanding";
                break;
            }
            case StateAction::HomingAttack:
            {
                result = "HomingAttack";
                break;
            }
            case StateAction::HomingAttackAfter:
            {
                result = "HomingAttackAfter";
                break;
            }
            case StateAction::Grind:
            {
                result = "Grind";
                break;
            }
            case StateAction::GrindJumpSide_Crash:
            {
                result = "GrindJumpSide_Crash";
                break;
            }
            case StateAction::GrindSquat_Crash:
            {
                result = "GrindSquat_Crash";
                break;
            }
            case StateAction::GrindJumpShort_Crash:
            {
                result = "GrindJumpShort_Crash";
                break;
            }
            case StateAction::GrindLandJumpShort_Crash:
            {
                result = "GrindLandJumpShort_Crash";
                break;
            }
            case StateAction::GrindSwitch_Crash:
            {
                result = "GrindSwitch_Crash";
                break;
            }
            case StateAction::GrindDamageMiddle_Crash:
            {
                result = "GrindDamageMiddle_Crash";
                break;
            }
            case StateAction::GrindToWallWalk_Crash:
            {
                result = "GrindToWallWalk_Crash";
                break;
            }
            case StateAction::OnSlipFloor:
            {
                result = "OnSlipFloor";
                break;
            }
            case StateAction::Stepping_Crash:
            {
                result = "Stepping_Crash";
                break;
            }
            case StateAction::TramRiding_Crash:
            {
                result = "TramRiding_Crash";
                break;
            }
            case StateAction::BeforeBoundJump:
            {
                result = "BeforeBoundJump";
                break;
            }
            case StateAction::BoundJump:
            {
                result = "BoundJump";
                break;
            }
            case StateAction::StumbleAir:
            {
                result = "StumbleAir";
                break;
            }
            case StateAction::StumbleGround:
            {
                result = "StumbleGround";
                break;
            }
            case StateAction::QuickStep:
            {
                result = "QuickStep";
                break;
            }
            case StateAction::RunQuickStep:
            {
                result = "RunQuickStep";
                break;
            }
            case StateAction::StartDash:
            {
                result = "StartDash";
                break;
            }
            case StateAction::StartEvent:
            {
                result = "StartEvent";
                break;
            }
            case StateAction::StartCrouching:
            {
                result = "StartCrouching";
                break;
            }
            case StateAction::BoardWalk:
            {
                result = "BoardWalk";
                break;
            }
            case StateAction::BoardGetOn:
            {
                result = "BoardGetOn";
                break;
            }
            case StateAction::BoardGetOff:
            {
                result = "BoardGetOff";
                break;
            }
            case StateAction::BoardNormalDamage:
            {
                result = "BoardNormalDamage";
                break;
            }
            case StateAction::BoardJumpShort:
            {
                result = "BoardJumpShort";
                break;
            }
            case StateAction::BoardJump:
            {
                result = "BoardJump";
                break;
            }
            case StateAction::BoardAdlibTrick:
            {
                result = "BoardAdlibTrick";
                break;
            }
            case StateAction::BoardQuickStep:
            {
                result = "BoardQuickStep";
                break;
            }
            case StateAction::BoardRunQuickStep:
            {
                result = "BoardRunQuickStep";
                break;
            }
            case StateAction::BoardFall:
            {
                result = "BoardFall";
                break;
            }
            case StateAction::BoardGrind:
            {
                result = "BoardGrind";
                break;
            }
            case StateAction::BoardGrindJumpShort:
            {
                result = "BoardGrindJumpShort";
                break;
            }
            case StateAction::BoardGrindJumpSide:
            {
                result = "BoardGrindJumpSide";
                break;
            }
            case StateAction::BoardGrindLandJumpShort:
            {
                result = "BoardGrindLandJumpShort";
                break;
            }
            case StateAction::BoardLandJumpShort:
            {
                result = "BoardLandJumpShort";
                break;
            }
            case StateAction::BoardAirBoost:
            {
                result = "BoardAirBoost";
                break;
            }
            case StateAction::BoardJumpSpring:
            {
                result = "BoardJumpSpring";
                break;
            }
            case StateAction::BoardDrift:
            {
                result = "BoardDrift";
                break;
            }
            case StateAction::JumpHurdle:
            {
                result = "JumpHurdle";
                break;
            }
            case StateAction::Squat:
            {
                result = "Squat";
                break;
            }
            case StateAction::SlidingEnd:
            {
                result = "SlidingEnd";
                break;
            }
            case StateAction::SelectJump:
            {
                result = "SelectJump";
                break;
            }
            case StateAction::JumpSelector:
            {
                result = "JumpSelector";
                break;
            }
            case StateAction::Stomping_Crash:
            {
                result = "Stomping_Crash";
                break;
            }
            case StateAction::StompingLand:
            {
                result = "StompingLand";
                break;
            }
            case StateAction::LightSpeedDash:
            {
                result = "LightSpeedDash";
                break;
            }
            case StateAction::SelectJumpAfter:
            {
                result = "SelectJumpAfter";
                break;
            }
            case StateAction::DivingFloat:
            {
                result = "DivingFloat";
                break;
            }
            case StateAction::DivingDive:
            {
                result = "DivingDive";
                break;
            }
            case StateAction::DivingDamage:
            {
                result = "DivingDamage";
                break;
            }
            case StateAction::SquatKick:
            {
                result = "SquatKick";
                break;
            }
            case StateAction::OnIce:
            {
                result = "OnIce";
                break;
            }
            case StateAction::DamageFreeze:
            {
                result = "DamageFreeze";
                break;
            }
            case StateAction::DamageShock:
            {
                result = "DamageShock";
                break;
            }
            case StateAction::DamageNearFar:
            {
                result = "DamageNearFar";
                break;
            }
            case StateAction::Spin:
            {
                result = "Spin";
                break;
            }
            case StateAction::SpinCharge:
            {
                result = "SpinCharge";
                break;
            }
            case StateAction::SpinChargeSliding:
            {
                result = "SpinChargeSliding";
                break;
            }
            case StateAction::SpinSliding:
            {
                result = "SpinSliding";
                break;
            }
            case StateAction::SquatCharge:
            {
                result = "SquatCharge";
                break;
            }
            case StateAction::FloatingBoost:
            {
                result = "FloatingBoost";
                break;
            }
            case StateAction::TransformRocket:
            {
                result = "TransformRocket";
                break;
            }
            case StateAction::TransformSpike:
            {
                result = "TransformSpike";
                break;
            }
            case StateAction::TransformSp:
            {
                result = "TransformSp";
                break;
            }
            case StateAction::TransformStandard:
            {
                result = "TransformStandard";
                break;
            }
            case StateAction::RocketIdle:
            {
                result = "RocketIdle";
                break;
            }
            case StateAction::RocketLaunch:
            {
                result = "RocketLaunch";
                break;
            }
            case StateAction::RocketEnd:
            {
                result = "RocketEnd";
                break;
            }
            case StateAction::RocketOverHeat:
            {
                result = "RocketOverHeat";
                break;
            }
            case StateAction::SpikeIdle:
            {
                result = "SpikeIdle";
                break;
            }
            case StateAction::SpikeWalk:
            {
                result = "SpikeWalk";
                break;
            }
            case StateAction::SpikeFall:
            {
                result = "SpikeFall";
                break;
            }
            case StateAction::SpikeJump:
            {
                result = "SpikeJump";
                break;
            }
            case StateAction::SpikeLand:
            {
                result = "SpikeLand";
                break;
            }
            case StateAction::SpikeCharge:
            {
                result = "SpikeCharge";
                break;
            }
            case StateAction::SpikeBoost:
            {
                result = "SpikeBoost";
                break;
            }
            case StateAction::SpikeJumpSpring:
            {
                result = "SpikeJumpSpring";
                break;
            }
            case StateAction::SpikeSpecialJump:
            {
                result = "SpikeSpecialJump";
                break;
            }
            case StateAction::SpikeDamageShock:
            {
                result = "SpikeDamageShock";
                break;
            }
            case StateAction::SpikeHomingAttack:
            {
                result = "SpikeHomingAttack";
                break;
            }
            case StateAction::SpikeHomingAttackAfter:
            {
                result = "SpikeHomingAttackAfter";
                break;
            }
            }

            return result;
        }
    public:
        boost::shared_ptr<CMatrixNodeTransform> m_spMatrixNode; // 0x10
        boost::shared_ptr<Hedgehog::Mirage::CMatrixNodeNormal> m_spModelMatrixNode; // 0x18
        boost::shared_ptr<Hedgehog::Mirage::CMatrixNodeNormal> m_spField20; // 0x20
        boost::shared_ptr<Hedgehog::Mirage::CMatrixNodeNormal> m_spField28; // 0x28
        boost::shared_ptr<CMatrixNodeTransform> m_spField30; // 0x30

        boost::shared_ptr<CRayCastCollision> m_spRayCastCollision; // 0x38
        boost::shared_ptr<CRayCastCollision> m_spShapeCastCollision; // 0x40

        Hedgehog::Mirage::CTransform m_StartTransform; // 0x50
        Hedgehog::Mirage::CTransform m_RestartPosition; // 0xB0

        CPlayer* m_pPlayer; // 0x110

        boost::shared_ptr<CCharacterProxy> m_spCharacterProxy; //0x114
        boost::shared_ptr<CInputPad> m_spInputPad; //0x11C

        BB_INSERT_PADDING(0x0C);

        Hedgehog::Math::CVector m_WorldInput; //0x130
        Hedgehog::Math::CVector m_Field140; //0x140
        Hedgehog::Math::CVector m_PathInput2D; //0x150

        uint8_t m_Field160;
        size_t m_Field164;
        uint8_t m_Field168;
        size_t m_RestartCount; // 0x16C

        uint8_t m_Field170;
        uint8_t m_Field171;
        bool m_Is2DMode;
        uint8_t m_Field173;

        size_t m_Field174;
        size_t m_Field178;

        Hedgehog::Math::CVector m_Field180;
        Hedgehog::Math::CVector m_Field190;

        size_t m_SuperRenderableActorID; // 0x1A0

        // HACK: unaligned keyword doesn't work & this is basically a 64bit int at 0x1A4 for some reason.
        // We can just make these private I guess, use a member function to acces them.
    private:
        uint32_t m_SkillsP1; // 0x1A4
        uint32_t m_SkillsP2; // 0x1A8
    public:
        uint64_t Skills()
        {
            return *(uint64_t*)&m_SkillsP1;
        }

        uint8_t m_Field1AC[172];

        float m_Field258;

        hh::map<void*, void*> m_Field25C;
        hh::map<void*, void*> m_Field268;

        boost::shared_ptr<CParameter> m_spParentParameter; // 0x274
        boost::shared_ptr<CParameter> m_spParameter; // 0x27C

        BB_INSERT_PADDING(0xC);

        virtual ~CPlayerContext() = default;

        virtual void CPlayerContext04() {}
        virtual void CPlayerContext08() {}
        virtual void CPlayerContext0C() {}
        virtual void CPlayerContext10() {}
        virtual void CPlayerContext14() {}
        virtual void CPlayerContext18() {}
        virtual void CPlayerContext1C() {}
        virtual void CPlayerContext20() {}
        virtual void CPlayerContext24() {}
        virtual void CPlayerContext28() {}
        virtual void CPlayerContext2C() {}
        virtual void CPlayerContext30() {}
        virtual void CPlayerContext34() {}
        virtual void CPlayerContext38() {}
        virtual void CPlayerContext3C() {}
        virtual void CPlayerContext40() {}
        virtual void CPlayerContext44() {}
        virtual void CPlayerContext48() {}
        virtual void CPlayerContext4C() {}
        virtual void CPlayerContext50() {}
        virtual void CPlayerContext54() {}
        virtual void CPlayerContext58() {}

        virtual void ChangeAnimation(const Hedgehog::Base::CSharedString& name) = 0;

        virtual void CPlayerContext60() {}
        virtual void CPlayerContext64() {}
        virtual void CPlayerContext68() {}
        virtual void CPlayerContext6C() {}
        virtual void CPlayerContext70() {}

        virtual boost::shared_ptr<Hedgehog::Sound::CSoundHandle> PlaySound(size_t cueId, bool loop) = 0;

        virtual void CPlayerContext78() {}
        virtual void CPlayerContext7C() {}
        virtual void CPlayerContext80() {}
        virtual void CPlayerContext84() {}
        virtual void CPlayerContext88() {}
        virtual void CPlayerContext8C() {}
        virtual void CPlayerContext90() {}

        const Hedgehog::Base::CSharedString& GetCurrentAnimationName()
        {
            return fpCPlayerContextGetCurrentAnimationName(this);
        }

        Hedgehog::Universe::TStateMachine<CPlayerContext>::TState* ChangeState(const Hedgehog::Base::CSharedString& type)
        {
            return fCPlayerContextChangeState(this, &type);
        }

        inline Hedgehog::Universe::TStateMachine<CPlayerContext>::TState* ChangeState(StateAction state)
        {
            //const char* stateString = StringFromActionEnum(state);
            //auto stateSharedStr = Hedgehog::Base::CSharedString(stateString);
            return ChangeState(StringFromActionEnum(state));
        }

        const float GetFloatParamByInt(const int param) const
        {
            const float result = fpGetFloatParameter(this->m_spParameter.get(), param);
            return result;
        }

        template<typename T>
        T* ChangeState()
        {
            return static_cast<T*>(ChangeState(T::ms_StateName));
        }
    };

    BB_ASSERT_OFFSETOF(CPlayerContext, m_spMatrixNode, 0x10);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_spModelMatrixNode, 0x18);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_spField20, 0x20);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_spField28, 0x28);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_spField30, 0x30);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_spRayCastCollision, 0x38);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_spShapeCastCollision, 0x40);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_StartTransform, 0x50);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_RestartPosition, 0xB0);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_pPlayer, 0x110);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_spCharacterProxy, 0x114);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_spInputPad, 0x11C);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_WorldInput, 0x130);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field140, 0x140);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_PathInput2D, 0x150);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field160, 0x160);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field164, 0x164);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field168, 0x168);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_RestartCount, 0x16C);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field170, 0x170);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field171, 0x171);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Is2DMode, 0x172);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field173, 0x173);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field174, 0x174);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field178, 0x178);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field180, 0x180);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field190, 0x190);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_SuperRenderableActorID, 0x1A0);

    // Because they're private, these assertions don't work.
    // Set them to public & uncomment these if you want to verify these specifically work.
    //BB_ASSERT_OFFSETOF(CPlayerContext, m_SkillsP1, 0x1A4);
    //BB_ASSERT_OFFSETOF(CPlayerContext, m_SkillsP2, 0x1A8);

    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field1AC, 0x1AC);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field258, 0x258);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field25C, 0x25C);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_Field268, 0x268);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_spParentParameter, 0x274);
    BB_ASSERT_OFFSETOF(CPlayerContext, m_spParameter, 0x27C);
    BB_ASSERT_SIZEOF(CPlayerContext, 0x290);
}