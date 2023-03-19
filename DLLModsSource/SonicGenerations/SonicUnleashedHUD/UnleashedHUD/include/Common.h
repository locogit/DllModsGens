#pragma once

#define PI 3.141592
#define PI_F 3.141592f
#define DEG_TO_RAD PI_F / 180.0f
#define RAD_TO_DEG 180.0f / PI_F

#define DEBUG_VECTOR3(vec3) vec3.x(),vec3.y(),vec3.z()

#include <vector>
#include <fstream>

#include "include\IniReader.h"

typedef void CSonicContext;
CSonicContext** const PLAYER_CONTEXT = (CSonicContext**)0x1E5E2F0;
CSonicContext** const pModernSonicContext = (CSonicContext**)0x1E5E2F8;
CSonicContext** const pClassicSonicContext = (CSonicContext**)0x1E5E304;
CSonicContext** const pSuperSonicContext = (CSonicContext**)0x1E5E310;

uint32_t* const BACKBUFFER_WIDTH = (uint32_t*)0x1DFDDDC;
uint32_t* const BACKBUFFER_HEIGHT = (uint32_t*)0x1DFDDE0;

uint32_t const CStringConstructor = 0x6621A0;
uint32_t const CStringDestructor = 0x661550;
using SharedPtrTypeless = boost::shared_ptr<void>;

enum SonicCollision : uint32_t
{
	TypeNoAttack			= 0x1E61B5C,
	TypeRagdoll				= 0x1E61B60,
	TypeSonicSpinCharge		= 0x1E61B64,
	TypeSonicSpin			= 0x1E61B68,
	TypeSonicUnbeaten		= 0x1E61B6C,
	TypeSuperSonic			= 0x1E61B70,
	TypeSonicSliding		= 0x1E61B74,
	TypeSonicHoming			= 0x1E61B78,
	TypeSonicSelectJump		= 0x1E61B7C,
	TypeSonicDrift			= 0x1E61B80,
	TypeSonicBoost			= 0x1E61B84,
	TypeSonicStomping		= 0x1E61B88,
	TypeSonicTrickAttack	= 0x1E61B8C,
	TypeSonicSquatKick		= 0x1E61B90,
	TypeSonicClassicSpin	= 0x1E61B94,
	TypeExplosion			= 0x1E61B98,
	TypeBossAttack			= 0x1E61B9C,
	TypeGunTruckAttack		= 0x1E61BA0,
	TypeRagdollEnemyAttack	= 0x1E61BA4,
};

enum ImpulseType : uint32_t
{
	None,
	DashPanel,
	UnknowCase_0x2,
	UnknowCase_0x3,
	UnknowCase_0x4,
	UnknowCase_0x5,
	JumpBoard,
	JumpBoardSpecial,
	DashRing,
	DashRingR,
	LookBack,
	HomingAttackAfter,
	BoardJumpBoard,
	UnknowCase_0xD,
	BoardJumpAdlibTrickA,
	BoardJumpAdlibTrickB,
	BoardJumpAdlibTrickC
};

struct CSonicStateFlags
{
	bool EarthGround;
	bool HeadToVelocity;
	bool HeadUpOnly;
	bool SlowHeadToUpDirection;
	bool OutOfControl;
	bool NoLandOutOfControl;
	bool DisableForwardPathInterpolation;
	bool ChangePath;
	bool ChangePathCameraChange;
	bool WallWalkJump;
	bool SupportWalkOnCeiling;
	bool NotifyWalkOnCeiling;
	bool AlwaysDownForce;
	bool DisableDownForce;
	bool Dead;
	bool Goal;
	bool Boost;
	bool FloatingBoost;
	bool StartingBoost;
	bool EndBoost;
	bool DummyBoost;
	bool EndDummyBoost;
	bool Homing;
	bool EnableHomingAttack;
	bool EnableHomingAttackOnDiving;
	bool EnableHomingAttackOutOfControl;
	bool DisableAirBoost;
	bool EnableAirOnceAction;
	bool DisableStomping;
	bool ForceShoesSliding;
	bool DisableShoesSliding;
	bool OnWater;
	bool OnNoDeadWater;
	bool OnAboveWater;
	bool OnSurfaceWater;
	bool OnShallowWater;
	bool OnWaterSeEnabled;
	bool TerrainCollisionEnable;
	bool AirOutOfControl;
	bool NoPadStopWalk;
	bool DisableAirAutoDec;
	bool DisableWallJumpReady;
	bool GroundDistanceZero;
	bool NoPitchRollHorzOnAir;
	bool DisableMoonsault;
	bool KeepRunning;
	bool KeepRunningEnableBackForce;
	bool KeepRunningOnSpiral;
	bool KeepRunningPause;
	bool SShapeRunning;
	bool MoveOnPath;
	bool IgnoreTerrain;
	bool UpdateYawByVelocity;
	bool NoPitchRoll;
	bool NoUpdateYaw;
	bool NoUpdateYawRef;
	bool UpdateYawOnAir;
	bool NoUpdateAdvancedDirection;
	bool CameraChagedPadCorrection;
	bool EnableCameraChagedPadCorrect;
	bool CameraToCullingBasePosEnabled;
	bool NoDamage;
	bool NoDead;
	bool Damaging;
	bool Paralyzing;
	bool ReactinJumpPathChange;
	bool ForcePitchRollGround;
	bool EnableAttenuateJump;
	bool NoGroundFall;
	bool OnStairs;
	bool OnBeltConveyor;
	bool MoveToPositionAndWait;
	bool StopPositionCount;
	bool OnNoWallWalkGround;
	bool ForceLandForCaught;
	bool Restarting;
	bool SlidingAndSquatPressOnly;
	bool SideViewNoPathMove;
	bool Pushing;
	bool NoChange2DPath;
	bool PrepreRestart;
	bool SetDirectRotation;
	bool IsTakingBreath;
	bool OnSpiralGound;
	bool OnMovableGround;
	bool OnFakeMovableGround;
	bool OnMoonsaltDisableGround;
	bool EnableExceptionalBoost;
	bool AcceptSlidingCollision;
	bool AcceptBuoyancyForce;
	bool AcceptEnvForce;
	bool DisableAdlibTrick;
	bool StandbyAdlibTrick;
	bool StandbyChangeToSpin;
	bool Pressing;
	bool FallEnabledInPressing;
	bool OnIntersectionalWay;
	bool ObserveBoostInExternalControl;
	bool ObserveSpinInExternalControl;
	bool ObserveInputInExternalControl;
	bool EscapeEnabledInExternalControl;
	bool NoDamageInExternalControl;
	bool Slipping;
	bool InvokeFlameBarrier;
	bool InvokeAquaBarrier;
	bool InvokeThunderBarrier;
	bool InvokeBarrier;
	bool InvokePreciousTime;
	bool InvokeHighSpeed;
	bool InvokeTimeBreak;
	bool InvokeSkateBoard;
	bool InvokeSuperSonic;
	bool InvokePtmRocket;
	bool InvokePtmSpike;
	bool InvokeUnbeaten;
	bool DoubleJumping;
	bool InvokeFixedMoveOn3D;
	bool KeepStateAfterChangeDimension;
	bool KeepPostureAfterChangeDimension;
	bool CalibrateFrontDir;
	bool EnableAnimationVelocityDirect;
	bool EnableGravityControl;
	bool EnableAirBoostOnGravityControl;
	bool EnableChaosEnergySetting;
	bool ChaosEnergySubstractDisabled;
	bool EnableAutoAim;
	bool ThroughGoalSignboard;
	bool AutoBoost;
	bool MaxSpeedLimited;
	bool Pause;
	bool SkateBoardSlowDown;
	bool ChangeCollisionLocked;
	bool ReadyGoOpened;
	bool SpikeSpin;
	bool ChangePostureInDeadAirEnabled;
	bool DisableGroundSmoke;
	bool Drifting;
	bool DriftingLowSpeed;
	bool DriftRight;
	bool Squat;
	bool LightSpeedDashReady;
	bool BoostKeep;
	bool DisableCrashWall;
	bool WallCorrection;
	bool Diving;
	bool DivingFloat;
	bool HipSliding;
	bool GrindFullBalance;
	bool GrindSideJump;
	bool Freeze;
	bool DisableBoost;
	bool NoSquatToSliding;
	bool IgnorePadInput;
	bool Rising;
	bool SpinChargeSliding;
	bool CombinationAttacking;
	bool SpinDash;
	bool LightAttack;
	bool AtomicSpin;
	bool Binding;
	bool ChgPlayerDisabled;
	bool EndReflection;
	bool CorrectOnPath;
	bool DebugDrawGroundHitPosition;
	bool DebugDrawVelocity;
	bool DebugDrawPath;
	bool DebugDrawSpikeInfo;
	bool DebugExceptionalMove;
};

