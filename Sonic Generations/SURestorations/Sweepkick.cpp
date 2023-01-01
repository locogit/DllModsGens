int BPressed = 0;
bool BResetTimerEnable = false;
float BResetTimer;
float sweepkickColTime = 0.0f;
SharedPtrTypeless squatKickParticleHandle;
Hedgehog::Math::CQuaternion squatKickRotation;

hh::base::CRefPtr<Sonic::CLocalLight> sweepLight = nullptr;
float sweepLightAlpha = 0.0f;
float desiredSweepLightAlpha = 0.0f;
float sweepLightAlphaSpeed = 0.0f;
float sweepLightTime = 0.0f;

INIReader sweepIni;
bool customSweepIniFound = false;

bool sweepLightBool = Common::reader.GetBoolean("Changes", "SweepLight", false);

Hedgehog::Math::CVector GetSweepOffset() {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	Hedgehog::Math::CVector playerRight = sonic->m_spMatrixNode->m_Transform.m_Rotation * Hedgehog::Math::CVector::UnitX();
	Hedgehog::Math::CVector playerUp = sonic->m_spMatrixNode->m_Transform.m_Rotation * Hedgehog::Math::CVector::UnitY();
	Hedgehog::Math::CVector playerForward = sonic->m_spMatrixNode->m_Transform.m_Rotation * Hedgehog::Math::CVector::UnitZ();

	Hedgehog::Math::CVector offset = playerRight * sweepIni.GetFloat("Position", "OffsetX", 0) + playerUp * sweepIni.GetFloat("Position", "OffsetY", 0) + playerForward * sweepIni.GetFloat("Position", "OffsetZ", 0);

	return offset;
}

void SweepLightFunc() {
	if (sweepLight != nullptr) { return; }
	Hedgehog::Base::TSynchronizedPtr gameDoc = Sonic::CGameDocument::GetInstance();

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Hedgehog::Math::CVector position = sonic->m_spMatrixNode->m_Transform.m_Position + GetSweepOffset();

	sweepLight = gameDoc->m_pMember->m_spLightManager->AddLocalLight(position, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, sweepIni.GetFloat("Range", "LightRange", 0) });
}

HOOK(void, __fastcall, CSonicStateSquatKickBegin, 0x12526D0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	Sonic::Player::CPlayerSpeedContext* context = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;

	void* middlematrixNode = (void*)((uint32_t)context + 0x10);
	Common::fCGlitterCreate(context, squatKickParticleHandle, middlematrixNode, "Sweepkick", 1);

	context->PlaySound(2002497, true);

	sweepkickColTime = 0.2f;

	sweepLightTime = sweepIni.GetFloat("Timing", "LightLifeTime", 0);
	desiredSweepLightAlpha = 1.0f;
	sweepLightAlphaSpeed = sweepIni.GetFloat("Timing", "LerpSpeedIn", 0);

	SweepLightFunc();

	originalCSonicStateSquatKickBegin(This);
}

HOOK(void, __fastcall, CSonicStateSquatKickAdvance, 0x1252810, hh::fnd::CStateMachineBase::CStateBase* This) {
	auto* context = (Sonic::Player::CPlayerSpeedContext*)This->GetContextBase();
	Hedgehog::Math::CVector pos = context->m_spMatrixNode->m_Transform.m_Position;
	if (sweepkickColTime <= 0) Common::CreatePlayerSupportShockWave(pos, 0.5f, 2.5f, 0.1f);
	originalCSonicStateSquatKickAdvance(This);
}

HOOK(void, __fastcall, CSonicStateSquatKickEnd, 0x12527B0, void* This)
{
	Common::fCGlitterEnd(BlueBlurCommon::GetContext(), squatKickParticleHandle, false);

	desiredSweepLightAlpha = 0.0f;
	sweepLightAlpha = 0.0f;

	originalCSonicStateSquatKickEnd(This);
}

// Original Code by Briannu
bool __fastcall CSonicStateSquatKickAdvanceTransitionOutImpl(char const* name)
{
	if (strcmp(name, "Stand") == 0)
	{
		Eigen::Vector3f inputDirection;
		if (Common::GetWorldInputDirection(inputDirection) && inputDirection.isZero())
		{
			BlueBlurCommon::GetContext()->ChangeState("Squat");
			BlueBlurCommon::GetContext()->ChangeAnimation("SquatToStand");
			return true;
		}
	}

	return false;
}

void __declspec(naked) CSonicStateSquatKickAdvanceTransitionOut()
{
	static uint32_t returnAddress = 0x1252924;
	static uint32_t sub_E4FF30 = 0xE4FF30;
	__asm
	{
		push    eax
		push    ecx

		mov     ecx, [eax]
		call    CSonicStateSquatKickAdvanceTransitionOutImpl
		mov     bl, al

		pop     ecx
		pop     eax

		test    bl, bl
		jnz     jump
		call[sub_E4FF30]

		jump:
		jmp[returnAddress]
	}
}

void ReadSweepConfig(std::string path = "") {
	sweepIni = path.empty() ? "sur_sweepkick.ini" : path;
}

