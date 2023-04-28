int BPressed = 0;
float BResetTimer = -1;
float sweepkickColTime = 0.0f;
SharedPtrTypeless squatKickParticleHandle;
Hedgehog::Math::CQuaternion squatKickRotation;

hh::base::CRefPtr<Sonic::CLocalLight> sweepLight;
float sweepLightAlpha = 0.0f;
float desiredSweepLightAlpha = 0.0f;
float sweepLightAlphaSpeed = 0.0f;
float sweepLightTime = -1;

INIReader sweepIni;
bool customSweepIniFound = false;

float sweepLightDelay = -1;

bool sweepKickActive = false;

Hedgehog::Math::CVector GetSweepOffset() {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	Hedgehog::Math::CVector playerRight = sonic->m_spMatrixNode->m_Transform.m_Rotation * Hedgehog::Math::CVector::UnitX();
	Hedgehog::Math::CVector playerUp = sonic->m_spMatrixNode->m_Transform.m_Rotation * Hedgehog::Math::CVector::UnitY();
	Hedgehog::Math::CVector playerForward = sonic->m_spMatrixNode->m_Transform.m_Rotation * Hedgehog::Math::CVector::UnitZ();

	Hedgehog::Math::CVector offset = playerRight * Sweepkick::offsetX +
		playerUp * Sweepkick::offsetY +
		playerForward * Sweepkick::offsetZ;

	return offset;
}

void SweepLightFunc() {
	sweepLight = nullptr;

	Hedgehog::Base::TSynchronizedPtr gameDoc = Sonic::CGameDocument::GetInstance();

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Hedgehog::Math::CVector position = sonic->m_spMatrixNode->m_Transform.m_Position + GetSweepOffset();

	sweepLight = gameDoc->m_pMember->m_spLightManager->AddLocalLight(position, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, Sweepkick::lightRange });
}

HOOK(void, __fastcall, CSonicStateSquatKickBegin, 0x12526D0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	Sonic::Player::CPlayerSpeedContext* context = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;

	auto RefNode = context->m_pPlayer->m_spCharacterModel->GetNode("Reference");
	Common::fCGlitterCreate(context, squatKickParticleHandle, &RefNode, "Sweepkick", 1);

	context->PlaySound(2002497, true);

	sweepkickColTime = Sweepkick::supportShockwaveDelay;

	if (Sweepkick::useLight) {
		sweepLightDelay = Sweepkick::lightInDelay;

		sweepLightTime = Sweepkick::lightLifeTime;
		sweepLightAlphaSpeed = Sweepkick::lerpSpeedIn;

		SweepLightFunc();
	}

	originalCSonicStateSquatKickBegin(This);
}

HOOK(void, __fastcall, CSonicStateSquatKickAdvance, 0x1252810, hh::fnd::CStateMachineBase::CStateBase* This) {
	auto* context = (Sonic::Player::CPlayerSpeedContext*)This->GetContextBase();
	Hedgehog::Math::CVector pos = context->m_spMatrixNode->m_Transform.m_Position;
	if (sweepkickColTime <= 0 && Sweepkick::useSupportShockwave) { Common::CreatePlayerSupportShockWave(pos, 0.15f, 3.5f, 0.1f); }
	originalCSonicStateSquatKickAdvance(This);
}

