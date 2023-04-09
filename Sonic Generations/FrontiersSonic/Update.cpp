boost::shared_ptr<Sonic::CGameObject> m_kocoSingleton;
static SharedPtrTypeless soundHandle;

class CObjKoco : public Sonic::CGameObject
{
	boost::shared_ptr<hh::mr::CSingleElement> m_spModel;
	Sonic::Player::CPlayerSpeedContext* context;
	Eigen::Vector3f scale;
	Eigen::Vector3f desiredScale;
	float scaleSpeed;
public:
	CObjKoco()
	{
	}
	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& worldHolder, Sonic::CGameDocument* pGameDocument,
		const boost::shared_ptr<Hedgehog::Database::CDatabase>& spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		pGameDocument->AddUpdateUnit("f", this);

		m_spModel = boost::make_shared<hh::mr::CSingleElement>(hh::mr::CMirageDatabaseWrapper(spDatabase.get()).GetModelData("dlcslr_obj_kocotoy"));
		AddRenderable("Object", m_spModel, false);

		context = Sonic::Player::CPlayerSpeedContext::GetInstance();

		scale = Eigen::Vector3f::Zero();
		desiredScale = Eigen::Vector3f::Zero();
		scaleSpeed = 5.0f;
	}

	void UpdateParallel(const Hedgehog::Universe::SUpdateInfo& updateInfo) override
	{
		UpdateTransform(updateInfo);
	}

	bool ProcessMessage(Hedgehog::Universe::Message& message, bool flag) override
	{
		if (flag)
		{
			if (std::strstr(message.GetType(), "MsgRestartStage") != nullptr
				|| std::strstr(message.GetType(), "MsgStageClear") != nullptr)
			{
				Kill();
				return true;
			}
		}

		return Sonic::CGameObject::ProcessMessage(message, flag);
	}

	void Kill()
	{
		soundHandle.reset();
		m_kocoSingleton = nullptr;
		SendMessage(m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
	}

	void UpdateTransform(const Hedgehog::Universe::SUpdateInfo& updateInfo) {

		Hedgehog::Base::CSharedString state = context->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
		Hedgehog::Base::CSharedString anim = context->GetCurrentAnimationName();

		const auto GetAnimInfo = boost::make_shared<Sonic::Message::MsgGetAnimationInfo>();
		context->m_pPlayer->SendMessageImm(context->m_pPlayer->m_ActorID, GetAnimInfo);

		auto HandleNode = context->m_pPlayer->m_spCharacterModel->GetNode("Free01");

		Eigen::Affine3f affineHandleNode;

		affineHandleNode = HandleNode->m_WorldMatrix;

		hh::math::CVector HandleVector;
		HandleVector = affineHandleNode.translation();

		hh::math::CQuaternion HandleQuaternion = hh::math::CQuaternion(affineHandleNode.rotation());

		auto matrix = (Eigen::Translation3f(HandleVector) * HandleQuaternion).matrix();

		// intro
		if (GetAnimInfo->m_Frame >= 24 && GetAnimInfo->m_Frame <= 25) {
			desiredScale = Eigen::Vector3f(1, 1, 1);
			scale = desiredScale;
		}

		float squish1[2] = { 115.5f, 135 };
		float squish2[2] = { 160.5f, 182 };

		float squishSpeedIn = 8.5f;
		float squishSpeedOut = 7.5f;

		Eigen::Vector3f squishScale = Eigen::Vector3f(0.8f, 1, 1);

		// first squish
		if (GetAnimInfo->m_Frame >= squish1[0] && GetAnimInfo->m_Frame <= squish1[0]+1) {
			scaleSpeed = squishSpeedIn;
			desiredScale = squishScale;
		}

		if (GetAnimInfo->m_Frame >= squish1[1] && GetAnimInfo->m_Frame <= squish1[1] + 1) {
			scaleSpeed = squishSpeedOut;
			desiredScale = Eigen::Vector3f(1, 1, 1);
		}

		// second squish
		if (GetAnimInfo->m_Frame >= squish2[0] && GetAnimInfo->m_Frame <= squish2[0] + 1) {
			scaleSpeed = squishSpeedIn;
			desiredScale = squishScale;
		}

		if (GetAnimInfo->m_Frame >= squish2[1] && GetAnimInfo->m_Frame <= squish2[1] + 1) {
			scaleSpeed = squishSpeedOut;
			desiredScale = Eigen::Vector3f(1, 1, 1);
		}

		// outro
		if (GetAnimInfo->m_Frame >= 1130 && GetAnimInfo->m_Frame <= 1131) {
			desiredScale = Eigen::Vector3f::Zero();
			scale = desiredScale;
		}

		scale.x() = Common::Lerp(scale.x(), desiredScale.x(), updateInfo.DeltaTime * scaleSpeed);
		scale.y() = Common::Lerp(scale.y(), desiredScale.y(), updateInfo.DeltaTime * scaleSpeed);
		scale.z() = Common::Lerp(scale.z(), desiredScale.z(), updateInfo.DeltaTime * scaleSpeed);

		matrix.col(0) *= scale.x();
		matrix.col(1) *= scale.y();
		matrix.col(2) *= scale.z();

		if (anim != "IdleKoco" && state != "Stand" && m_kocoSingleton) {
			Kill();
		}

		if (GetAnimInfo->m_Frame >= 1194) {
			context->ChangeAnimation("Stand");
			Kill();
		}

		m_spModel->m_spInstanceInfo->m_Transform = matrix;
	}
};