enum EPlayerParameter
{
	AccelerationForce = 0x1C,
	AccelerationForceBaseUp = 0x20,
	AccelerationForceBySkill = 0x1D,
	AccelerationForceOnBoard = 0x1E,
	AccelerationForceOnBoardSlow = 0x1F,
	AccelerationForceOnSpike = 0x1DE,
	AccelerationForceSps = 0x22,
	ActionEnableAfterHomingAttack = 0x11D,
	AdTrickAddChaosEnergyBonus = 0x180,
	AdTrickAddChaosEnergyLv1 = 0x17D,
	AdTrickAddChaosEnergyLv2 = 0x17E,
	AdTrickAddChaosEnergyLv3 = 0x17F,
	AdTrickAddChaosEnergyMin = 0x17C,
	AdTrickAnimSpeedOnSkill = 0x186,
	AdTrickGameSpeed = 0x183,
	AdTrickGameSpeedAwakeTime = 0x185,
	AdTrickGameSpeedAwakeTimeOfStart = 0x182,
	AdTrickGameSpeedOfStart = 0x181,
	AdTrickGameSpeedSV = 0x184,
	AdTrickVelocityXZOfHorz = 0x267,
	AdTrickVelocityXZOfRolling = 0x265,
	AdTrickVelocityXZOfVert = 0x269,
	AdTrickVelocityYOfHorz = 0x266,
	AdTrickVelocityYOfRolling = 0x264,
	AdTrickVelocityYOfVert = 0x268,
	AdditionalLimitTime = 0xAA,
	AirBoostCalibratedAirDragScale = 0x170,
	AirBoostDisableTimeAfterSpinAttack = 0x16C,
	AirBoostEnableChaosEnergy = 0x167,
	AirBoostFirstDecrease = 0x16D,
	AirBoostGravityRate = 0x168,
	AirBoostLevitationForce = 0x169,
	AirBoostLevitationTime = 0x16A,
	AirBoostStartVelocityRate = 0x16E,
	AirBoostTime = 0x16B,
	AisacSpeed1st = 0x1BF,
	AisacSpeed2nd = 0x1C0,
	AisacSpeed3rd = 0x1C1,
	AtomicSpinChargeMaxTime = 0x1ED,
	AtomicSpinChargeMinTime = 0x1EC,
	AtomicSpinChargeOffsX = 0x1EE,
	AtomicSpinChargeOffsY = 0x1EF,
	AtomicSpinChargeOffsZ = 0x1F0,
	AtomicSpinHomingAngle = 0x1F1,
	AtomicSpinMaxTime = 0x1EB,
	AtomicSpinMinTime = 0x1EA,
	AtomicSpinMoveTime = 0x1F5,
	AtomicSpinStartHomingTime = 0x1F4,
	AtomicSpinVelocity = 0x1E9,
	AtomicSpinVelocityKeep = 0x1F2,
	AtomicSpinVelocityMove = 0x1F3,
	AtomicSpinVelocityReturn = 0x1F6,
	AttackAfterImpluseVelocityCoeff = 0x148,
	BallJumpAirDragRateScale = 0x4D,
	BlinkNoDamageTime = 0x6A,
	BlockingEnemyMaxNum = 0xD4,
	BoardMaxPitchVelocity = 0x1B2,
	BoardMinPitchVelocity = 0x1B1,
	BoostAccelerationForce = 0x163,
	BoostAccelerationForceWhenSlow = 0x164,
	BoostBeginCameraShakeCount = 0x153,
	BoostBeginCameraShakeSize = 0x151,
	BoostBeginCameraShakeTime = 0x152,
	BoostCancelBrakeTime = 0x165,
	BoostEnableChaosEnergy = 0x14E,
	BoostEndKeepTime = 0x15E,
	BoostFirstDecrease = 0x158,
	BoostInAirKeepTime = 0x15D,
	BoostMaxVelocityScaleLv0 = 0x161,
	BoostMaxVelocityScaleLvMax = 0x162,
	BoostMinVelocity = 0x15C,
	BoostPullRadius = 0x154,
	BoostStartVelocityRate = 0x15A,
	BoostStrength = 0x15F,
	BoostTimeDecrease = 0x159,
	BoostTimeDecreaseOnBoard = 0x150,
	BoostTimeDecreaseOnWater = 0x14F,
	BorderTimeOfBrakeCancelForStartDrift = 0x1E0,
	BoudnJumpVelocity = 0x195,
	BoundJumpBeforeVelocity = 0x194,
	BoundJumpGravityScale = 0x196,
	BrakeCancelAngle = 0x46,
	BrakeDownForceEnableSpeed = 0x43,
	BrakeEnableAngle = 0x45,
	BrakeEnableSpeed = 0x41,
	BrakeEndSpeed = 0x40,
	BrakeForce = 0x42,
	BrakeForceScaleOfLarge = 0x48,
	BrakeForceScaleOfSkill = 0x49,
	BrakeLargeSpeed = 0x47,
	BuoyantForceMaxGravityRate = 0xA6,
	BuoyantForceMaxVelocity = 0xA3,
	BuoyantForceZeroVelocity = 0xA2,
	CameraOffsetReverseVelocityLength = 0x25F,
	CameraTargetOffsetSensitive = 0xB1,
	CameraTargetUpOffset = 0xAE,
	CameraTargetUpOffsetInAir = 0xAF,
	CameraTargetUpOffsetSensitive = 0xB0,
	ChaosEnergyAutoRecoverRate = 0x14D,
	ChaosEnergyRecoverRateByRing = 0x14A,
	ChaosEnergyRecoverRateByRingBonus = 0x14B,
	ChaosEnergyRecoverRateByRingPenalty = 0x14C,
	CharProxySpeedThreshold = 0x8D,
	CharProxyStaticFriction = 0x8C,
	CollisionPushingRadius = 0xC1,
	CollisionRadius = 0xC0,
	CollisionSquatHeight = 0xC3,
	CollisionStandHeight = 0xC2,
	CrashWallBorder = 0x50,
	DamageAirJumpPermitDistance = 0x6C,
	DamageCancelHorzVelocity = 0x70,
	DamageCancelVertVelocity = 0x6F,
	DamageDecForceOfWallWalk = 0x6B,
	DamageDownTime = 0x67,
	DamageElectricTime = 0x72,
	DamageFireTime = 0x71,
	DamageFreezeAirKeepTime = 0x73,
	DamageFreezeReflectEfficient = 0x75,
	DamageFreezeTime = 0x74,
	DamageHorzDecRateOnBoard = 0x6E,
	DamageMoveScale = 0x68,
	DamageStandUpCancelTime = 0x69,
	DamageVertVelOnBoard = 0x6D,
	DeadBgmFadeTime = 0x78,
	DeadBgmWaitTime = 0x77,
	DeadToRestartTime = 0x76,
	DebugNoDamage = 0x1C2,
	DebugNoDead = 0x1C3,
	DebugNoDeadDrowning = 0x1C6,
	DebugNoDrowningImmediately = 0x1C5,
	DebugNoFallDead = 0x1C4,
	DebugResultRankLv = 0x4,
	DebugSEEnabled = 0x0,
	DebugSkillCancelEnabled = 0x1C7,
	DebugSkillSpikeEndless = 0x1C8,
	DebugSkillSuperSonic = 0x1C9,
	DebugVoiceEnabled = 0x1,
	DecelerationForceBoardTurn = 0x26,
	DecelerationForceHigh = 0x24,
	DecelerationForceLow = 0x25,
	DistanceFrom2DPath = 0x1DE,
	DivingDamageTime = 0x228,
	DivingDamageVelocityDecreaseRate = 0x227,
	DivingDiveBeginGravityScale = 0x21F,
	DivingDiveBeginHorzVelocityRate = 0x21E,
	DivingDiveEndSpeed = 0x223,
	DivingDiveMaxVelocityDown = 0x220,
	DivingDiveVelocityFront = 0x222,
	DivingDiveVelocitySide = 0x221,
	DivingFloatMaxVelocityDown = 0x224,
	DivingFloatVelocityFront = 0x226,
	DivingFloatVelocitySide = 0x225,
	DivingRotationMax = 0x21D,
	DownForceEfficient = 0x59,
	DriftAngleReturnForce = 0x20F,
	DriftBoostYawOffset = 0x217,
	DriftFinishAngle = 0x215,
	DriftFinishSideVelocity = 0x216,
	DriftFrontAccelForceRate = 0x214,
	DriftFrontAngleForce = 0x211,
	DriftMaxAngleVelocity = 0x212,
	DriftSideAngleForce = 0x210,
	DriftSideDecForce = 0x213,
	DynamicFrictionalForce = 0x95,
	DynamicFrictionalForceCoef = 0x96,
	DynamicResistanceForceWhenRun = 0x8E,
	DynamicResistanceForceWhenStand = 0x8F,
	EnableHurdleJumpMinVelocity = 0x1CA,
	EnemyBlockedJumpPower1 = 0xD7,
	EnemyBlockedJumpPowerDec = 0xD8,
	EnemyBlockedMaxSquatVelocity = 0x218,
	EnemyBlockedMaxVelocity1 = 0xD5,
	EnemyBlockedMaxVelocityDec = 0xD6,
	EnemyBlockedRotationForce1 = 0xD9,
	EnemyBlockedRotationForceDec = 0xDA,
	EnemyBlockedRotationSpeedMax1 = 0xDB,
	EnemyBlockedRotationSpeedMaxDec = 0xDC,
	EnemyBlockedSpringImpulseMax1 = 0xDF,
	EnemyBlockedSpringImpulseMaxDec = 0xE0,
	EnemyBlockedSpringImpulseScale1 = 0xDD,
	EnemyBlockedSpringImpulseScaleDec = 0xDE,
	EnemyComboTime = 0x235,
	FinishSlidingVelocity = 0x1CE,
	ForceGroundAttribute = 0x2,
	ForceResultRank = 0x3,
	FrontNoGroundStopMaxInput = 0xFF,
	FrontNoGroundStopMaxSpeed = 0xFE,
	FrontNoGroundStopSearchLength = 0xFD,
	GhostAutoRec = 0x12,
	Gravity = 0x14,
	GrindAccelerationForce = 0x204,
	GrindBeginMinSpeed = 0x199,
	GrindCurveDecreaseForce = 0x209,
	GrindDamageTime = 0x206,
	GrindDamageVelocityDecreaseRate = 0x207,
	GrindJumpPowerMin = 0x198,
	GrindJumpShortReleaseTime = 0x197,
	GrindJumpSideMaxDistance = 0x19A,
	GrindJumpSideSearchFrontOffsetTime = 0x19B,
	GrindMinSpeed = 0x205,
	GrindSEMaxPitch = 0x19E,
	GrindSEMaxPitchVelocity = 0x19F,
	GrindSEMinPitch = 0x19C,
	GrindSEMinPitchVelocity = 0x19D,
	GrindSideJumpForce = 0x208,
	GrindStumbleLimit = 0x20A,
	GrindStumbleLimitTime = 0x20B,
	GroundSearchDiameter = 0x144,
	HangOnJumpHorzVel = 0x1A2,
	HangOnJumpPower = 0x1A3,
	HangOnJumpVelCoeffDown = 0x1A1,
	HangOnJumpVelCoeffUp = 0x1A0,
	HighSpeedEffectMaxVelocity = 0x261,
	HighSpeedEffectMaxVelocityDuringFall = 0x263,
	HighSpeedEffectMinVelocity = 0x260,
	HighSpeedEffectMinVelocityDuringFall = 0x262,
	HipSlidingMaxVelocity = 0x17A,
	HipSlidingVelocity = 0x179,
	HomingAttackAfterUpVelocity = 0x123,
	HomingAttackEffectWidth = 0x13B,
	HomingAttackEnableAfterFall = 0x124,
	HomingAttackEnableVertUp = 0x11A,
	HomingAttackSplinePosCount = 0x13A,
	HomingBackEnableSpeed = 0x125,
	HomingCalibratedAirDragScale = 0x120,
	HomingInertiaForceRate = 0x121,
	HomingLockonCollisionAspect = 0x127,
	HomingLockonCollisionAspectN = 0x12E,
	HomingLockonCollisionFar = 0x12A,
	HomingLockonCollisionFarN = 0x131,
	HomingLockonCollisionFovy = 0x128,
	HomingLockonCollisionFovyN = 0x12F,
	HomingLockonCollisionNear = 0x129,
	HomingLockonCollisionNearN = 0x130,
	HomingLockonCollisionOffsetX = 0x12C,
	HomingLockonCollisionOffsetXN = 0x133,
	HomingLockonCollisionOffsetY = 0x12D,
	HomingLockonCollisionOffsetYN = 0x134,
	HomingLockonCollisionRotationX = 0x12B,
	HomingLockonCollisionRotationXN = 0x132,
	HomingLockonKeepBack = 0x139,
	HomingLockonKeepDownOffset = 0x138,
	HomingLockonKeepDownSlope = 0x137,
	HomingLockonKeepUpOffset = 0x136,
	HomingLockonKeepUpSlope = 0x135,
	HomingNoLockonKeepTime = 0x122,
	HomingRotateForce = 0x11C,
	HomingSpeed = 0x11E,
	HomingSpeedOfDummy = 0x11F,
	HorzStandEnableMaxSlope = 0xB3,
	HurdleFirstAnimSpeed = 0x232,
	HurdleJumpRotateZRate = 0x22B,
	HurdleReleaseAnimSpeed = 0x233,
	IdleActionTime = 0x145,
	IdleSleepActionCount = 0x146,
	InAirForceAccelRate = 0x27,
	InAirForceAccelRateInLowSpd = 0x2C,
	InAirForceAccelRateOnBoard = 0x31,
	InAirForceBrakeRate = 0x29,
	InAirForceBrakeRateInLowSpd = 0x2E,
	InAirForceBrakeRateOnBoard = 0x33,
	InAirForceDecRate = 0x28,
	InAirForceDecRateInLowSpd = 0x2D,
	InAirForceDecRateOnBoard = 0x32,
	InAirLowSpdThreshold = 0x30,
	InAirMaxAccelVelocity = 0x2A,
	InAirMaxAccelVelocityInLowSpd = 0x2F,
	InAirMaxAccelVelocityOfNormalJump = 0x2B,
	IncreaseChaosEnergyDriftPerSecond = 0x1DD,
	IncreaseChaosEnergyDriftStart = 0x1DC,
	InputSubStickInTrickCommand = 0x11,
	InputThreshold = 0xE,
	InputTransformHalf = 0xF,
	InputTransformPower = 0x10,
	InternalScaleInReflection = 0x1DD,
	IsEnableBoost3DEffect = 0x166,
	IsEnableCrashWall = 0x51,
	IsEnableEffect = 0x5,
	IsHomingLockonCollisionEdit = 0x126,
	IsLightSpeedDashCollisionEdit = 0x1F4,
	IsLightSpeedDashHoldMode = 0x202,
	IsLightSpeedDashStartCollisionEdit = 0x1EC,
	ItemCollisionHeightInDiving = 0x229,
	ItemCollisionRadius = 0xBA,
	ItemCollisionRadiusInBoost = 0xBD,
	ItemCollisionRadiusInDiving = 0x22A,
	ItemCollisionSquatHeight = 0xBC,
	ItemCollisionSquatHeightInBoost = 0xBF,
	ItemCollisionStandHeight = 0xBB,
	ItemCollisionStandHeightInBoost = 0xBE,
	JumpGroundVelocityXZRate = 0x3E,
	JumpGroundVelocityYRate = 0x3D,
	JumpPower = 0x36,
	JumpPowerAllRounder = 0x3C,
	JumpPowerBySkill = 0x37,
	JumpPowerDamageAir = 0x38,
	JumpPowerMin = 0x3A,
	JumpPowerMinOnBoard = 0x3B,
	JumpPowerOnBoard = 0x39,
	JumpShortGroundResetDistance = 0x4B,
	JumpShortReleaseTime = 0x4A,
	KeepRunningDamageInternalScale = 0x106,
	KeepRunningDamageTime = 0x105,
	KeepRunningDamageVelocityDecreaseRate = 0x104,
	KeepRunningMaxVelocityOnBoard = 0x109,
	KeepRunningMinVelocity = 0x102,
	KeepRunningMinVelocityDebug = 0x107,
	KeepRunningMinVelocityOnBoard = 0x108,
	KickbackPower = 0x3F,
	LandEnableMaxSlope = 0xB2,
	LandStrongMaxHorzSpeed = 0xB5,
	LandStrongMinVertSpeed = 0xB4,
	LightAttackChargeTime = 0x1DF,
	LightAttackColRadius0 = 0x1E2,
	LightAttackColRadius1 = 0x1E3,
	LightAttackDurationTime = 0x1E6,
	LightAttackEffScale0 = 0x1E4,
	LightAttackEffScale1 = 0x1E5,
	LightAttackVelocity0 = 0x1E0,
	LightAttackVelocity1 = 0x1E1,
	LightAttackVelocityKeep = 0x1E8,
	LightAttackVelocityReturn = 0x1E7,
	LightSpeedDashCollisionAspect = 0x1F5,
	LightSpeedDashCollisionFar = 0x1F8,
	LightSpeedDashCollisionFovy = 0x1F6,
	LightSpeedDashCollisionNear = 0x1F7,
	LightSpeedDashCollisionOffsetX = 0x1FA,
	LightSpeedDashCollisionOffsetY = 0x1FB,
	LightSpeedDashCollisionRotationX = 0x1F9,
	LightSpeedDashEnableAngle = 0x201,
	LightSpeedDashHoldTime = 0x200,
	LightSpeedDashMaxVelocity = 0x1FD,
	LightSpeedDashMaxVelocity3D = 0x1FF,
	LightSpeedDashMinVelocity = 0x1FC,
	LightSpeedDashMinVelocity3D = 0x1FE,
	LightSpeedDashStartCollisionAspect = 0x1ED,
	LightSpeedDashStartCollisionFar = 0x1F0,
	LightSpeedDashStartCollisionFovy = 0x1EE,
	LightSpeedDashStartCollisionNear = 0x1EF,
	LightSpeedDashStartCollisionOffsetX = 0x1F2,
	LightSpeedDashStartCollisionOffsetY = 0x1F3,
	LightSpeedDashStartCollisionRotationX = 0x1F1,
	LimitTimeOfSpin = 0x1B,
	LimitTimeToWaterWalk = 0xA9,
	LostGridSideSpeedRate = 0x99,
	MaxDownVelocity = 0xC4,
	MaxHorzVelocity = 0xC6,
	MaxUpSpeedWhenBound = 0xFB,
	MaxUpSpeedWhenJump = 0xFA,
	MaxUpVelocity = 0xC5,
	MaxVelocityBasis = 0x16,
	MaxVelocityBasisBaseUp = 0x21,
	MaxVelocityBasisSps = 0x23,
	MaxVelocityCoefOnSpike = 0x1DF,
	MaxVelocityFinalMax = 0x17,
	MaxVelocitySonicToSpin = 0x1A,
	MaxVelocitySpinToSonic = 0x19,
	ModelScaleInPlayableMenu = 0x149,
	MotionDashToJet = 0x231,
	MotionJogToRun = 0x22F,
	MotionRunToDash = 0x230,
	MotionStandToWalk = 0x22C,
	MotionWalkFastToJog = 0x22E,
	MotionWalkToWalkFast = 0x22D,
	MoveEnableGroundAngleInner = 0xB7,
	MoveEnableGroundAngleOuter = 0xB8,
	MoveEnableSearchDistance = 0xB9,
	MoveHoldingMinVelocity = 0xB6,
	NoInputTimeInReflection = 0x1DC,
	NoPadStopWalkBeginMaxVelocity = 0xD3,
	NoPadStopWalkDecelerationForce = 0xD1,
	NoPadStopWalkPadLengthLimit = 0xD0,
	NormalStrength = 0x8B,
	PadCorrectWhenCameraChangeAlwaysAccelTime = 0x140,
	PadCorrectWhenCameraChangeKeepTime = 0x141,
	PadInputForceCoeffInAir = 0x142,
	ParaloopInterval = 0x156,
	ParaloopMaxLength = 0x155,
	ParaloopMinSpeed = 0x157,
	PathBinrmCorrectionAngle = 0x13C,
	PathBinrmCorrectionOffset = 0x13D,
	PitchRollRotateMaxVelocity = 0xF9,
	PitchRollRotateMinVelocity = 0xF8,
	PitchRollRotateSpeedAccele = 0xF4,
	PitchRollRotateSpeedInAirRate = 0xF5,
	PitchRollRotateSpeedMax = 0xF2,
	PitchRollRotateSpeedMaxDif = 0xF0,
	PitchRollRotateSpeedMaxVelocity = 0xF7,
	PitchRollRotateSpeedMin = 0xF3,
	PitchRollRotateSpeedMinDif = 0xF1,
	PitchRollRotateSpeedMinVelocity = 0xF6,
	PrecedeJumpEnableTime = 0xFC,
	ReactionFailPushOutForce = 0x5D,
	ReactionJumpGravity = 0x62,
	ReactionJumpMaxEnterVelocity = 0x61,
	ReactionJumpMaxVelocity = 0x5F,
	ReactionJumpMinEnterVelocity = 0x60,
	ReactionJumpMinVelocity = 0x5E,
	RecoverGridSideSpeedRate = 0x98,
	RecoverGridSpeed = 0x97,
	ResistanceStandRunBorder = 0x92,
	ResistanceStandRunIntgerpolate = 0x93,
	ResultPosOffsetX = 0x1F8,
	ResultPosOffsetY = 0x1F9,
	ResultPosOffsetZ = 0x1FA,
	RingDropAdditionalNum = 0x188,
	RingDropMin = 0x187,
	RingSubtractTime = 0x1F7,
	RocketAccel = 0x1A6,
	RocketChargeEndTime = 0x1A4,
	RocketForceScale = 0x1AA,
	RocketLaunchTime = 0x1A5,
	RocketMaxVelocity = 0x1A9,
	RocketMinVelocity = 0x1A8,
	RocketVelocity = 0x1A7,
	RotationForce0 = 0xE1,
	RotationForce1 = 0xE3,
	RotationForce2 = 0xE5,
	RotationForceInAir = 0xEC,
	RotationForceInBoardAir = 0xEE,
	RotationForceInBoost = 0xEA,
	RotationForceMaxAngle = 0xCF,
	RotationForceMaxAngleLw = 0xCB,
	RotationForceMinAngle = 0xCE,
	RotationForceMinAngleLw = 0xCA,
	RotationForceRateInAir = 0xED,
	RotationForceRateInBoardAir = 0xEF,
	RotationForceRateMax = 0xCD,
	RotationForceRateMaxLw = 0xC9,
	RotationForceRateMin = 0xCC,
	RotationForceRateMinLw = 0xC8,
	RotationForceThresholdSpd = 0xC7,
	RotationForceVelocity0 = 0xE2,
	RotationForceVelocity1 = 0xE4,
	RotationForceVelocity2 = 0xE6,
	RotationSpeedMax = 0xE7,
	RotationSpeedMaxShoesSliding = 0xE9,
	RotationSpeedMaxSpike = 0xE8,
	RunQuickStepMoveDistance = 0x1D7,
	SkillAddChaosEnergyOnBoardJump = 0x193,
	SkillCountAquaBarrier = 0x18F,
	SkillCountBarrier = 0x18D,
	SkillCountFlameBarrier = 0x18E,
	SkillCountHighSpeed = 0x191,
	SkillCountPreciousTime = 0x192,
	SkillCountSkateBoard = 0x18C,
	SkillCountThunderBarrier = 0x190,
	SkillHighSpeedTime = 0x18B,
	SkillPreciousTime = 0x18A,
	SkillUnbeatenTime = 0x189,
	SlidingBrakeForce = 0x1D0,
	SlidingMinTime = 0x1D1,
	SlipToWalkVelocity = 0x17B,
	SlopeVelocityKeepRate = 0x117,
	SparkleEffectCount = 0x243,
	SparkleEffectIntervalTime = 0x24D,
	SparkleEffectLife = 0x244,
	SparkleEffectLifeRand = 0x245,
	SparkleEffectMeshPointCount = 0x24A,
	SparkleEffectRandDir = 0x24B,
	SparkleEffectRandSpeed = 0x24C,
	SparkleEffectSpeedMax = 0x249,
	SparkleEffectSpeedMin = 0x248,
	SparkleEffectWidth = 0x246,
	SparkleEffectWidthRand = 0x247,
	SpikeBoostChargeTime1 = 0x1AE,
	SpikeBoostChargeTime2 = 0x1AF,
	SpikeBoostVelocity0 = 0x1AB,
	SpikeBoostVelocity1 = 0x1AC,
	SpikeBoostVelocity2 = 0x1AD,
	SpikeInvokeTime = 0x1B0,
	SpinAttackAfterUpVelocity = 0x176,
	SpinAttackAfterUpVelocityStrong = 0x177,
	SpinAttackThresholdVelocity = 0x178,
	SpinChargeTimeLv0 = 0x24E,
	SpinChargeTimeLv1 = 0x24F,
	SpinChargeTimeLv2 = 0x250,
	SpinDecMaxVelocity = 0x25E,
	SpinDecRate = 0x25A,
	SpinDecRateBySkill = 0x25B,
	SpinDecStartVelocity = 0x25D,
	SpinDecVelocity = 0x25C,
	SpinDynamicFrictionalForce = 0x258,
	SpinDynamicFrictionalForceBySkill = 0x259,
	SpinMinTime = 0x256,
	SpinTimeOfKeeprunning = 0x257,
	SpinVelocityWithChargeLv0 = 0x251,
	SpinVelocityWithChargeLv1 = 0x252,
	SpinVelocityWithChargeLv2 = 0x253,
	SpinVelocityWithSlidingChargeLv1 = 0x254,
	SpinVelocityWithSlidingChargeLv2 = 0x255,
	SpringImpluseVelocityCoeff = 0x147,
	SpsDecRingTime = 0x1B3,
	SpsDecRingTimeBoost = 0x1B4,
	SpsFloatingHeight = 0x1B9,
	SpsFloatingMoveSide = 0x1B7,
	SpsFloatingMoveSideSpd = 0x1B8,
	SpsFloatingMoveUp = 0x1B5,
	SpsFloatingMoveUpSpd = 0x1B6,
	SquatAndSlidingStairsMax = 0x9B,
	SquatKickInputTime = 0x234,
	SquatToSlidingSlope = 0x1CF,
	StaggerNoInputSecond = 0x175,
	StaggerSearchDistance = 0x174,
	StaggerStartVelocityLength = 0x173,
	StairsMax = 0x9A,
	StairsSensitive = 0x9C,
	StandToWalkVelocity = 0x143,
	StartDriftFromSlidingBorder = 0x1E3,
	StartDriftInputAngleBorder = 0x1E2,
	StartTimeToCountDown = 0xAB,
	StaticFrictionalForce = 0x94,
	StaticResistanceForceWhenRun = 0x90,
	StaticResistanceForceWhenStand = 0x91,
	StompingDisableTime = 0x1EA,
	StompingDownFirstVelocity = 0x1E7,
	StompingHorzDecForce = 0x1EB,
	StompingStrength = 0x20E,
	StumbleDecRate = 0x55,
	StumbleDecTime = 0x56,
	StumbleHeightMax = 0x53,
	StumbleInternalScale = 0x57,
	StumbleTime = 0x52,
	StumbleVelocityMin = 0x54,
	TramJumpPowerAdvancing = 0x1BA,
	TramJumpPowerBreak = 0x1BB,
	VelocityAdjustSpeed = 0x1DB,
	VelocityByABSRay = 0x1D8,
	VelocityInputVert = 0x1D6,
	VelocityOnDamage = 0x1D9,
	VelocityReflection = 0x1D7,
	VelocityStrengthScale = 0x116,
	VelocityUpInFloatingBoost0 = 0x34,
	VelocityUpInFloatingBoost1 = 0x35,
	VelocityUpOnDamage = 0x1DA,
	VertVelocityBallToFall = 0x58,
	VirtualStairsEnableMaxVelocity = 0x13F,
	VirtualStairsMax = 0x13E,
	WalkBrakeForce = 0x44,
	WalkSideMotionMaxInput = 0x111,
	WalkSideMotionMinInput = 0x110,
	WalkSideMotionMovePathScale = 0x112,
	WalkSideMotionSensitive = 0x113,
	WalkWallMotionSlopeAngleMax = 0x115,
	WalkWallMotionSlopeAngleMin = 0x114,
	WallJumpAMinTime = 0x86,
	WallJumpAtoBDistance = 0x88,
	WallJumpAtoBTime = 0x87,
	WallJumpBtoCSpeed = 0x89,
	WallJumpFrontForce = 0x85,
	WallJumpOutOfControlTime = 0x8A,
	WallJumpReadyAccel = 0x83,
	WallJumpReadyAccelStartTime = 0x82,
	WallJumpReadyBorder = 0x7A,
	WallJumpReadyDisableAirTime = 0x7B,
	WallJumpReadyGravity = 0x7E,
	WallJumpReadyMaxDownSpeed = 0x7F,
	WallJumpReadyMaxTime = 0x81,
	WallJumpReadyMinBottomDistance = 0x7C,
	WallJumpReadyMinTime = 0x80,
	WallJumpReadyStopTime = 0x7D,
	WallJumpUpForce = 0x84,
	WallKickForce = 0x10F,
	WallPushForce = 0x10E,
	WallPushLimitCharacterAngle = 0x10B,
	WallPushLimitInputIntensity = 0x10A,
	WallPushSpeed = 0x10D,
	WallPushStartTime = 0x10C,
	WallRunCancelVelocity = 0x118,
	WallRunCancelVelocityNoRun = 0x119,
	WaterAddVelocity = 0xAD,
	WaterDeadDownDepthMax = 0x9F,
	WaterDeadDownDepthMaxSpeed = 0x9E,
	WaterDeadDownDepthMin = 0x9D,
	WaterDecreaseForce = 0xA7,
	WaterDecreaseForceInDeepDepth = 0xA8,
	WaterDrowningDepthMin = 0xA0,
	WaterDrowningNoBoostTime = 0xA1,
	WaterGravityScale = 0xAC,
	WaterTensionMaxVelocity = 0xA5,
	WaterTensionZeroVelocity = 0xA4,
	WindEffectCount = 0x236,
	WindEffectDirOffset = 0x23A,
	WindEffectIntervalTime = 0x241,
	WindEffectLife = 0x23F,
	WindEffectLifeRand = 0x240,
	WindEffectMeshPointCount = 0x237,
	WindEffectRandAmplitude = 0x238,
	WindEffectRandFrequency = 0x239,
	WindEffectSpeedMax = 0x23C,
	WindEffectSpeedMin = 0x23B,
	WindEffectStartPosRange = 0x242,
	WindEffectWidth = 0x23D,
	WindEffectWidthRand = 0x23E,
	XButtonHoming = 0x11B,
};

