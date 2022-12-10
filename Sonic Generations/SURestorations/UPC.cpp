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
float soundTime;
bool sound = false;
bool paraloop = false;

SharedPtrTypeless paraloopHandle;
struct ParaloopInfo {
	std::vector<float> startPos = { 0, 0, 0 };
	float distanceThreshold = 3.0f;
	float duration = 2.0f;
	float soundDelay = 2.0f;
	bool condition;
	void Update(Sonic::Player::CPlayerSpeedContext* sonic) {
		float dist = (sonic->m_spMatrixNode->m_Transform.m_Position - Hedgehog::Math::CVector(startPos[0], startPos[1], startPos[2])).norm();
		dist = abs(dist);
		if (dist <= distanceThreshold && condition) {
			void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
			Common::fCGlitterCreate(sonic, paraloopHandle, middlematrixNode, (BlueBlurCommon::IsSuper() ? "ss_paraloop" : "paraloop"), 0);
			paraloop = true;
			sound = true;
			paraloopTime = duration;
			soundTime = soundDelay;
		}
	}
};

void Paraloop(Sonic::Player::CPlayerSpeedContext* sonic) {

	if (paraloop) return;

	Hedgehog::Math::CVector pos = sonic->m_spMatrixNode->m_Transform.m_Position;

	// Mazuri
	if (Common::CheckCurrentStage("cpz200")) {
		{
			ParaloopInfo info;
			info.startPos = { 666.0f, -129.898f, 1364.824f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 2.0f;
			info.soundDelay = 1.0f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 697.287f, -92.5f, 1174.389f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 1.35f;
			info.soundDelay = 0.8f;
			info.Update(sonic);
		}

		// Paraloop (Rail)
		{
			ParaloopInfo info;
			info.startPos = { -5.615f, -53.5f, 449.415f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 1.5f;
			info.soundDelay = 1.0f;
			info.Update(sonic);
		}
	}
	else if (Common::CheckCurrentStage("euc200")) { // Abadat
		{
			ParaloopInfo info;
			info.startPos = { -1.464, 18.020f, -1502.584f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 1.75f;
			info.soundDelay = 0.0f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { -1.468f, 7.859f, -1517.009f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 1.5f;
			info.soundDelay = 0.0f;
			info.Update(sonic);
		}
	}
	else if (Common::CheckCurrentStage("ssh200")) { // Shamar
		{
			ParaloopInfo info;
			info.startPos = { -698.696f, 90.0f, 50.010f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 1.3f;
			info.soundDelay = 1.3f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 868.926f, 26.941f, 171.875f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 1.0f;
			info.soundDelay = 1.0f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { -22.0f, 63.0f, 5.372f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 1.65f;
			info.soundDelay = 1.65f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 1097.56f, -61.0f, 162.0f };
			info.distanceThreshold = 2.0f;
			info.condition = true;
			info.duration = 0.9f;
			info.soundDelay = 0.9f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 2162.285f, -85.735f, -326.38f };
			info.distanceThreshold = 2.5f;
			info.condition = true;
			info.duration = 1.35f;
			info.soundDelay = 1.35f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 2153.9f, -96.707f, -461.3f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 1.0f;
			info.soundDelay = 1.0f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 2222.497f, -172.588f, -1110.001f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 1.35f;
			info.soundDelay = 1.35f;
			info.Update(sonic);
		}
	}
	else if (Common::CheckCurrentStage("sph200")) { // Chun-Nan
		{
			ParaloopInfo info;
			info.startPos = { -508.074f, 46.021f, 206.315f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 1.2f;
			info.soundDelay = 1.2f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 561.526f, 20.047f, -1449.837f };
			info.distanceThreshold = 1.0f;
			info.condition = Common::IsPlayerIn2D();
			info.duration = 1.4f;
			info.soundDelay = 1.4f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 743.021f, -10.195f, -1490.299f };
			info.distanceThreshold = 1.0f;
			info.condition = true;
			info.duration = 1.5f;
			info.soundDelay = 1.5f;
			info.Update(sonic);
		}
	}
}
HOOK(void, __fastcall, SonicAddonUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicAddonUpdate(This, _, updateInfo);
	if (BlueBlurCommon::IsModern()) {
		Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
		Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
		Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

		Paraloop(sonic);

		if (paraloopTime > 0 && paraloop) paraloopTime -= updateInfo.DeltaTime;
		if (paraloopTime <= 0 && paraloop) {
			Common::fCGlitterEnd(sonic, paraloopHandle, true);
			paraloopHandle = nullptr;
			paraloop = false;
		}
		if (soundTime > 0 && sound) soundTime -= updateInfo.DeltaTime;
		if (soundTime <= 0 && sound) {
			sonic->PlaySound(2002501, true);
			sound = false;
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