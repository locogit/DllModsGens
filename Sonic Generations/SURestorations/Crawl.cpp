float crawlEnterTime = 0;
float crawlExitTime = 0;

float slopeDot;

float slideToCrawlSpeed = 7.0f;

float crawlSoundTime = -1;

Eigen::Vector3f currentCrawlVel;
Eigen::Vector3f targetCrawlVel;

HOOK(void, __stdcall, CrawlRotate, 0xE310A0, void* a1, float* targetDir, float turnRate1, float turnRateMultiplier, bool noLockDirection, float turnRate2)
{
	if (Crawl::isCrawling) {
		originalCrawlRotate(a1, targetDir, Crawl::crawlTurnSpeed * 0.55f, PI_F * Crawl::crawlTurnSpeed * 0.06f, true, Crawl::crawlTurnSpeed);
	}
	else {
		originalCrawlRotate(a1, targetDir, turnRate1, turnRateMultiplier, noLockDirection, turnRate2);
	}
}

bool CanEnterCrawl() {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();
	Eigen::Vector3f inputDirection;

	Common::GetWorldInputDirection(inputDirection);

	float slopeDot = sonic->m_UpVector.dot(sonic->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitZ());

	bool toSlide = slopeDot >= .25f &&
		!Common::GetSonicStateFlags()->OnStairs &&
		Crawl::crawlToSlide &&
		sonic->m_Velocity.norm() > slideToCrawlSpeed;

	return input.IsDown(Sonic::eKeyState_B) && !inputDirection.isZero() && !Crawl::isCrawling && crawlEnterTime <= 0 && sonic->m_Grounded && !toSlide && !Crawl::isCrawling;
}

HOOK(void, __fastcall, SquatAdvanceCrawl, 0x1230B60, void* This)
{
	originalSquatAdvanceCrawl(This);
	
	if (!CanEnterCrawl()) { return; }

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	crawlEnterTime = 0.3f;
	sonic->ChangeAnimation("CrawlEnter");
	sonic->ChangeState("Crawl");
}

class CrawlState : public Sonic::Player::CPlayerSpeedContext::CStateSpeedBase
{
	Sonic::Player::CPlayerSpeedContext* sonic;
	Sonic::SPadState input;
	Eigen::Vector3f inputDirection;
	Hedgehog::Base::CSharedString anim;
public:
	static constexpr const char* ms_StateName = "Crawl";

	Eigen::Vector3f GetForward()
	{
		auto context = GetContext();
		return (context->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitZ());
	}

	Eigen::Vector3f GetPosition() {
		return sonic->m_spMatrixNode->m_Transform.m_Position;
	}

	Eigen::Quaternionf GetRotation() {
		return sonic->m_spMatrixNode->m_Transform.m_Rotation;
	}

	bool ToSlide() {
		return slopeDot >= .25f && 
			!Common::GetSonicStateFlags()->OnStairs && 
			Crawl::crawlToSlide && 
			sonic->m_Velocity.norm() > slideToCrawlSpeed;
	}

	void EnterState() override
	{
		sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		crawlSoundTime = 0.0f;
		Crawl::isCrawling = true;
		sonic->StateFlag(eStateFlag_DisableGroundSmoke) = true;
	}

