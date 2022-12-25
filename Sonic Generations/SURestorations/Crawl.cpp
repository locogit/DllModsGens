bool isCrawling = false;
const float crawlSpeed = 8;
float crawlEnterTime = 0;
float crawlExitTime = 0;

float crawlTurnRate = 400;

float crawlSpeedMult = 1.0f;
float slopeDot;
HOOK(void, __stdcall, CrawlRotate, 0xE310A0, void* a1, float* targetDir, float turnRate1, float turnRateMultiplier, bool noLockDirection, float turnRate2)
{
	originalCrawlRotate(a1, targetDir, turnRate1, turnRateMultiplier, noLockDirection, turnRate2);
}

HOOK(void, __fastcall, SquatAdvanceCrawl, 0x1230B60, void* This)
{
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	slopeDot = sonic->m_UpVector.dot(sonic->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitZ());

	printf("\n%f, %f", slopeDot, crawlSpeedMult);
	if (!BlueBlurCommon::IsSuper()) {
		Eigen::Vector3f inputDirection;
		Common::GetWorldInputDirection(inputDirection);

		Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
		Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

		float moveMult = std::clamp(abs(inputDirection.norm()), 0.25f, 1.0f);

		if (isCrawling) {

			Eigen::Vector3f playerPosition = sonic->m_spMatrixNode->m_Transform.m_Position;
			Eigen::Quaternionf playerRotation = sonic->m_spMatrixNode->m_Transform.m_Rotation;

			alignas(16) MsgApplyImpulse message {};
			{
				message.m_position = playerPosition;
				message.m_impulse = playerRotation * Eigen::Vector3f::UnitZ();
				message.m_impulseType = ImpulseType::None;
				message.m_outOfControl = 0.0f;
				message.m_notRelative = true;
				message.m_snapPosition = false;
				message.m_pathInterpolate = false;
				message.m_alwaysMinusOne = -1.0f;

				message.m_impulse *= crawlSpeed * moveMult * (slopeDot >= .15f ? crawlSpeedMult : 1.0f);

				Common::ApplyPlayerApplyImpulse(message);
			}

			alignas(16) float dir[4] = { inputDirection.x(), inputDirection.y(), inputDirection.z(), 0 };
			if (!Common::IsPlayerIn2D()) {
				originalCrawlRotate(This, dir, 100, PI_F * 10.0f, false, crawlTurnRate * moveMult);
			}
			else {
				originalCrawlRotate(This, dir, 1000, 1000, false, 1000);
			}
			if (!inputDirection.isZero()) {
				if (anim != "CrawlLoop" && crawlEnterTime <= 0)
				{
					sonic->ChangeAnimation("CrawlLoop");
				}
			}
		}
		if (crawlExitTime <= 0 && anim == "CrawlExit" && !isCrawling) {
			sonic->ChangeAnimation("Squat");
		}

		if (sonic->m_Grounded) {
			if (!inputDirection.isZero() && input.IsDown(Sonic::eKeyState_B)) {
				if (!isCrawling && crawlEnterTime <= 0) {
					crawlEnterTime = 0.3f;
					sonic->ChangeAnimation("CrawlEnter");
				}
				isCrawling = true;
			}
			else if (inputDirection.isZero()) {
				isCrawling = false;
				if (anim == "CrawlLoop" || anim == "CrawlEnter") {
					if (anim != "Squat") {
						crawlExitTime = 0.3f;
						sonic->ChangeAnimation("CrawlExit");
					}
				}
				originalSquatAdvanceCrawl(This);
			}
			else if (input.IsUp(Sonic::eKeyState_B)) {
				isCrawling = false;
				if (!inputDirection.isZero()) {
					sonic->m_Velocity = Hedgehog::Math::CVector::Zero();
					originalSquatAdvanceCrawl(This);
				}
				else {
					originalSquatAdvanceCrawl(This);
				}
			}
			if (input.IsTapped(Sonic::eKeyState_A)) {
				originalSquatAdvanceCrawl(This);
				isCrawling = false;
			}
		}
		else if (!sonic->m_Grounded) {
			isCrawling = false;
			originalSquatAdvanceCrawl(This);
		}
	}
	else {
		originalSquatAdvanceCrawl(This);
	}
}
HOOK(void, __fastcall, SonicCrawlUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicCrawlUpdate(This, _, updateInfo);
	if (BlueBlurCommon::IsModern()) {
		Eigen::Vector3f inputDirection;
		Common::GetWorldInputDirection(inputDirection);
		if (crawlEnterTime > 0) { crawlEnterTime -= updateInfo.DeltaTime; }
		if (crawlExitTime > 0) { crawlExitTime -= updateInfo.DeltaTime; }
		if (slopeDot >= .15f && !inputDirection.isZero()) { crawlSpeedMult += updateInfo.DeltaTime * 0.75f; }
		else { crawlSpeedMult = 1.0f; }
	}
}
void Crawl::Install() {
	INSTALL_HOOK(SquatAdvanceCrawl);
	INSTALL_HOOK(SonicCrawlUpdate);
}