FUNCTION_PTR(bool*, __thiscall, CSingleElementChangeMaterial, 0x701CC0, Hedgehog::Mirage::CSingleElement* singleElement, hh::mr::CMaterialData* from, boost::shared_ptr<hh::mr::CMaterialData>& to);
FUNCTION_PTR(bool*, __thiscall, CSingleElementResetMaterial, 0x701830, Hedgehog::Mirage::CSingleElement* singleElement, hh::mr::CMaterialData* mat);

void StartIdle() {
	//Common::PlaySoundStatic(soundHandle, 819001);
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	sonic->ChangeAnimation("IdleKoco");
	if (m_kocoSingleton) {
		((CObjKoco*)m_kocoSingleton.get())->Kill();
	}
	m_kocoSingleton = boost::make_shared<CObjKoco>();
	sonic->m_pPlayer->m_pMember->m_pGameDocument->AddGameObject(m_kocoSingleton);
}

bool GetDoubleJumpFlag()
{
	return CONTEXT->StateFlag(eStateFlag_DoubleJumping);
}

void SetDoubleJumpFlag(bool value)
{
	CONTEXT->StateFlag(eStateFlag_DoubleJumping) = value;
}

void DoubleJumpUpdate()
{
	auto input = Sonic::CInputState::GetInstance()->GetPadState();

	if (input.IsTapped(Sonic::eKeyState_A))
	{
		if (GetDoubleJumpFlag())
		{

		}
		else
		{
			CONTEXT->PlaySound(2002027, 1);

			CONTEXT->ChangeState("Jump");

			CONTEXT->m_Velocity.y() = ((CONTEXT->GetVelocity().y() - CONTEXT->m_PreviousVelocity.y()) + CONTEXT->StateFlag(eStateFlag_OnWater)
				? 10.0f
				: 15.0f);

			// Don't set double jump flag underwater.
			if (!CONTEXT->StateFlag(eStateFlag_OnWater))
				SetDoubleJumpFlag(true);
		}
	}
}

bool magnetDash = false;
float magnetDashTimer = 0.0f;