enum StageMissionType : uint32_t
{
	SMT_ghz100 = 0x00,
	SMT_ghz200 = 0x01,
	SMT_cpz100 = 0x02,
	SMT_cpz200 = 0x03,
	SMT_ssz100 = 0x04,
	SMT_ssz200 = 0x05,
	SMT_sph100 = 0x06,
	SMT_sph200 = 0x07,
	SMT_cte100 = 0x08,
	SMT_cte200 = 0x09,
	SMT_ssh100 = 0x0A,
	SMT_ssh200 = 0x0B,
	SMT_csc100 = 0x0C,
	SMT_csc200 = 0x0D,
	SMT_euc100 = 0x0E,
	SMT_euc200 = 0x0F,
	SMT_pla100 = 0x10,
	SMT_pla200 = 0x11,
	SMT_cnz100 = 0x12,
	SMT_emu	= 0x13,
	SMT_bms	= 0x14,
	SMT_bsd	= 0x15,
	SMT_bsl	= 0x16,
	SMT_bde	= 0x17,
	SMT_bpc	= 0x18,
	SMT_bne	= 0x19,
	SMT_blb	= 0x1A,
	SMT_pam000 = 0x1B,
	SMT_fig000 = 0x1C,
	SMT_evt041 = 0x1D,
	SMT_evt105 = 0x1E,
	SMT_evt121 = 0x1F,

