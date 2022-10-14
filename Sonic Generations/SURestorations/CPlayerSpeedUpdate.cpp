// Original code by Brianuuu: https://github.com/brianuuu
void __declspec(naked) groundBoostSuperSonicOnly()
{
	static uint32_t returnAddress = 0xDFF270;
	static uint32_t failAddress = 0xDFF2CB;
	__asm
	{
		// disable air boost for normal Sonic
		mov     eax, [ebx + 534h]
		mov     eax, [eax + 4]
		cmp     byte ptr[eax + 6Fh], 0
		jz      jump

		// original function
		movss   xmm0, dword ptr[ebx + 5BCh]
		jmp[returnAddress]

		jump:
		jmp[failAddress]
	}
}

void __declspec(naked) airBoostSuperSonicOnly()
{
	static uint32_t returnAddress = 0xDFE094;
	static uint32_t failAddress = 0xDFDFE6;
	__asm
	{
		// disable air boost for normal Sonic
		mov     eax, [esi + 534h]
		mov     eax, [eax + 4]
		cmp     byte ptr[eax + 6Fh], 0
		jz      jump

		// original function
		movss   xmm0, dword ptr[esi + 5BCh]
		jmp[returnAddress]

		jump:
		jmp[failAddress]
	}
}
bool SUMouthFix;
float ringEnergyAmount = 2.75f;
size_t prevRingCount;
float previousChaosEnergy;
bool MoreVoice = Common::reader.GetBoolean("Changes", "MoreVoice", false);
HOOK(void, __fastcall, CHudSonicStageUpdateParallel, 0x1098A50, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo) {
	originalCHudSonicStageUpdateParallel(This, Edx, in_rUpdateInfo);
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (sonic) {
		if (prevRingCount < sonic->m_RingCount) {
			if (sonic->m_ChaosEnergy < 100.0f && previousChaosEnergy + ringEnergyAmount < 100.0f)
				sonic->m_ChaosEnergy = max(previousChaosEnergy + ringEnergyAmount, 0);
		}
		else {
			previousChaosEnergy = sonic->m_ChaosEnergy;
		}
		prevRingCount = sonic->m_RingCount;
	}
}

SharedPtrTypeless RampHandle;
bool usedRamp;

bool bobsleighBoostCancel = false;
bool usingBobsleigh = Common::UP || Common::AP || Common::IsModEnabled("EggmanLand");
int lastHurdleIndex = 0;
int currentHurdleIndex = 1;
bool InfLivesCodeChange = Common::reader.GetBoolean("Changes", "InfLives", true);