void MapBoost(bool enable) {
	int map = enable ? 32 : -1;
	// https://github.com/brianuuu/DllMods/blob/master/Source/Sonic06DefinitiveExperience/NextGenPhysics.cpp
	WRITE_MEMORY(0xDFDF4C, uint32_t, map); // Air Boost
	WRITE_MEMORY(0xDFF25B, uint32_t, map); // Grind Boost
	WRITE_MEMORY(0xE3D991, uint32_t, map); // Blast Off
	WRITE_MEMORY(0xE4776B, uint32_t, map); // Dummy Boost
	WRITE_MEMORY(0x11177EE, uint32_t, map); // Boost
	WRITE_MEMORY(0x1118CEE, uint32_t, map); // Fall Boost
	WRITE_MEMORY(0x111BE61, uint32_t, map); // Null Boost?
	WRITE_MEMORY(0x111BEE8, uint32_t, map); // Dummy Boost plugin
	WRITE_MEMORY(0x111D801, uint32_t, map); // Board Fall Boost
	WRITE_MEMORY(0x11A0716, uint32_t, map); // Dummy Boost External
	WRITE_MEMORY(0x11A07D7, uint32_t, map); // Speed external control
	WRITE_MEMORY(0x11A0DA8, uint32_t, map); // Speed external control
	WRITE_MEMORY(0x11BD057, uint32_t, map); // DivingDive start
	WRITE_MEMORY(0x124AF01, uint32_t, map); // DivingDive end
}

int bounceCount = 0;
float bounceDeltaTime = 0.0f;

bool hasStompBounced = false;

int stompBounceCount = 2;

void StompBounceUpdate(Sonic::Player::CPlayerSpeedContext* context, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto currentAnim = context->GetCurrentAnimationName().c_str();
	auto input = Sonic::CInputState::GetInstance()->GetPadState();

	if (input.IsDown(Sonic::eKeyState_B))
	{
		if (StringHelper::Compare(currentAnim, "StompingSquat"))
		{
			if (!hasStompBounced)
			{
				if (stompBounceCount != -1 && bounceCount >= stompBounceCount)
					return;

				bounceDeltaTime += updateInfo.DeltaTime;
				{
					if (bounceDeltaTime > .08f)
					{
						hasStompBounced = true;

						bounceDeltaTime = 0.0f;

						context->ChangeState
						(
							context->StateFlag(eStateFlag_OnWater)
							? "HomingAttackAfter"
							: "Jump"
						);

						if (context->StateFlag(eStateFlag_OnWater))
						{
							context->ChangeAnimation("SpinAttack");

						}

						context->m_Velocity.y() = 
						(
							context->GetVelocity().y() + context->StateFlag(eStateFlag_OnWater)
							? 10.0f
							: 20.0f
						);

						bounceCount++;
					}
				}
			}
		}
	}
	else
	{
		hasStompBounced = true;

		bounceCount = 0;
	}
}

void StompBounceEnd(Sonic::Player::CPlayerSpeedContext* context, const hh::fnd::SUpdateInfo& updateInfo)
{
	hasStompBounced = false;
}

Sonic::CCamera::CMyCamera camera;
bool isBoostToFall = false;
bool isAirBoostToFall = false;