	SMT_Mission1 = 0x100,
	SMT_Mission2 = 0x200,
	SMT_Mission3 = 0x300,
	SMT_Mission4 = 0x400,
	SMT_Mission5 = 0x500,

	SMT_BossHard = 0x100,
};

namespace Common
{

	inline float* GetPlayerBoost()
	{
		if (!*PLAYER_CONTEXT) return 0;
		return (float*)((uint32_t)*PLAYER_CONTEXT + 0x5BC);
	}

	inline uint32_t GetMultiLevelAddress(uint32_t initAddress, std::vector<uint32_t> offsets)
	{
		uint32_t address = *(uint32_t*)initAddress;
		for (uint32_t i = 0; i < offsets.size(); i++)
		{
			uint32_t const& offset = offsets[i];
			address += offset;

			if (i < offsets.size() - 1)
			{
				address = *(uint32_t*)address;
			}
		}
		return address;
	}

	inline bool CheckCurrentStage(char const* stageID)
	{
		char const* currentStageID = (char*)0x01E774D4;
		return strcmp(currentStageID, stageID) == 0;
	}

	inline uint32_t GetCurrentStageID()
	{
		uint32_t stageIDAddress = GetMultiLevelAddress(0x1E66B34, { 0x4, 0x1B4, 0x80, 0x0 });
		return *(uint32_t*)stageIDAddress;
	}