// Original Code by Briannu
bool __fastcall CSonicStateSquatKickAdvanceTransitionOutImpl(char const* name)
{
	if (strcmp(name, "Stand") == 0 && Sweepkick::transitionToStand)
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

float lightColor[3] = {}; // RGBA (A doesn't do anything so we lerp to black)

HOOK(void, __fastcall, SonicUpdateSweep, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicUpdateSweep(This, _, updateInfo);
	if (BlueBlurCommon::IsModern()) {
		if (sweepkickColTime > 0) sweepkickColTime -= updateInfo.DeltaTime;
		Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
		Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
		Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

		if (input.IsTapped(Sonic::eKeyState_B)) {
			if (BResetTimer == -1) {
				BResetTimer = Sweepkick::sweepInputTime;
			}
			BPressed++;
			if (BPressed == 2) {
				BPressed = 0;
				if (BResetTimer > 0) {
					bool canSquatKick = (state == "Squat" || state == "Sliding" || state == "Walk" || state == "SlidingEnd" || state == "StompingLand" || state == "SquatCharge") && !Common::IsPlayerControlLocked() && !BlueBlurCommon::IsSuper();
					if (canSquatKick) {
						squatKickRotation = sonic->m_spMatrixNode->m_Transform.m_Rotation;
						sonic->ChangeState("SquatKick");
						sweepKickActive = true;
					}
				}
			}
		}

		if (sweepLight) {
			if (sweepLightDelay != -1) {
				sweepLightDelay -= updateInfo.DeltaTime;
				if (sweepLightDelay <= 0) {
					sweepLightDelay = -1;
					desiredSweepLightAlpha = 1.0f;
				}
			}

			sweepLightAlpha = Common::Lerp(sweepLightAlpha, desiredSweepLightAlpha, updateInfo.DeltaTime * sweepLightAlphaSpeed);
			lightColor[0] = Common::Lerp(0.0f, Sweepkick::colorR, sweepLightAlpha);
			lightColor[1] = Common::Lerp(0.0f, Sweepkick::colorG, sweepLightAlpha);
			lightColor[2] = Common::Lerp(0.0f, Sweepkick::colorB, sweepLightAlpha);
			lightColor[3] = 1.0f;
			sweepLight->m_spLight->m_Color = { lightColor[0], lightColor[1], lightColor[2], lightColor[3] };

			Hedgehog::Math::CVector position = sonic->m_spMatrixNode->m_Transform.m_Position + GetSweepOffset();
			sweepLight->SetPosition(position);

			if (!Sweepkick::useLight) {
				sweepLight = nullptr;
			}
		}

		if (state == "SquatKick" || sweepKickActive) {
			if (sonic->m_Velocity.norm() == 0.0f) { sonic->m_spMatrixNode->m_Transform.SetRotation(squatKickRotation); }
			if (Sweepkick::useLight) {
				if (sweepLightTime != -1) {
					sweepLightTime -= updateInfo.DeltaTime;
					if (sweepLightTime <= 0) {
						sweepLightTime = -1;
						desiredSweepLightAlpha = 0.0f;
						sweepLightAlphaSpeed = Sweepkick::lerpSpeedOut;
					}
				}
			}
		}
		if (sweepKickActive && (state != "Squat" && state != "Sliding" && state != "Walk" && state != "SlidingEnd" && state != "StompingLand" && state != "SquatCharge" && state != "SquatKick")) {
			Common::fCGlitterEnd(BlueBlurCommon::GetContext(), squatKickParticleHandle, true);
			sweepKickActive = false;

			if (Sweepkick::useLight) {
				desiredSweepLightAlpha = 0.0f;
				sweepLightAlpha = 0.0f;
			}
		}

		if (BResetTimer != -1) {
			BResetTimer -= updateInfo.DeltaTime;
			if (BResetTimer <= 0) {
				BResetTimer = -1;
				BPressed = 0;
			}
		}
	}
}

void Sweepkick::Install() {

	SweepConfig();

	//Original code by Brianuu
	static double const c_sweepKickActivateTime = sweepIni.GetFloat("Gameplay", "SweepKickActivateTime", 0.0f);
	WRITE_MEMORY(0x125299E, double*, &c_sweepKickActivateTime);

	if (sweepIni.GetBoolean("Gameplay", "UseSlideCollision", true)) {
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
	}

	Sweepkick::useLight = Common::reader.GetBoolean("Changes", "SweepLight", false);

	Sweepkick::transitionToStand = sweepIni.GetBoolean("Gameplay", "TransitionToStand", true);
	WRITE_JUMP(0x125291F, CSonicStateSquatKickAdvanceTransitionOut);

	Sweepkick::useSupportShockwave = sweepIni.GetBoolean("Gameplay", "UseSupportShockWave", true);

	Sweepkick::supportShockwaveDelay = sweepIni.GetFloat("Gameplay", "SupportShockWaveDelay", 0.2f);
	Sweepkick::lightInDelay = sweepIni.GetFloat("Timing", "LightInDelay", 0.2f);
	Sweepkick::lightLifeTime = sweepIni.GetFloat("Timing", "LightLifeTime", 0.5f);
	Sweepkick::lerpSpeedIn = sweepIni.GetFloat("Timing", "LerpSpeedIn", 10.0f);
	Sweepkick::lightRange = sweepIni.GetFloat("Range", "LightRange", 1.5f);

	Sweepkick::offsetX = sweepIni.GetFloat("Position", "OffsetX", 0.0f);
	Sweepkick::offsetY = sweepIni.GetFloat("Position", "OffsetY", 0.6f);
	Sweepkick::offsetZ = sweepIni.GetFloat("Position", "OffsetZ", 0.0f);

	Sweepkick::lerpSpeedOut = sweepIni.GetFloat("Timing", "LerpSpeedOut", 5.0f);

	Sweepkick::colorR = sweepIni.GetFloat("Color", "ColorR", 0.0f);
	Sweepkick::colorG = sweepIni.GetFloat("Color", "ColorG", 0.4f);
	Sweepkick::colorB = sweepIni.GetFloat("Color", "ColorB", 1.0f);

	Sweepkick::sweepInputTime = 0.3f;

	INSTALL_HOOK(SonicUpdateSweep);
	INSTALL_HOOK(CSonicStateSquatKickBegin);
	INSTALL_HOOK(CSonicStateSquatKickAdvance);
}