	void UpdateState() override
	{
		input = Sonic::CInputState::GetInstance()->GetPadState();
		Common::GetWorldInputDirection(inputDirection);

		anim = sonic->GetCurrentAnimationName();

		float moveMult = Ease::LinearInterpolate(0.35f, 1.0f, abs(inputDirection.norm()));

		slopeDot = sonic->m_UpVector.dot(GetRotation() * Eigen::Vector3f::UnitZ());

		Eigen::Vector3f currentVel = sonic->m_Velocity;
		targetCrawlVel = (GetRotation() * Eigen::Vector3f::UnitZ()).normalized() * Crawl::crawlSpeed * moveMult;
		Eigen::Vector3f velocityChange = (currentCrawlVel - currentVel);
		velocityChange = Eigen::ClampMagnitude(velocityChange, Crawl::crawlSpeed);

		sonic->m_HorizontalVelocity += velocityChange;
		sonic->m_Velocity = Eigen::ClampMagnitude(sonic->m_Velocity, Crawl::crawlSpeed);

		if (ToSlide()) {
			Crawl::isCrawling = false;
			sonic->m_HorizontalVelocity += sonic->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitZ() * 3.0f;
			sonic->ChangeState("Sliding");
			sonic->ChangeAnimation("Sliding");
			crawlSoundTime = -1;
			sonic->StateFlag(eStateFlag_DisableGroundSmoke) = false;
			return;
		}

		if (!inputDirection.isZero() && anim != "CrawlLoop" && crawlEnterTime <= 0) {
			sonic->ChangeAnimation("CrawlLoop");
			crawlSoundTime = 0.0f;
		}

		if (crawlExitTime <= 0 && anim == "CrawlExit") {
			sonic->ChangeAnimation("Squat");
		}

		if (input.IsDown(Sonic::eKeyState_B) && !inputDirection.isZero() && crawlEnterTime <= 0 && sonic->m_Grounded && anim == "Squat") {
			crawlEnterTime = 0.3f;
			sonic->ChangeAnimation("CrawlEnter");
			Crawl::isCrawling = true;
		}

		if (sonic->m_Grounded && !ToSlide() && (input.IsReleased(Sonic::eKeyState_B) || input.IsUp(Sonic::eKeyState_B))) {
			Crawl::isCrawling = false;
			if (Math::ApproximatelyEqualTo(sonic->m_Velocity.norm(), 0.0f, 0.05f)) {
				sonic->ChangeState("Squat");
			}
			else {
				sonic->SetHorizontalVelocity(Hedgehog::Math::CVector::Zero());
				sonic->ChangeState("Stand");
			}
		}

		if (inputDirection.isZero() && sonic->m_Grounded && !ToSlide() && (anim == "CrawlLoop" || anim == "CrawlEnter")) {
			crawlExitTime = 0.3f;
			sonic->ChangeAnimation("CrawlExit");
			crawlSoundTime = -1;
			sonic->StateFlag(eStateFlag_DisableGroundSmoke) = false;
		}

		if (input.IsTapped(Sonic::eKeyState_A) && sonic->m_Grounded && Crawl::isCrawling) {
			sonic->ChangeState("Sliding");
			Crawl::isCrawling = false;
			sonic->StateFlag(eStateFlag_DisableGroundSmoke) = false;
		}
		else if (!sonic->m_Grounded) {
			sonic->ChangeState("Fall");
			Crawl::isCrawling = false;
			sonic->StateFlag(eStateFlag_DisableGroundSmoke) = false;
		}
	}

};

void AddCrawlState(Sonic::Player::CPlayerSpeedContext* context) // NextinHKRY
{
	static bool added = false;
	if (added) return;

	if (!added)
	{
		auto state = (Sonic::Player::CPlayerSpeedContext::CStateSpeedBase*)0x016D7648;
		context->m_pPlayer->m_StateMachine.RegisterStateFactory<CrawlState>();
		added = true;
	}
}

HOOK(void*, __fastcall, _InitializePlayer, 0x00D96110, void* This)
{
	void* result = original_InitializePlayer(This);
	auto context = Sonic::Player::CPlayerSpeedContext::GetInstance();    // Hack: there's a better way to do this but whatever. This writes to the singleton anyway.

	AddCrawlState(context);
	return result;
}

// Brianuuu 06 Experience
HOOK(int, __fastcall, SlideStart, 0x11D7110, void* This) {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

	Eigen::Vector3f inputDirection;
	Common::GetWorldInputDirection(inputDirection);

	if (sonic->m_HorizontalVelocity.norm() <= slideToCrawlSpeed * 1.5f && !Crawl::isCrawling && input.IsDown(Sonic::eKeyState_B) && !inputDirection.isZero() && slopeDot < .15f) {
		sonic->ChangeState("Crawl");
		sonic->ChangeAnimation("CrawlLoop");
		// Disable sliding sfx and voice
		WRITE_MEMORY(0x11D722C, int, -1);
		WRITE_MEMORY(0x11D72DC, int, -1);
		return 0;
	}
	else {
		// Sliding sfx and voice
		WRITE_MEMORY(0x11D722C, uint32_t, 2002032);
		WRITE_MEMORY(0x11D72DC, uint32_t, 3002016);
		return originalSlideStart(This);
	}
}

