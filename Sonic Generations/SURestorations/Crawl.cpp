bool isCrawling = false;
const float crawlSpeed = 6.75f;
float crawlEnterTime = 0;
float crawlExitTime = 0;

float crawlTurnRate = 175;
float slopeDot;

HOOK(void, __stdcall, CrawlRotate, 0xE310A0, void* a1, float* targetDir, float turnRate1, float turnRateMultiplier, bool noLockDirection, float turnRate2)
{
	originalCrawlRotate(a1, targetDir, turnRate1, turnRateMultiplier, noLockDirection, turnRate2);
}

HOOK(void, __fastcall, SquatAdvanceCrawl, 0x1230B60, void* This)
{
	originalSquatAdvanceCrawl(This);

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	slopeDot = sonic->m_UpVector.dot(sonic->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitZ());

	sonic->StateFlag(eStateFlag_DisableGroundSmoke) = isCrawling;

	printf("\n%f, %s", slopeDot, sonic->GetCurrentAnimationName().c_str());
	if (!BlueBlurCommon::IsSuper()) {
		Eigen::Vector3f inputDirection;
		Common::GetWorldInputDirection(inputDirection);

		Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
		Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

		Eigen::Vector3f playerPosition = sonic->m_spMatrixNode->m_Transform.m_Position;
		Eigen::Quaternionf playerRotation = sonic->m_spMatrixNode->m_Transform.m_Rotation;

		float moveMult = std::clamp(abs(inputDirection.norm()), 0.25f, 1.0f);

		if (isCrawling) {
			sonic->StateFlag(eStateFlag_UpdateYawByVelocity) = true;
			sonic->StateFlag(eStateFlag_CalibrateFrontDir) = true;

			sonic->m_HorizontalVelocity = playerRotation * Eigen::Vector3f::UnitZ() * crawlSpeed * moveMult;

			alignas(16) float dir[4] = { inputDirection.x(), inputDirection.y(), inputDirection.z(), 0 };
			if (!sonic->m_Is2DMode) {
				originalCrawlRotate(This, dir, 95, PI_F * 10.0f, true, crawlTurnRate * moveMult);
			}
			else {
				originalCrawlRotate(This, dir, 1000, 1000, true, 1000);
			}

			if (!inputDirection.isZero()) {
				if (anim != "CrawlLoop" && crawlEnterTime <= 0)
				{
					sonic->ChangeAnimation("CrawlLoop");
				}
			}

			if (slopeDot >= .25f && !Common::GetSonicStateFlags()->OnStairs) {
				isCrawling = false;
				sonic->m_HorizontalVelocity += sonic->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitZ() * 3.0f;
				sonic->ChangeState("Sliding");
				sonic->ChangeAnimation("Sliding");
			}
		}
		else {
			if (slopeDot >= .15f && sonic->m_Grounded && !Common::GetSonicStateFlags()->OnStairs) {
				isCrawling = false;
				sonic->SetHorizontalVelocity(sonic->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitZ() * 5.0f);
				sonic->ChangeState("Sliding");
				sonic->ChangeAnimation("Sliding");
			}
		}

		if (crawlExitTime <= 0 && anim == "CrawlExit" && !isCrawling) {
			sonic->ChangeAnimation("Squat");
		}

		if (sonic->m_Grounded) {
			if (!inputDirection.isZero()) {
				if (input.IsDown(Sonic::eKeyState_B)) {
					if (!isCrawling && crawlEnterTime <= 0) {
						crawlEnterTime = 0.3f;
						sonic->ChangeAnimation("CrawlEnter");
						isCrawling = true;
					}
				}
			}
			if (input.IsReleased(Sonic::eKeyState_B) || input.IsUp(Sonic::eKeyState_B)) {
				isCrawling = false;
				sonic->SetHorizontalVelocity(Hedgehog::Math::CVector::Zero());
				sonic->ChangeAnimation("SquatToStand");
			}
			if (inputDirection.isZero()) {
				isCrawling = false;
				if (anim == "CrawlLoop" || anim == "CrawlEnter") {
					crawlExitTime = 0.3f;
					sonic->ChangeAnimation("CrawlExit");
				}
			}

			if (input.IsTapped(Sonic::eKeyState_A)) {
				isCrawling = false;
			}
		}
		else if (!sonic->m_Grounded) {
			sonic->ChangeAnimation("Fall");
			isCrawling = false;
		}
	}
}
// Brianuuu 06 Experience
HOOK(int, __fastcall, SlideStart, 0x11D7110, void* This) {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

	Eigen::Vector3f inputDirection;
	Common::GetWorldInputDirection(inputDirection);

	if (sonic->m_Velocity.norm() <= 3.0f && input.IsDown(Sonic::eKeyState_B) && !inputDirection.isZero() && slopeDot < .15f) {
		sonic->ChangeState("Squat");
	}
	else {
		return originalSlideStart(This);
	}
}

HOOK(void, __fastcall, SonicCrawlUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicCrawlUpdate(This, _, updateInfo);

	if (BlueBlurCommon::IsModern()) {
		Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
		Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
		Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

		Eigen::Vector3f inputDirection;
		Common::GetWorldInputDirection(inputDirection);

		if (crawlEnterTime > 0) { crawlEnterTime -= updateInfo.DeltaTime; }
		if (crawlExitTime > 0) { crawlExitTime -= updateInfo.DeltaTime; }
	}
}

void Crawl::Install() {
	// Brianuuu 06 Experience
	// Don't allow stick move start sliding from squat
	WRITE_MEMORY(0x1230D62, uint8_t, 0xEB);
	WRITE_MEMORY(0x1230DA9, uint8_t, 0xE9, 0xA8, 0x00, 0x00, 0x00, 0x90);
	// Do not use sliding fail pfx
	WRITE_NOP(0x11D6A6D, 2);

	INSTALL_HOOK(SlideStart);
	INSTALL_HOOK(SquatAdvanceCrawl);
	INSTALL_HOOK(SonicCrawlUpdate);
}