	inline bool IsCurrentStageBossBattle() {
		uint8_t stageID = GetCurrentStageID() & 0xFF;
		return stageID >= 0x13 && stageID <= 0x1A;
	}

	inline bool IsCurrentStageMission()
	{
		uint8_t missionNumber = (GetCurrentStageID() & 0xFF00) >> 8;
		return !IsCurrentStageBossBattle() && missionNumber > 0;
	}

	inline bool IsAtLoadingScreen()
	{
		uint32_t** hudCount = (uint32_t**)0x1E66B40;
		if (!*hudCount) return false;
		return (*hudCount)[2] > 0;
	}

	inline void PlaySoundStatic(SharedPtrTypeless& soundHandle, uint32_t cueID)
	{
		uint32_t* syncObject = *(uint32_t**)0x1E79044;
		if (syncObject)
		{
			FUNCTION_PTR(void*, __thiscall, sub_75FA60, 0x75FA60, void* This, SharedPtrTypeless&, uint32_t cueId);
			sub_75FA60((void*)syncObject[8], soundHandle, cueID);
		}
	}
	
	inline CSonicStateFlags* GetSonicStateFlags()
	{
		auto* const context = reinterpret_cast<int*>(*PLAYER_CONTEXT);
		return reinterpret_cast<CSonicStateFlags*>(*reinterpret_cast<int*>(context[0x14D] + 4));
	}
	