HOOK(void, __fastcall, SonicUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicUpdate(This, _, updateInfo);

	if (BlueBlurCommon::IsClassic() || BlueBlurCommon::IsBossSuper()) { return; }

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Hedgehog::Base::CSharedString state = sonic->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();

	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

	if (!sonic->StateFlag(eStateFlag_Boost)) {
		sonic->m_ChaosEnergy = std::clamp(sonic->m_ChaosEnergy + updateInfo.DeltaTime * 2.0f, 0.0f, sonic->GetMaxChaosEnergy());
	}

	if (state == "Stomping" || state == "Jump" || state == "Sliding" || state == "HomingAttack") {
		Common::SonicContextSetCollision(SonicCollision::TypeSonicStomping, true);
	}
	else {
		Common::SonicContextSetCollision(SonicCollision::TypeSonicStomping, false);
	}

	MapBoost(!(state == "Stand" || state == "MoveStop"));

	if (magnetDash) {
		magnetDashTimer += updateInfo.DeltaTime;
		if (input.IsTapped(Sonic::eKeyState_RightTrigger)) {
			if (state == "HomingAttack") {
				sonic->ChangeState("AirBoost");
				Eigen::Vector3f velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitZ() * sonic->m_Velocity.norm() * 8.0f;
				velocity.y() = sonic->m_Velocity.norm() * updateInfo.DeltaTime * 5.0f;

				alignas(16) MsgApplyImpulse message {};
				message.m_position = sonic->m_spMatrixNode->m_Transform.m_Position;
				message.m_impulse = velocity;
				message.m_impulseType = ImpulseType::None;
				message.m_outOfControl = 0.0f;
				message.m_keepVelocityTime = 0.2f;
				message.m_notRelative = true;
				message.m_snapPosition = false;
				message.m_pathInterpolate = false;
				message.m_alwaysMinusOne = -1.0f;

				Common::ApplyPlayerApplyImpulse(message);

				magnetDash = false;
			}
		}
		if (magnetDashTimer >= 0.25f) {
			magnetDash = false;
		}
	}

	if (input.IsTapped(Sonic::eKeyState_LeftTrigger) && state == "Stand") {
		StartIdle();
	}

	if ((anim == "IdleSleepStart" || anim == "IdleSleepStartSV") && state == "Stand") {
		StartIdle();
	}

	if (input.IsTapped(Sonic::eKeyState_LeftBumper) && (state == "Jump" || state == "ShortJump" || state == "JumpHurdle" || state == "Fall" || state == "HomingAttackAfter") && !sonic->m_Is2DMode) {
		sonic->ChangeState("Fall");
		sonic->ChangeAnimation("AirDodgeL");
		sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitX() * 20.0f;
	}
	if (input.IsTapped(Sonic::eKeyState_RightBumper) && (state == "Jump" || state == "ShortJump" || state == "JumpHurdle" || state == "Fall" || state == "HomingAttackAfter") && !sonic->m_Is2DMode) {
		sonic->ChangeState("Fall");
		sonic->ChangeAnimation("AirDodgeR");
		sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitX() * -20.0f;
	}

	if (sonic->StateFlag(eStateFlag_Boost)) {
		Common::SonicContextSetCollision(SonicCollision::TypeSonicBoost, false);
	}

	if (sonic->m_Grounded) {
		SetDoubleJumpFlag(false);
		StompBounceUpdate(sonic, updateInfo);
		FUNCTION_PTR(char, __fastcall, HomingUpdate, 0xDFFE30, Sonic::Player::CPlayerSpeedContext * _this, void* _, int a2);
		{
			HomingUpdate(sonic, (void*)0xDFFE30, -1);
		}
	}
	else {
		StompBounceEnd(sonic, updateInfo);
	}
}

HOOK(int, __fastcall, CSonicStateJumpBallUpdate, 0x11BCB00, float* _this)
{
	DoubleJumpUpdate();

	return originalCSonicStateJumpBallUpdate(_this);
}

HOOK(void, __fastcall, CStateJumpShortStart, 0x1255820, hh::fnd::CStateMachineBase::CStateBase* _this)
{
	DoubleJumpUpdate();

	originalCStateJumpShortStart(_this);
}

HOOK(int, __fastcall, CSonicStateFallStart, 0x1118FB0, hh::fnd::CStateMachineBase::CStateBase* _this)
{
	auto context = (Sonic::Player::CPlayerSpeedContext*)_this->GetContextBase();

	isBoostToFall = context->StateFlag(eStateFlag_Boost) && !isAirBoostToFall;

	if (isAirBoostToFall || isBoostToFall || (strstr(context->GetCurrentAnimationName().c_str(),"AirDodge")))
	{
		// Don't transition animation.
		WRITE_MEMORY(0x1118DE5, uint8_t, 0xEB);
		WRITE_MEMORY(0x1118E94, uint8_t, 0xEB);
		WRITE_MEMORY(0x111910F, uint8_t, 0xEB);

		if (isBoostToFall)
			context->ChangeAnimation("AirBoost");
	}
	else
	{
		WRITE_MEMORY(0x1118DE5, uint8_t, 0x75);
		WRITE_MEMORY(0x1118E94, uint8_t, 0x76);
		WRITE_MEMORY(0x111910F, uint8_t, 0x75);
	}

	return originalCSonicStateFallStart(_this);
}