HOOK(void, __fastcall, SlideAdvance, 0x11D69A0, hh::fnd::CStateMachineBase::CStateBase* This) {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

	Eigen::Vector3f inputDirection;
	Common::GetWorldInputDirection(inputDirection);

	if (sonic->m_HorizontalVelocity.norm() <= Crawl::crawlSpeed * 1.25f && !Crawl::isCrawling && input.IsDown(Sonic::eKeyState_B) && !inputDirection.isZero() && slopeDot < .15f) {
		sonic->ChangeState("Crawl");
		sonic->ChangeAnimation("CrawlLoop");
	}
	else {
		return originalSlideAdvance(This);
	}
}

void CrawlSound(const hh::fnd::SUpdateInfo& updateInfo, Sonic::Player::CPlayerSpeedContext* sonic) {
	if (!Crawl::isCrawling || crawlSoundTime == -1) { return; }
	crawlSoundTime -= updateInfo.DeltaTime;
	if (crawlSoundTime <= 0) {
		sonic->PlaySound(2002505, true);
		crawlSoundTime = 0.225f;
	}
}

Eigen::Vector3f crawlVelRef1;
Eigen::Vector3f crawlVelRef2;

void ResetCrawlVel() {
	crawlVelRef1 = Eigen::Vector3f::Zero();
	crawlVelRef2 = Eigen::Vector3f::Zero();
	currentCrawlVel = Eigen::Vector3f::Zero();
	targetCrawlVel = Eigen::Vector3f::Zero();
}

void Crawl::OnUpdate(const hh::fnd::SUpdateInfo& updateInfo) {
	if (!BlueBlurCommon::IsModern()) { return; }
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Hedgehog::Base::CSharedString state = sonic->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();

	CrawlSound(updateInfo, sonic);

	if (crawlEnterTime > 0) { crawlEnterTime -= updateInfo.DeltaTime; }
	if (crawlExitTime > 0) { crawlExitTime -= updateInfo.DeltaTime; }

	if (state != "Squat" && state != "Crawl") {
		ResetCrawlVel();
	}

	if (Crawl::isCrawling) {
		currentCrawlVel = Eigen::SmoothDamp(currentCrawlVel, targetCrawlVel, crawlVelRef1, 0.075f * Crawl::crawlSlipMult, INFINITY, updateInfo.DeltaTime);
	}
	else {
		currentCrawlVel = Eigen::SmoothDamp(currentCrawlVel, Eigen::Vector3f::Zero(), crawlVelRef2, 0.1f * Crawl::crawlSlipMult, INFINITY, updateInfo.DeltaTime);
	}
}

HOOK(void, __fastcall, QuickStepStart, 0x123C2A0, int This) {
	Crawl::isCrawling = false;
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	sonic->StateFlag(eStateFlag_DisableGroundSmoke) = false;
	originalQuickStepStart(This);
}

HOOK(int, __fastcall, ProcMsgRestartStageCrawl, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	if (!BlueBlurCommon::IsModern()) { return originalProcMsgRestartStageCrawl(This, Edx, message); }
	ResetCrawlVel();
	return originalProcMsgRestartStageCrawl(This, Edx, message);
}

void Crawl::Install() {
	Crawl::crawlTurnSpeed = 125;
	Crawl::crawlSpeed = 7.35f;
	Crawl::crawlToSlide = true;
	Crawl::crawlSlipMult = 1.0f;

	// Brianuuu 06 Experience
	// Don't allow stick move start sliding from squat
	WRITE_MEMORY(0x1230D62, uint8_t, 0xEB);
	WRITE_MEMORY(0x1230DA9, uint8_t, 0xE9, 0xA8, 0x00, 0x00, 0x00, 0x90);
	// Do not use sliding fail pfx
	WRITE_NOP(0x11D6A6D, 2);

	INSTALL_HOOK(SlideStart);
	INSTALL_HOOK(SlideAdvance);
	//INSTALL_HOOK(CrawlRotate);

	//INSTALL_HOOK(QuickStepStart);
	INSTALL_HOOK(SquatAdvanceCrawl);
	INSTALL_HOOK(ProcMsgRestartStageCrawl);
	INSTALL_HOOK(_InitializePlayer);
}