	inline bool IsPlayerSuper()
	{
		if (!*PLAYER_CONTEXT) return false;
		return GetSonicStateFlags()->InvokeSuperSonic;
	}

	inline bool IsFileExist(std::string const& file)
	{
		struct stat buffer;
		return stat(file.c_str(), &buffer) == 0;
	}

	inline void GetModIniList(std::vector<std::string>& modIniList)
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		std::string exePath(buffer);
		std::string cpkRedirConfig = exePath.substr(0, exePath.find_last_of("\\")) + "\\cpkredir.ini";

		if (!Common::IsFileExist(cpkRedirConfig))
		{
			printf("%s not exist.\n", cpkRedirConfig.c_str());
			return;
		}

		INIReader reader(cpkRedirConfig);
		std::string modsDatabase = reader.Get("CPKREDIR", "ModsDbIni", "mods\\ModsDB.ini");

		if (!Common::IsFileExist(modsDatabase))
		{
			printf("%s not exist.\n", modsDatabase.c_str());
			return;
		}

		INIReader modsDatabaseReader(modsDatabase);
		int count = modsDatabaseReader.GetInteger("Main", "ActiveModCount", 0);
		for (int i = 0; i < count; i++)
		{
			std::string guid = modsDatabaseReader.Get("Main", "ActiveMod" + std::to_string(i), "");
			std::string config = modsDatabaseReader.Get("Mods", guid, "");
			if (!config.empty() && Common::IsFileExist(config))
			{
				modIniList.push_back(config);
			}
		}
	}

	inline void ClampFloat(float& number, float min, float max)
	{
		if (number < min) number = min;
		if (number > max) number = max;
	}
	inline void ClampInt(int& number, int min, int max)
	{
		if (number < min) number = min;
		if (number > max) number = max;
	}

	inline bool IsModIdEnabled(std::string const& id, std::string* o_iniPath = nullptr)
	{
		std::vector<std::string> modIniList;
		GetModIniList(modIniList);

		for (size_t i = 0; i < modIniList.size(); i++)
		{
			std::string const& config = modIniList[i];
			INIReader configReader(config);
			std::string modId = configReader.Get("Main", "ID", "");

			if (modId == id)
			{
				if (o_iniPath)
				{
					*o_iniPath = config;
				}

				return true;
			}
		}

		return false;
	}

	inline bool IsModNameEnabled(std::string const& testModName, std::string* o_iniPath = nullptr)
	{
		std::vector<std::string> modIniList;
		GetModIniList(modIniList);
		for (size_t i = 0; i < modIniList.size(); i++)
		{
			std::string const& config = modIniList[i];
			INIReader configReader(config);
			std::string name = configReader.Get("Desc", "Title", "");
			if (name == testModName)
			{
				if (o_iniPath)
				{
					*o_iniPath = config;
				}

				return true;
			}
		}

		return false;
	}

	inline bool DoesArchiveExist(std::string const& archiveName)
	{
		std::vector<std::string> modFolderList;
		GetModIniList(modFolderList);
		for (std::string& folder : modFolderList)
		{
			folder = folder.substr(0, folder.length() - 7);
			for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(folder))
			{
				if (dirEntry.path().filename() == archiveName)
				{
					return true;
				}
			}
		}
		return false;
	}
}