HOOK(void, __fastcall, CSonicStateFallUpdate, 0x1118C50, hh::fnd::CStateMachineBase::CStateBase* _this)
{
	auto context = (Sonic::Player::CPlayerSpeedContext*)_this->GetContextBase();

	DoubleJumpUpdate();

	if (isBoostToFall && !context->StateFlag(eStateFlag_Boost))
	{
		isBoostToFall = false;

		context->ChangeAnimation("Fall");

		// Allow animation to change to Fall and FallLarge.
		WRITE_MEMORY(0x1118DE5, uint8_t, 0x75);
		WRITE_MEMORY(0x1118E94, uint8_t, 0x76);
	}

	WRITE_MEMORY(0x1118DE5, uint8_t, 0xEB);
	WRITE_MEMORY(0x1118E94, uint8_t, 0xEB);
	WRITE_MEMORY(0x111910F, uint8_t, 0xEB);

	originalCSonicStateFallUpdate(_this);
}

HOOK(int, __fastcall, CSonicStateFallEnd, 0x1118F20, hh::fnd::CStateMachineBase::CStateBase* _this)
{
	isAirBoostToFall = false;
	isBoostToFall = false;

	return originalCSonicStateFallEnd(_this);
}

void __declspec(naked) AirBoostToFallFlag()
{
	static void* returnAddress = (void*)0x1233330;

	static const char* animName = "Fall";

	__asm
	{
		mov isAirBoostToFall, 1

		push[animName]

		jmp[returnAddress]
	}
}

HOOK(void, __fastcall, CustomCamera_CCameraUpdateParallel, 0x10FB770, Sonic::CCamera* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo) {
	originalCustomCamera_CCameraUpdateParallel(This, Edx, in_rUpdateInfo);
	camera = This->m_MyCamera;
}

void __declspec(naked) CObjPlaTramCarBoostButtonChange()
{
	static uint32_t successAddress = 0xF368C4;
	static uint32_t returnAddress = 0xF3691D;
	__asm
	{
		test    word ptr[esi], 0x8000
		jz      jump
		jmp[successAddress]

		jump:
		jmp[returnAddress]
	}
}
bool IsBlacklistedAnimation(std::vector<const char*> animations, const char* animName)
{
	for (auto name : animations)
	{
		if (StringHelper::ContainsSubstring(animName, name))
			return true;
	}

	return false;
}

bool IsGrinding(const char* animName)
{
	std::vector<const char*> animations =
	{
		"Grind"
	};

	return IsBlacklistedAnimation(animations, animName);
}

HOOK(char, __fastcall, CSonicContextHomingUpdate, 0xDFFE30, int* _this, void* _, int a2)
{
	auto input = Sonic::CInputState::GetInstance()->GetPadState();

	if (input.IsTapped(Sonic::eKeyState_A))
		return 0;

	bool canHomingAttack = _this[934] != 0 &&
		!CONTEXT->StateFlag(eStateFlag_InvokeSkateBoard) &&
		!CONTEXT->StateFlag(eStateFlag_OutOfControl) &&
		!IsGrinding(CONTEXT->GetCurrentAnimationName().c_str());

	if
		(
			canHomingAttack &&

			input.IsTapped
			(
				Sonic::eKeyState_X
			)
			)
	{
		if (CONTEXT->m_Grounded)
		{
			CONTEXT->ChangeState
			(
				CONTEXT->StateFlag(eStateFlag_OnWater)
				? "HomingAttackAfter"
				: "Jump"
			);
		}

		CONTEXT->ChangeState("HomingAttack");
		magnetDash = true;
		magnetDashTimer = 0.0f;
		return 1;
	}

	return originalCSonicContextHomingUpdate(_this, _, a2);
}

