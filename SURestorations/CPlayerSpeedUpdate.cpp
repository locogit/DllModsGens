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
	if (prevRingCount < sonic->m_RingCount) {
		if (sonic->m_ChaosEnergy < 100.0f && previousChaosEnergy + ringEnergyAmount < 100.0f)
			sonic->m_ChaosEnergy = max(previousChaosEnergy + ringEnergyAmount, 0);
	}
	else {
		previousChaosEnergy = sonic->m_ChaosEnergy;
	}
	prevRingCount = sonic->m_RingCount;
}

SharedPtrTypeless RampHandle;
bool usedRamp;

bool bobsleighBoostCancel = false;
bool usingBobsleigh = false;
bool bobsleigh = false;
bool poleSwing = false;
float poleParticleDelay = 0.25f;
float poleParticleTime = 0;
HOOK(void, __fastcall, CPlayerSpeedUpdateParallel, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	originalCPlayerSpeedUpdateParallel(This, _, updateInfo);
	if (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) {

		auto sonic = This->GetContext();
		auto state = This->m_StateMachine.GetCurrentState()->GetStateName();
		auto input = Sonic::CInputState::GetInstance()->GetPadState();

		if (sonic->m_ChaosEnergy != 100 && Common::CheckCurrentStage("pam000")) // For Boost Gauge Starts Empty Code
			sonic->m_ChaosEnergy = 100;

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
HOOK(void, __fastcall, ramp, 0x11DE240, int This) {
	if (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) {
		auto sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

		// dash rings use SpecialJump as well, this makes sure you are using a ramp by checking the animation that is playing.

		bool ramp = sonic->GetCurrentAnimationName() == "JumpBoardSpecialR" || sonic->GetCurrentAnimationName() == "JumpBoardSpecialL" || sonic->GetCurrentAnimationName() == "JumpBoardSpecial";

		void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);

		if (sonic->StateFlag(eStateFlag_Boost) && ramp && !usedRamp) {
			usedRamp = true;
			Common::fCGlitterCreate(sonic, RampHandle, middlematrixNode, "jump_delux", 1);
		}
	}
	originalramp(This);
}
// https://github.com/brianuuu/DllMods/blob/master/Source/NavigationLightdashSound/NavigationSound.cpp#L14
HOOK(void, __fastcall, MsgStartCommonButtonSign, 0x5289A0, void* thisDeclaration, void* edx, uint32_t a2)
{
	// Disable Y button prompt.
	if (*(uint32_t*)(a2 + 16) == 3)
		return;

	originalMsgStartCommonButtonSign(thisDeclaration, edx, a2);
}
std::vector<std::string> SUModelMods = { "Chip Bracelet (Unleashed)", "Pure SU Sonic", "SU Marza Sonic", "Unleashed Sonic Model"};
void CPlayerSpeedUpdate::Install()
{
	usingBobsleigh = Common::UP || Common::AP;

	for(std::string modName : SUModelMods)
	{
		if (Common::IsModEnabled(modName) || Common::IsModEnabledContains(modName))
			SUMouthFix = true;
	}

	if(Common::reader.GetBoolean("Config", "ForceMouthFix", false))
		SUMouthFix = Common::reader.GetBoolean("Config", "SUMouthFix", true); // incase you need to override the bool

	//https://github.com/LadyLunanova
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
	if (usingBobsleigh) {
		WRITE_MEMORY(0xDFF622, byte, 0xEB); // Disables Drifting when using bobsleigh
	}
	INSTALL_HOOK(ramp);
	INSTALL_HOOK(CHudSonicStageUpdateParallel);
	INSTALL_HOOK(CPlayerSpeedUpdateParallel);
	INSTALL_HOOK(MsgStartCommonButtonSign);
}