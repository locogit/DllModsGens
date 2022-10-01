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
HOOK(void, __fastcall, CHudSonicStageUpdateParallel, 0x1098A50, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo) {
	originalCHudSonicStageUpdateParallel(This, Edx, in_rUpdateInfo);
	auto sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
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
bool bobsleigh = false;
bool poleSwing = false;
float poleParticleDelay = 0.25f;
float poleParticleTime = 0;
int lastHurdleIndex = 0;
int currentHurdleIndex = 1;
bool InfLivesCodeChange = Common::reader.GetBoolean("Changes", "InfLives", true);

bool isCrawling = false;
const float crawlSpeed = 10;
float crawlEnterTime = 0;
HOOK(void, __fastcall, CPlayerSpeedUpdateParallel, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	originalCPlayerSpeedUpdateParallel(This, _, updateInfo);
	if (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) {
		auto sonic = This->GetContext();
		auto state = This->m_StateMachine.GetCurrentState()->GetStateName();
		auto anim = sonic->GetCurrentAnimationName();
		auto input = Sonic::CInputState::GetInstance()->GetPadState();

		//printf("%s\n", state.c_str());

		if (crawlEnterTime > 0) { crawlEnterTime -= updateInfo.DeltaTime; }

		if ((byte)Common::GetMultiLevelAddress(0xD59A67, { 0x6 }) == 150 && *Common::GetPlayerLives() != 99 && InfLivesCodeChange)
			*Common::GetPlayerLives() = 99;

		if (sonic->m_ChaosEnergy != 100 && Common::CheckCurrentStage("pam000")) // For Boost Gauge Starts Empty Code
			sonic->m_ChaosEnergy = 100;

		if (state == "JumpHurdle")
			lastHurdleIndex = (anim == "JumpHurdleL") ? 1 : 0;

		if (state == "ExternalControl" && sonic->GetCurrentAnimationName() == "PoleSpinLoop" && !poleSwing) {
			poleSwing = true;
			poleParticleTime = poleParticleDelay;
		}
		else if (state != "ExternalControl" && poleSwing) {
			if (poleParticleTime > 0)
				poleParticleTime -= updateInfo.DeltaTime;
			else
				poleSwing = false;
		}

		// If bobsleigh is used (using UP or AP)
		if (usingBobsleigh) {
			// All of the states you do not want to be allowed to boost in.
			std::vector<Hedgehog::Base::CSharedString> list = { "BoardWalk","BoardJump","BoardFall","BoardLandJumpShort","CPlayerSpeedStateBoardTrickJump","BoardJumpShort","BoardGrindLandJumpShort","BoardGrindJumpShort","BoardGetOn","BoardDrift","BoardGrind" };
			bobsleigh = std::find(list.begin(), list.end(), state) != list.end();
			if (bobsleigh) {
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
bool RampVoice = Common::reader.GetBoolean("Changes", "RampVoice", false);
HOOK(void, __fastcall, RampParticle, 0x11DE240, int This) {
	if (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) {
		auto sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

		// dash rings use SpecialJump as well, this makes sure you are using a ramp by checking the animation that is playing.

		bool ramp = sonic->GetCurrentAnimationName() == "JumpBoardSpecialR" || sonic->GetCurrentAnimationName() == "JumpBoardSpecialL" || sonic->GetCurrentAnimationName() == "JumpBoardSpecial";

		void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);

		if (sonic->StateFlag(eStateFlag_Boost) && ramp && !usedRamp) {
			usedRamp = true;
			Common::fCGlitterCreate(sonic, RampHandle, middlematrixNode, "jump_delux", 1);
			printf("[SU Restorations] Play Ramp Particle\n");
			if (RampVoice) { Common::SonicContextPlayVoice(RampVoiceHandle, 3002013, 30); }
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
float crawlTurnRate = 200;
HOOK(void, __stdcall, CSonicRotationAdvance, 0xE310A0, void* a1, float* targetDir, float turnRate1, float turnRateMultiplier, bool noLockDirection, float turnRate2)
{
	originalCSonicRotationAdvance(a1, targetDir, turnRate1, turnRateMultiplier, noLockDirection, turnRate2);
}
HOOK(void, __fastcall, CSonicStateSquatAdvance, 0x1230B60, void* This)
{
	if (CrawlEnabled) {

		Eigen::Vector3f inputDirection;
		Common::GetWorldInputDirection(inputDirection);

		auto sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		auto anim = sonic->GetCurrentAnimationName();
		auto input = Sonic::CInputState::GetInstance()->GetPadState();

		float moveMult = std::clamp(abs(inputDirection.norm()), 0.15f, 1.0f);

		if (isCrawling) {
			sonic->m_Velocity = (sonic->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitZ() * crawlSpeed) * moveMult;
			if (!inputDirection.isZero()) {
				alignas(16) float dir[4] = { inputDirection.x(), inputDirection.y(), inputDirection.z(), 0 };
				originalCSonicRotationAdvance(This, dir, crawlTurnRate * moveMult, crawlTurnRate * moveMult, false, crawlTurnRate * moveMult);
				if (anim != "CrawlLoop" && crawlEnterTime <= 0)
				{
					sonic->ChangeAnimation("CrawlLoop");
				}
			}
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
					sonic->ChangeAnimation("CrawlExit");
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
		auto sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		auto velXZ = sonic->m_Velocity.x() + sonic->m_Velocity.z();
		shortJumpMove = abs(velXZ) >= 5;
	}
	return originalShortJumpMove(This);
}
HOOK(void, __fastcall, HurdleAnim, 0x11BEEC0, float *This) {
	originalHurdleAnim(This);
	if (JumpRestore) {
		auto sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		auto anim = sonic->GetCurrentAnimationName();
		if (shortJumpMove) {
			if (anim == "JumpShortBegin") {
				currentHurdleIndex = (lastHurdleIndex == 0) ? 1 : 0;
				lastHurdleIndex = currentHurdleIndex;
			}
			if (anim != "JumpHurdleL" && anim != "JumpHurdleR") {
				if (anim != "SpinAttack") {
					auto hurdleAnimName = (currentHurdleIndex == 0) ? "JumpHurdleL" : "JumpHurdleR";
					sonic->ChangeAnimation(hurdleAnimName);
				}
			}
		}
	}
}
void HMMPatches() {
	//Patch "Disable Spin Dash on Dash Panels" by "Hyper"
	WRITE_MEMORY(0xE0AC1C, byte, 0xE9, 0x27, 0x01, 0x00, 0x00);
	WRITE_MEMORY(0xE0C734, byte, 0xE9, 0x27, 0x01, 0x00, 0x00);

	//Patch "Unleashed Style Grinding Animations" by "Skyth"
	WRITE_MEMORY(0xDF2380, USHORT, 0xA4E9);
	WRITE_MEMORY(0xDF2382, byte, 0x0);
	WRITE_NOP(0xDF2385, 1);
	WRITE_MEMORY(0xDF2356, byte, 0xEB);
	WRITE_MEMORY(0xDF2485, byte, 0xEB);

	//Patch "Unleashed Stick Deadzones" by "M&M"
	WRITE_MEMORY(0x16055EC, byte, 0x69); /* InputThreshold (0.02f) */
	WRITE_MEMORY(0xE75F93, byte, 0x38, 0x71); /* InputTransformHalf (0.85f) */
	WRITE_MEMORY(0x1605610, byte, 0x69);
	WRITE_MEMORY(0x160562C, byte, 0x69); /* InputTransformPower (3.0f) */

	//Patch "Use Bumpers to Switch Grind Rails" by "Skyth"
	WRITE_MEMORY(0xDFCC92, UINT, 0x10244C8B);
	WRITE_NOP(0xDFCC96, 2);
	WRITE_MEMORY(0xDFCC99, byte, 0xE1BA);
	WRITE_MEMORY(0xDFCC9B, byte, 0xD);
	WRITE_NOP(0xDFCC9C, 3);
	WRITE_MEMORY(0xDFCC9F, byte, 0x73);
	WRITE_MEMORY(0xDFCCA8, UINT, 0xCE1BA0F);
	WRITE_NOP(0xDFCCAC, 7);
	WRITE_MEMORY(0xDFCCB3, byte, 0x73);

	if (Common::reader.GetBoolean("Restorations", "XHoming", true)) {
		//Patch "Homing Attack on Boost" by "SWS90"
		WRITE_MEMORY(0x015FA418, byte, 0);
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
	INSTALL_HOOK(CSonicRotationAdvance);
	INSTALL_HOOK(CSonicStateSquatAdvance);
}