void Update::Install() {
	INSTALL_HOOK(SonicUpdate);
	INSTALL_HOOK(CustomCamera_CCameraUpdateParallel);

	INSTALL_HOOK(CSonicStateJumpBallUpdate);
	INSTALL_HOOK(CStateJumpShortStart);

	INSTALL_HOOK(CSonicStateFallStart);
	INSTALL_HOOK(CSonicStateFallUpdate);
	INSTALL_HOOK(CSonicStateFallEnd);

	// PlaTramCar
	WRITE_JUMP(0xF368BF, CObjPlaTramCarBoostButtonChange); // PlaTramCar

	// https://github.com/HyperBE32/App-Extension-Mods/blob/main/Sonic%20Generations/FrontiersSonic/Player.cpp
	// Unmap drift.
	WRITE_MEMORY(0xDF2DFF, uint32_t, -1);
	WRITE_MEMORY(0xDFF62B, uint32_t, -1);
	WRITE_MEMORY(0xDFF816, uint32_t, -1);
	WRITE_MEMORY(0x1119549, uint32_t, -1);
	WRITE_MEMORY(0x119910D, uint32_t, -1);

	WRITE_MEMORY(0xDFFD5F, uint32_t, 31); // LightSpeedDashReady
	WRITE_MEMORY(0x111761A, uint32_t, 31); // LightSpeedDash

	// Restore XButtonHoming string to disable the HMM code.
	WRITE_STRING(0x15FA418, "XButtonHoming");

	// Loop dash ring animations.
	WRITE_MEMORY(0x1276D20, uint8_t, 0x1D);
	WRITE_MEMORY(0x1276D87, uint8_t, 0x1D);

	// Replace animations with the jump ball.
	WRITE_MEMORY(0x12733A2, const char*, "sn_ball_loop"); // JumpShortBegin
	WRITE_MEMORY(0x1273409, const char*, "sn_ball_loop"); // JumpShort
	WRITE_MEMORY(0x1273470, const char*, "sn_ball_loop"); // JumpShortTop
	WRITE_MEMORY(0x127380A, const char*, "sn_ball_loop"); // JumpHurdleL
	WRITE_MEMORY(0x1273876, const char*, "sn_ball_loop"); // JumpHurdleR
	WRITE_MEMORY(0x1273A12, const char*, "sn_ball_loop"); // SquatToJump

	// Disable boost particles.
	WRITE_MEMORY(0x15A3568, uint8_t, 0x00); // ef_bo_sha_yh1_boost1
	WRITE_MEMORY(0x15E9048, uint8_t, 0x00); // ef_ch_sng_yh1_boost1
	WRITE_MEMORY(0x15E9060, uint8_t, 0x00); // ef_ch_sng_yh1_boost2
	WRITE_MEMORY(0x15EE774, uint8_t, 0x00); // ef_bo_sha_yh1_hyper_sn
	WRITE_MEMORY(0x15EE78C, uint8_t, 0x00); // ef_bo_sha_yh1_hyper_sn
	WRITE_MEMORY(0x15F99F8, uint8_t, 0x00); // ef_ch_sps_yh1_boost1
	WRITE_MEMORY(0x15F9A10, uint8_t, 0x00); // ef_ch_sps_yh1_boost2
	WRITE_MEMORY(0x164330C, uint8_t, 0x00); // ef_st_ssh_yh1_bobsled_boost

	// Disable spin dash on dash panels.
	WRITE_MEMORY(0xE0AC1C, uint8_t, 0xE9, 0x27, 0x01, 0x00, 0x00);
	WRITE_MEMORY(0xE0C734, uint8_t, 0xE9, 0x27, 0x01, 0x00, 0x00);

	WRITE_JUMP(0x123332B, AirBoostToFallFlag);

	INSTALL_HOOK(CSonicContextHomingUpdate);

}