// god bless hyper for doing this for score gens
void SweepConfig() {
	std::string modsDatabase = Common::GetModsDatabase();

	if (Common::FileExists(modsDatabase))
	{
		INIReader reader(modsDatabase);

		bool reverseLoadOrder;
		{
			// Used to flip the for loop direction.
			reverseLoadOrder = reader.GetBoolean("Main", "ReverseLoadOrder", 0);
		}

		int count;
		{
			// Used for iterating the for loop.
			count = reader.GetInteger("Main", "ActiveModCount", 0);
		}

		// (reverseLoadOrder == true)  = for (int i = 0; i < count; i++)
		// (reverseLoadOrder == false) = for (int i = count - 1; i + 1 > 0; i--)
		for (int i = reverseLoadOrder ? 0 : count - 1; reverseLoadOrder ? i < count : i + 1 > 0; reverseLoadOrder ? i++ : i--)
		{
			std::string guid;
			{
				guid = reader.Get("Main", "ActiveMod" + std::to_string(i), "");
			}

			std::string config;
			{
				config = reader.Get("Mods", guid, "");
			}

			std::string sweepConfig = config.substr(0, config.find_last_of("\\")) + "\\" + "sur_sweepkick.ini";
			if (Common::FileExists(sweepConfig))
			{
				if (!Common::FileExists(config.substr(0, config.find_last_of("\\")) + "\\" + "SURestorations.dll")) // make sure mod is not self
				{ 
					ReadSweepConfig(sweepConfig);
					customSweepIniFound = true;
				}
			}
		}
		if (!customSweepIniFound) {
			reverseLoadOrder = !reverseLoadOrder;
			for (int i = reverseLoadOrder ? 0 : count - 1; reverseLoadOrder ? i < count : i + 1 > 0; reverseLoadOrder ? i++ : i--)
			{
				std::string guid;
				{
					guid = reader.Get("Main", "ActiveMod" + std::to_string(i), "");
				}

				std::string config;
				{
					config = reader.Get("Mods", guid, "");
				}

				std::string sweepConfig = config.substr(0, config.find_last_of("\\")) + "\\" + "sur_sweepkick.ini";
				if (Common::FileExists(sweepConfig))
				{
					if (!Common::FileExists(config.substr(0, config.find_last_of("\\")) + "\\" + "SURestorations.dll")) // make sure mod is not self
					{
						ReadSweepConfig(sweepConfig);
						customSweepIniFound = true;
					}
				}
			}
		}
		if (!customSweepIniFound) {
			ReadSweepConfig("");
		}
		
	}
}

HOOK(void, __fastcall, SonicUpdateSweep, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicUpdateSweep(This, _, updateInfo);
	if (BlueBlurCommon::IsModern()) {
		if (sweepkickColTime > 0) sweepkickColTime -= updateInfo.DeltaTime;
		Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
		Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
		Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

		if (input.IsTapped(Sonic::eKeyState_B)) {
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

		if (sweepLight != nullptr) {
			sweepLightAlpha = Common::Lerp(sweepLightAlpha, desiredSweepLightAlpha, updateInfo.DeltaTime * sweepLightAlphaSpeed);

			float lightColor[3] = {}; // RGBA (A doesn't do anything so we lerp to black)
			lightColor[0] = sweepLightBool ? Common::Lerp(0.0f, sweepIni.GetFloat("Color", "ColorR", 0.0f), sweepLightAlpha) : 0.0f;
			lightColor[1] = sweepLightBool ? Common::Lerp(0.0f, sweepIni.GetFloat("Color", "ColorG", 0.0f), sweepLightAlpha) : 0.0f;
			lightColor[2] = sweepLightBool ? Common::Lerp(0.0f, sweepIni.GetFloat("Color", "ColorB", 0.0f), sweepLightAlpha) : 0.0f;
			lightColor[3] = 1.0f;

			sweepLight->m_spLight->m_Color = { lightColor[0], lightColor[1], lightColor[2], lightColor[3] };

			Hedgehog::Math::CVector position = sonic->m_spMatrixNode->m_Transform.m_Position + GetSweepOffset();
			sweepLight->SetPosition(position);
		}

		if (state == "SquatKick") {
			if (sweepLightTime > 0) { sweepLightTime -= updateInfo.DeltaTime; }
			if (sweepLightTime <= 0) {
				desiredSweepLightAlpha = 0.0f;
				sweepLightAlphaSpeed = sweepIni.GetFloat("Timing", "LerpSpeedOut", 0);
			}

			if (sonic->m_Velocity.norm() == 0.0f) { sonic->m_spMatrixNode->m_Transform.SetRotation(squatKickRotation); }
		}

		if (BResetTimerEnable) {
			if (BResetTimer > 0) BResetTimer -= updateInfo.DeltaTime;
			if (BResetTimer <= 0) {
				BPressed = 0;
				BResetTimerEnable = false;
			}
		}
	}
}

void Sweepkick::Install() {

	SweepConfig();

	//Original code by Brianuu
	static double const c_sweepKickActivateTime = 0.0f;
	WRITE_MEMORY(0x125299E, double*, &c_sweepKickActivateTime);

	// Change SquatKick's collision the same as sliding
	WRITE_MEMORY(0xDFCD6D, uint8_t, 0x5); // switch 6 cases
	static uint32_t const collisionSwitchTable[6] =
	{
		0xDFCD7B, // normal
		0xDFCDC0, // slide
		0xDFCD7B, // boost
		0xDFCD7B,
		0xDFCDFA, // unused
		0xDFCDC0  // squat kick
	};
	WRITE_MEMORY(0xDFCD77, uint32_t*, collisionSwitchTable);

	WRITE_JUMP(0x125291F, CSonicStateSquatKickAdvanceTransitionOut);

	INSTALL_HOOK(SonicUpdateSweep);
	INSTALL_HOOK(CSonicStateSquatKickBegin);
	INSTALL_HOOK(CSonicStateSquatKickAdvance);
	INSTALL_HOOK(CSonicStateSquatKickEnd);
}