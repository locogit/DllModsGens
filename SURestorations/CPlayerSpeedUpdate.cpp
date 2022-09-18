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
float ringEnergyAmount = 2.0f;
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
HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicNew</Archive>\n";
		stream << "  </DefAppend>\n";

		str = stream.str();
	}

	const size_t newSize = size + str.size();
	const std::unique_ptr<char[]> buffer = std::make_unique<char[]>(newSize);
	memcpy(buffer.get(), data, size);

	char* insertionPos = strstr(buffer.get(), "<Include>");

	memmove(insertionPos + str.size(), insertionPos, size - (size_t)(insertionPos - buffer.get()));
	memcpy(insertionPos, str.c_str(), str.size());

	bool result;
	{
		result = originalParseArchiveTree(A1, buffer.get(), newSize, database);
	}

	return result;
}

SharedPtrTypeless RampHandle;
bool usedRamp;

bool bobsleighBoostCancel = false;
bool usingBobsleigh = false;
bool bobsleigh = false;

template<typename C, typename T>
bool contains(C&& c, T e) {
	return std::find(std::begin(c), std::end(c), e) != std::end(c);
};
bool poleSwing = false;
float poleParticleDelay = 0.25f;
float poleParticleTime = 0;
HOOK(void, __fastcall, CPlayerSpeedUpdateParallel, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	originalCPlayerSpeedUpdateParallel(This, _, updateInfo);
	if (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) {
		auto sonic = This->GetContext();
		auto state = This->m_StateMachine.GetCurrentState()->GetStateName();
		if (state == "ExternalControl" && sonic->GetCurrentAnimationName() == "PoleSpinLoop" && !poleSwing) {
			poleSwing = true;
			poleParticleTime = poleParticleDelay;
		}
		else if (state != "ExternalControl" && poleSwing) {
			if (poleParticleTime > 0) {
				poleParticleTime -= updateInfo.DeltaTime;
			}
			else {
				poleSwing = false;
			}
		}

		// If bobsleigh is used (using UP or AP)
		if (usingBobsleigh) {
			// All of the states you do not want to be allowed to boost in.
			Hedgehog::Base::CSharedString list[] = { "BoardWalk","BoardJump","BoardFall","BoardLandJumpShort","CPlayerSpeedStateBoardTrickJump","BoardJumpShort","BoardGrindLandJumpShort","BoardGrindJumpShort","BoardGetOn","BoardDrift","BoardGrind" };
			bobsleigh = contains(list, state);
			if (bobsleigh && !bobsleighBoostCancel) {
				bobsleighBoostCancel = true;
				Common::SonicContextSetCollision(TypeSonicBoost, true);
				sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_EndBoost] = 1;
				WRITE_JUMP(0xDFF268, groundBoostSuperSonicOnly);
				WRITE_JUMP(0xDFE05F, airBoostSuperSonicOnly);
			}
			else if (!bobsleigh && bobsleighBoostCancel) {
				bobsleighBoostCancel = false;
				Common::SonicContextSetCollision(TypeSonicBoost, false);
				WRITE_MEMORY(0xDFF268, uint8_t, 0xF3, 0x0F, 0x10, 0x83, 0xBC);
				WRITE_MEMORY(0xDFE05F, uint8_t, 0xF3, 0x0F, 0x10, 0x86, 0xBC);
			}
		}

		if (state != "SpecialJump") { usedRamp = false; }
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
void CPlayerSpeedUpdate::Install()
{
	//CreateConsole();

	usingBobsleigh = Common::IsModEnabled("Unleashed Project") || Common::IsModEnabledContains("Holoska Adventure Pack");

	SUMouthFix = Common::reader.GetBoolean("Config","SUMouthFix",true);
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

	INSTALL_HOOK(ParseArchiveTree);
	INSTALL_HOOK(ramp);
	//INSTALL_HOOK(missile);
	INSTALL_HOOK(CHudSonicStageUpdateParallel);
	INSTALL_HOOK(CPlayerSpeedUpdateParallel);
	INSTALL_HOOK(MsgStartCommonButtonSign);
}