bool isCrawling = false;
const float crawlSpeed = 10;
float crawlEnterTime = 0;
float crawlExitTime = 0;
bool DoMSpeed;
bool MSpeed = Common::reader.GetBoolean("Restorations", "MSpeed", true);
int BPressed = 0;
bool BResetTimerEnable = false;
float BResetTimer;
SharedPtrTypeless squatKickParticleHandle;
Hedgehog::Math::CQuaternion squatKickRotation;
HOOK(int*, __fastcall, CSonicStateSquatKickBegin, 0x12526D0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	Sonic::Player::CPlayerSpeedContext* context = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;

	void* middlematrixNode = (void*)((uint32_t)context + 0x10);
	Common::fCGlitterCreate(context, squatKickParticleHandle, middlematrixNode, "Sweepkick", 1);

	context->PlaySound(2002497, true);

	return originalCSonicStateSquatKickBegin(This);
}
HOOK(int*, __fastcall, CSonicStateSquatKickEnd, 0x12527B0, void* This)
{
	Common::fCGlitterEnd(BlueBlurCommon::GetContext(), squatKickParticleHandle, true);
	return originalCSonicStateSquatKickEnd(This);
}
const char* lastAnimDashRing = "";
HOOK(void, __fastcall, CPlayerSpeedUpdateParallel, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	if ((byte)Common::GetMultiLevelAddress(0xD59A67, { 0x6 }) == 150 && *Common::GetPlayerLives() != 99 && InfLivesCodeChange)
		*Common::GetPlayerLives() = 99;

	originalCPlayerSpeedUpdateParallel(This, _, updateInfo);

	if (BlueBlurCommon::IsModern()) {
		Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
		Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
		Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
		Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();
		Hedgehog::Math::CVector velNoY = sonic->m_Velocity;
		velNoY.y() = 0;
		printf("%s\n", anim);
		if (anim == "Fall" && state == "SpecialJump" && !sonic->StateFlag(eStateFlag_Boost)) {
			sonic->ChangeAnimation("JumpBoardLoop");
		}
		if (anim == "IdleInWater") {
			int randNum = rand() % 5;

			switch (randNum)
			{
			case 0:
				sonic->ChangeAnimation("Stand");
				break;
			case 1:
				sonic->ChangeAnimation("IdleA");
				break;
			case 2:
				sonic->ChangeAnimation("IdleB");
				break;
			case 3:
				sonic->ChangeAnimation("IdleC");
				break;
			case 4:
				sonic->ChangeAnimation("IdleD");
				break;
			case 5:
				sonic->ChangeAnimation("IdleE");
				break;
			}
		}
		if (anim == "DashRingL" || anim == "DashRingR") {
			if (MoreVoice) {
				if (lastAnimDashRing != anim.c_str()) {
					lastAnimDashRing = anim.c_str();
					sonic->PlaySound(2002498, true);
					static SharedPtrTypeless soundHandle;
					Common::SonicContextPlayVoice(soundHandle, 0, 30);
				}
			}
		}
		else {
			if (lastAnimDashRing != "") {
				lastAnimDashRing = "";
			}
		}

		if (input.IsTapped(Sonic::eKeyState_B) && Common::reader.GetBoolean("Restorations", "SweepKick", true)) {
			if (!BResetTimerEnable) {
				BResetTimer = 0.3f;
				BResetTimerEnable = true;
			}
			BPressed++;
			if (BPressed == 2) {
				BPressed = 0;
				if (BResetTimer > 0) {
					bool canSquatKick = (state == "Squat" || state == "Sliding") && !Common::IsPlayerControlLocked() && !BlueBlurCommon::IsSuper();
					if (canSquatKick) {
						squatKickRotation = sonic->m_spMatrixNode->m_Transform.m_Rotation;
						sonic->ChangeState("SquatKick");
					}
				}
			}
		}

		if (state == "SquatKick") {
			// Original code by brianuuu
			if (sonic->m_Velocity.norm() == 0.0f) {
				sonic->m_spMatrixNode->m_Transform.SetRotation(squatKickRotation);
			}
			else {
				Common::SonicContextUpdateRotationToVelocity(sonic, &sonic->m_Velocity, true);
			}
		}

		if (BResetTimerEnable) {
			if (BResetTimer > 0) BResetTimer -= updateInfo.DeltaTime;
			if (BResetTimer <= 0) {
				BPressed = 0;
				BResetTimerEnable = false;
			}
		}

		if (MSpeed) {

			float mSpeedStick = Common::IsPlayerIn2D() ? input.LeftStickHorizontal : input.LeftStickVertical;
			float mSpeedThreshold = Common::IsPlayerIn2D() ? 45 : 90;

			if (!Common::IsPlayerOnBoard() && abs(velNoY.norm()) >= mSpeedThreshold && !sonic->m_Grounded && DoMSpeed && abs(mSpeedStick) < 0.25f && abs(mSpeedStick) > 0.0f) {
				float dimensionMultiplier = Common::IsPlayerIn2D() ? 50 : 95;
				sonic->m_Velocity += sonic->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitZ() * (updateInfo.DeltaTime * dimensionMultiplier);
				sonic->m_PreviousVelocity = sonic->m_Velocity;
			}

			if (strstr(state.c_str(), "JumpShort") && sonic->m_Grounded) {
				DoMSpeed = true;
			}
			else if (!strstr(state.c_str(), "JumpShort") && !strstr(state.c_str(), "Jump")) {
				DoMSpeed = false;
			}

		}

		if (crawlEnterTime > 0) { crawlEnterTime -= updateInfo.DeltaTime; }
		if (crawlExitTime > 0) { crawlExitTime -= updateInfo.DeltaTime; }

		if (sonic->m_ChaosEnergy != 100 && Common::CheckCurrentStage("pam000")) // For Boost Gauge Starts Empty Code
			sonic->m_ChaosEnergy = 100;

		if (state == "JumpHurdle")
			lastHurdleIndex = (anim == "JumpHurdleL") ? 1 : 0;

		// If bobsleigh is used (using UP or AP)
		if (usingBobsleigh) {
			if (strstr(state.c_str(), "Board")) {
				if (!bobsleighBoostCancel) { bobsleighBoostCancel = true; }
				Common::SonicContextSetCollision(TypeSonicBoost, true);
				sonic->StateFlag(eStateFlag_EndBoost) = true;
				WRITE_JUMP(0xDFF268, groundBoostSuperSonicOnly);
				WRITE_JUMP(0xDFE05F, airBoostSuperSonicOnly);
			}
			else if (bobsleighBoostCancel) {
				Common::SonicContextSetCollision(TypeSonicBoost, false);
				WRITE_MEMORY(0xDFF268, uint8_t, 0xF3, 0x0F, 0x10, 0x83, 0xBC);
				WRITE_MEMORY(0xDFE05F, uint8_t, 0xF3, 0x0F, 0x10, 0x86, 0xBC);
				bobsleighBoostCancel = false;
			}
		}

		if (state != "SpecialJump")
			usedRamp = false;
	}
}
SharedPtrTypeless RampVoiceHandle;
HOOK(void, __fastcall, RampParticle, 0x11DE240, int This) {
	if (BlueBlurCommon::IsModern()) {
		Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

		// dash rings use SpecialJump as well, this makes sure you are using a ramp by checking the animation that is playing.
		bool ramp = strstr(sonic->GetCurrentAnimationName().c_str(), "JumpBoardSpecial");
		void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
		if (sonic->StateFlag(eStateFlag_Boost) && ramp && !usedRamp) {
			usedRamp = true;
			Common::fCGlitterCreate(sonic, RampHandle, middlematrixNode, "jump_delux", 1);
			printf("[SU Restorations] Play Ramp Particle\n");
		}
	}
	originalRampParticle(This);
}
// https://github.com/brianuuu/DllMods/blob/master/Source/NavigationLightdashSound/NavigationSound.cpp#L14
HOOK(void, __fastcall, MsgStartCommonButtonSign, 0x5289A0, void* thisDeclaration, void* edx, uint32_t a2)
{
	// Disable Y button prompt.
	if (*(uint32_t*)(a2 + 16) == 3)
		return;

	originalMsgStartCommonButtonSign(thisDeclaration, edx, a2);
}
HOOK(int*, __fastcall, CSonicStateSquatBegin, 0x1230A30, void* This)
{
	return originalCSonicStateSquatBegin(This);
}
bool CrawlEnabled = Common::reader.GetBoolean("Restorations", "Crawl", true);
float crawlTurnRate = 400;
HOOK(void, __stdcall, CSonicRotationAdvance, 0xE310A0, void* a1, float* targetDir, float turnRate1, float turnRateMultiplier, bool noLockDirection, float turnRate2)
{
	originalCSonicRotationAdvance(a1, targetDir, turnRate1, turnRateMultiplier, noLockDirection, turnRate2);
}
// Unleashed HUD
HOOK(void, __stdcall, CPlayerGetLife, 0xE75520, Sonic::Player::CPlayerContext* context, int lifeCount, bool playSound)
{
	originalCPlayerGetLife(context, lifeCount, playSound);

	if (lifeCount > 0)
	{
		Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		if (sonic && BlueBlurCommon::IsModern()) {
			sonic->PlaySound(2002499, true);
			static SharedPtrTypeless soundHandle;
			Common::SonicContextPlayVoice(soundHandle, 0, 30);
		}
	}
}
HOOK(void, __fastcall, CSonicStateSquatAdvance, 0x1230B60, void* This)
{
	if (CrawlEnabled && !BlueBlurCommon::IsSuper()) {

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
				originalCSonicRotationAdvance(This, dir, 100, PI_F * 10.0f, false, crawlTurnRate * moveMult);
			}
			else {
				originalCSonicRotationAdvance(This, dir, 1000, 1000, false, 1000);
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
				originalCSonicStateSquatAdvance(This);
			}
			else if (input.IsUp(Sonic::eKeyState_B)) {
				isCrawling = false;
				if (!inputDirection.isZero()) {
					sonic->ChangeState("Walk");
				}
				else {
					originalCSonicStateSquatAdvance(This);
				}
			}
		}
		else if (!sonic->m_Grounded) {
			isCrawling = false;
			originalCSonicStateSquatAdvance(This);
		}
	}
	else {
		originalCSonicStateSquatAdvance(This);
	}
}
std::vector<std::string> SUModelMods = { "Chip Bracelet (Unleashed)", "Pure SU Sonic", "SU Marza Sonic", "Unleashed Sonic Model"};
bool DisableBoard = Common::reader.GetBoolean("Changes", "DisableBoard", false);
void CreateConsole()
{
	if (!AllocConsole()) {
		// Add some error handling here.
		// You can call GetLastError() to get more info about the error.
		return;
	}

	// std::cout, std::clog, std::cerr, std::cin
	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
	std::cin.clear();

	// std::wcout, std::wclog, std::wcerr, std::wcin
	HANDLE hConOut = CreateFile(_T("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hConIn = CreateFile(_T("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
	SetStdHandle(STD_ERROR_HANDLE, hConOut);
	SetStdHandle(STD_INPUT_HANDLE, hConIn);
	std::wcout.clear();
	std::wclog.clear();
	std::wcerr.clear();
	std::wcin.clear();
}
bool shortJumpMove = false;
bool JumpRestore = Common::reader.GetBoolean("Restorations", "RunJump", true);
HOOK(int, __fastcall, ShortJumpMove, 0x11BF200, int This) {
	if (JumpRestore) {
		Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		float velXZ = sonic->m_Velocity.x() + sonic->m_Velocity.z();
		shortJumpMove = abs(velXZ) >= 5;
	}
	return originalShortJumpMove(This);
}
HOOK(void, __fastcall, HurdleAnim, 0x11BEEC0, float *This) {
	originalHurdleAnim(This);
	if (JumpRestore) {
		Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
		if (shortJumpMove) {
			if (anim == "JumpShortBegin") {
				currentHurdleIndex = (lastHurdleIndex == 0) ? 1 : 0;
				lastHurdleIndex = currentHurdleIndex;
			}
			if (anim != "JumpHurdleL" && anim != "JumpHurdleR") {
				if (anim != "SpinAttack") {
					Hedgehog::Base::CSharedString hurdleAnimName = (currentHurdleIndex == 0) ? "JumpHurdleL" : "JumpHurdleR";
					sonic->ChangeAnimation(hurdleAnimName);
				}
			}
		}
	}
}
void HMMPatches() {
	if (Common::reader.GetBoolean("Restorations", "XHoming", true)) {
		if (*(byte*) 0x015FA418 != 0) {
			//Patch "Homing Attack on Boost" by "SWS90"
			WRITE_MEMORY(0x015FA418, byte, 0);
		}
	}
}
void CPlayerSpeedUpdate::Install()
{
	CreateConsole();

	HMMPatches();

	for(std::string modName : SUModelMods)
	{
		if (Common::IsModEnabled(modName) || Common::IsModEnabledContains(modName)) {
			printf("[SU Restorations] %s Found, Fixing Mouth...\n", modName.c_str());
			SUMouthFix = true;
		}
	}

	if (Common::reader.GetBoolean("Hidden", "ForceMouthFix", false)) {
		SUMouthFix = Common::reader.GetBoolean("Hidden", "SUMouthFix", false); // incase you need to override the bool
		printf("[SU Restorations] SU Mouth Fix Overwritten.\n");
	}

	if (DisableBoard) {
		//Credit to Skyth
		WRITE_MEMORY(0xD947CC, byte, 0xEB);
		WRITE_MEMORY(0xE6B750, byte, 0xC2, 0x04, 0x00);
		printf("[SU Restorations] Disabling Board...\n");
	}

	//Credit to LadyLunanova
	if (SUMouthFix) {
		//Right Mouth
		WRITE_MEMORY(0x015E8FB4, const char*, "Jaw_LT");
		WRITE_MEMORY(0x015E8FBC, const char*, "Lip_C_LT");
		WRITE_MEMORY(0x015E8FCC, const char*, "Lip_L_LT");
		WRITE_MEMORY(0x015E8FD4, const char*, "Lip_R_LT");
		//Left Mouth
		WRITE_MEMORY(0x015E8FFC, const char*, "Jaw_LT1");
		WRITE_MEMORY(0x015E9004, const char*, "Lip_C_LT1");
		WRITE_MEMORY(0x015E9014, const char*, "Lip_L_LT1");
		WRITE_MEMORY(0x015E901C, const char*, "Lip_R_LT1");
	}

	// Credit to Skyth
	if (usingBobsleigh)
		WRITE_MEMORY(0xDFF622, byte, 0xEB); // Disables Drifting when using bobsleigh

	// https://github.com/brianuuu/DllMods/blob/master/Source/Sonic06DefinitiveExperience/NextGenSonic.cpp
	if(Common::reader.GetBoolean("Restorations", "DPadDisable", true))
		WRITE_JUMP(0xD97B56, (void*)0xD97B9E); // Disable D-Pad Input
	INSTALL_HOOK(ShortJumpMove);
	INSTALL_HOOK(HurdleAnim);
	INSTALL_HOOK(RampParticle);
	INSTALL_HOOK(CHudSonicStageUpdateParallel);
	INSTALL_HOOK(CPlayerSpeedUpdateParallel);
	INSTALL_HOOK(MsgStartCommonButtonSign);
	INSTALL_HOOK(CSonicStateSquatBegin);
	INSTALL_HOOK(CSonicStateSquatAdvance);

	if (MoreVoice) { INSTALL_HOOK(CPlayerGetLife); }

	//Original code by Brianuu
	static double const c_sweepKickActivateTime = 0.0f;
	WRITE_MEMORY(0x125299E, double*, &c_sweepKickActivateTime);

	INSTALL_HOOK(CSonicStateSquatKickBegin);
	INSTALL_HOOK(CSonicStateSquatKickEnd);

	if(Common::reader.GetBoolean("Changes", "UnwiishedFall", false))
		WRITE_MEMORY(0x15E812C, char, "sn_wall_fly02_loop");
}