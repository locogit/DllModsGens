bool isCrawling = false;
float crawlEnterTime = 0;
float crawlExitTime = 0;

float slopeDot;

float slideToCrawlSpeed = 7.0f;

float crawlSoundTime = -1;

Eigen::Vector3f currentCrawlVel;
Eigen::Vector3f targetCrawlVel;

HOOK(void, __stdcall, CrawlRotate, 0xE310A0, void* a1, float* targetDir, float turnRate1, float turnRateMultiplier, bool noLockDirection, float turnRate2)
{
	originalCrawlRotate(a1, targetDir, turnRate1, turnRateMultiplier, noLockDirection, turnRate2);
}

HOOK(void, __fastcall, SquatAdvanceCrawl, 0x1230B60, void* This)
{
	originalSquatAdvanceCrawl(This);

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	if (sonic->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName() == "QuickStep") { return; }

	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();
	Eigen::Vector3f inputDirection;
	Common::GetWorldInputDirection(inputDirection);

	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
	Eigen::Vector3f playerPosition = sonic->m_spMatrixNode->m_Transform.m_Position;
	Eigen::Quaternionf playerRotation = sonic->m_spMatrixNode->m_Transform.m_Rotation;

	float moveMult = Ease::LinearInterpolate(0.35f, 1.0f, abs(inputDirection.norm()));

	slopeDot = sonic->m_UpVector.dot(sonic->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitZ());
	bool toSlide = slopeDot >= .25f && !Common::GetSonicStateFlags()->OnStairs && Crawl::crawlToSlide && sonic->m_Velocity.norm() > slideToCrawlSpeed;

	Eigen::Vector3f currentVel = sonic->m_Velocity;
	targetCrawlVel = (playerRotation * Eigen::Vector3f::UnitZ()).normalized() * Crawl::crawlSpeed * moveMult;
	Eigen::Vector3f velocityChange = (currentCrawlVel - currentVel);
	velocityChange = Eigen::ClampMagnitude(velocityChange, Crawl::crawlSpeed);

	sonic->m_HorizontalVelocity += velocityChange;

	sonic->StateFlag(eStateFlag_DisableGroundSmoke) = isCrawling;

	alignas(16) float dir[4] = { inputDirection.x(), inputDirection.y(), inputDirection.z(), 0 };

	if (toSlide && isCrawling) {
		isCrawling = false;
		sonic->m_HorizontalVelocity += sonic->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitZ() * 3.0f;
		sonic->ChangeState("Sliding");
		sonic->ChangeAnimation("Sliding");
		crawlSoundTime = -1;
		return;
	}

	if (!inputDirection.isZero() && isCrawling) {
		if (anim != "CrawlLoop" && crawlEnterTime <= 0)
		{
			sonic->ChangeAnimation("CrawlLoop");
			crawlSoundTime = 0.0f;
		}
	}

	if (!sonic->m_Is2DMode && isCrawling) {
		originalCrawlRotate(This, dir, Crawl::crawlTurnSpeed * 0.55f, PI_F * Crawl::crawlTurnSpeed * 0.06f, true, Crawl::crawlTurnSpeed * moveMult);
	}
	else if (sonic->m_Is2DMode && isCrawling) {
		originalCrawlRotate(This, dir, 400, 400, true, 100);
	}

	if (!isCrawling) {
		crawlSoundTime = -1;
		sonic->m_Velocity += velocityChange;
	}

	if (crawlExitTime <= 0 && anim == "CrawlExit" && !isCrawling) {
		sonic->ChangeAnimation("Squat");
	}

	if (input.IsDown(Sonic::eKeyState_B) && !inputDirection.isZero() && !isCrawling && crawlEnterTime <= 0 && sonic->m_Grounded && !toSlide && !isCrawling && anim == "Squat") {
		crawlEnterTime = 0.3f;
		sonic->ChangeAnimation("CrawlEnter");
		isCrawling = true;
	}

	if (sonic->m_Grounded && !toSlide && isCrawling && (input.IsReleased(Sonic::eKeyState_B) || input.IsUp(Sonic::eKeyState_B))) {
		isCrawling = false;
		sonic->SetHorizontalVelocity(Hedgehog::Math::CVector::Zero());
		sonic->ChangeAnimation("SquatToStand");
	}

	if (inputDirection.isZero() && sonic->m_Grounded && !toSlide && isCrawling) {
		isCrawling = false;
		if (anim == "CrawlLoop" || anim == "CrawlEnter") {
			crawlExitTime = 0.3f;
			sonic->ChangeAnimation("CrawlExit");
			crawlSoundTime = -1;
		}
	}

	if (input.IsTapped(Sonic::eKeyState_A) && sonic->m_Grounded && !toSlide && isCrawling) {
		isCrawling = false;
	}

	if (!sonic->m_Grounded && isCrawling) {
		sonic->ChangeAnimation("Fall");
		isCrawling = false;
	}
}
// Brianuuu 06 Experience
HOOK(int, __fastcall, SlideStart, 0x11D7110, void* This) {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

	Eigen::Vector3f inputDirection;
	Common::GetWorldInputDirection(inputDirection);

	if (sonic->m_HorizontalVelocity.norm() <= slideToCrawlSpeed * 1.5f && !isCrawling && input.IsDown(Sonic::eKeyState_B) && !inputDirection.isZero() && slopeDot < .15f) {
		sonic->ChangeState("Squat");
		sonic->ChangeAnimation("CrawlLoop");
		crawlSoundTime = 0.0f;
		isCrawling = true;
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

	if (sonic->m_HorizontalVelocity.norm() <= Crawl::crawlSpeed * 1.25f && !isCrawling && input.IsDown(Sonic::eKeyState_B) && !inputDirection.isZero() && slopeDot < .15f) {
		sonic->ChangeState("Squat");
		sonic->ChangeAnimation("CrawlLoop");
		crawlSoundTime = 0.0f;
		isCrawling = true;
	}
	else {
		return originalSlideAdvance(This);
	}
}

void CrawlSound(const hh::fnd::SUpdateInfo& updateInfo, Sonic::Player::CPlayerSpeedContext* sonic) {
	if (!isCrawling) { return; }
	if (crawlSoundTime == -1) { return; }
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
	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

	Eigen::Vector3f inputDirection;
	Common::GetWorldInputDirection(inputDirection);

	CrawlSound(updateInfo, sonic);

	if (crawlEnterTime > 0) { crawlEnterTime -= updateInfo.DeltaTime; }
	if (crawlExitTime > 0) { crawlExitTime -= updateInfo.DeltaTime; }

	if (state != "Squat") {
		ResetCrawlVel();
	}

	if (isCrawling) {
		currentCrawlVel = Eigen::SmoothDamp(currentCrawlVel, targetCrawlVel, crawlVelRef1, 0.075f * Crawl::crawlSlipMult, INFINITY, updateInfo.DeltaTime);
	}
	else {
		currentCrawlVel = Eigen::SmoothDamp(currentCrawlVel, Eigen::Vector3f::Zero(), crawlVelRef2, 0.1f * Crawl::crawlSlipMult, INFINITY, updateInfo.DeltaTime);
	}
}

HOOK(void, __fastcall, QuickStepStart, 0x123C2A0, int This) {
	isCrawling = false;
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

	INSTALL_HOOK(SquatAdvanceCrawl);
	INSTALL_HOOK(QuickStepStart);
	INSTALL_HOOK(ProcMsgRestartStageCrawl);
}