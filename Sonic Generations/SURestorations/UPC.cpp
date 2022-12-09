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

bool bobsleighBoostCancel = false;

float paraloopTime;
bool paraloop = false;

struct ParaloopInfo {
	std::vector<float> startPos = { 0, 0, 0 };
	float distanceThreshold = 3.0f;
	float duration = 2.0f;
	bool condition;
	void Update(Hedgehog::Math::CVector playerPos) {
		float dist = (playerPos - Hedgehog::Math::CVector(startPos[0], startPos[1], startPos[2])).norm();
		dist = abs(dist);
		if (dist <= distanceThreshold && condition) {
			MessageBeep(MB_ICONINFORMATION);
			paraloop = true;
			paraloopTime = duration;
		}
	}
};

void Paraloop(Sonic::Player::CPlayerSpeedContext* sonic) {

	if (paraloop) return;

	// Mazuri
	if (Common::CheckCurrentStage("cpz200")) {
		Hedgehog::Math::CVector pos = sonic->m_spMatrixNode->m_Transform.m_Position;
		{
			ParaloopInfo info;
			info.startPos = { 666.0f, -129.898f, 1364.824f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 1.0f;
			info.Update(pos);
		}

		{
			ParaloopInfo info;
			info.startPos = { 697.287f, -92.5f, 1174.389f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 1.0f;
			info.Update(pos);
		}

		// Paraloop (Rail)
		{
			ParaloopInfo info;
			info.startPos = { -5.615f, -53.5f, 449.415f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 1.0f;
			info.Update(pos);
		}
	}
}
HOOK(void, __fastcall, SonicAddonUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicAddonUpdate(This, _, updateInfo);
	if (BlueBlurCommon::IsModern()) {
		Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
		Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();

		Paraloop(sonic);

		if (paraloopTime > 0 && paraloop) paraloopTime -= updateInfo.DeltaTime;
		if (paraloopTime <= 0 && paraloop) {
			paraloop = false;
			MessageBeep(MB_ICONERROR);
		}

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
}
void UPC::Install() {
	// Credit to Skyth
	WRITE_MEMORY(0xDFF622, ::byte, 0xEB); // Disables Drifting when using bobsleigh

	// Speed Highway Rocket Explosion
	WRITE_MEMORY(0x164D90C, char, "fireworks");

	INSTALL_HOOK(SonicAddonUpdate);
}