bool isCrawling = false;
const float crawlSpeed = 8;
float crawlEnterTime = 0;
float crawlExitTime = 0;

float crawlTurnRate = 400;
HOOK(void, __stdcall, CrawlRotate, 0xE310A0, void* a1, float* targetDir, float turnRate1, float turnRateMultiplier, bool noLockDirection, float turnRate2)
{
	originalCrawlRotate(a1, targetDir, turnRate1, turnRateMultiplier, noLockDirection, turnRate2);
}
HOOK(void, __fastcall, SquatAdvanceCrawl, 0x1230B60, void* This)
{
	if (!BlueBlurCommon::IsSuper()) {

		Eigen::Vector3f inputDirection;
		Hedgehog::Math::CVector crawlVelocity;
		Common::GetWorldInputDirection(inputDirection);

		Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
		Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();
		float moveMult = std::clamp(abs(inputDirection.norm()), 0.15f, 1.0f);

		if (isCrawling) {
			crawlVelocity = (sonic->m_spMatrixNode->m_Transform.m_Rotation.normalized() * Eigen::Vector3f::UnitZ() * crawlSpeed) * moveMult;
			crawlVelocity.y() = sonic->m_Velocity.y();
			sonic->m_Velocity = crawlVelocity;
			alignas(16) float dir[4] = { inputDirection.normalized().x(), inputDirection.y(), inputDirection.normalized().z(), 0 };
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
					sonic->ChangeState("Stand");
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
		if (crawlEnterTime > 0) { crawlEnterTime -= updateInfo.DeltaTime; }
		if (crawlExitTime > 0) { crawlExitTime -= updateInfo.DeltaTime; }
	}
}
void Crawl::Install() {
	INSTALL_HOOK(SquatAdvanceCrawl);
	INSTALL_HOOK(SonicCrawlUpdate);
}