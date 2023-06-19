bool SUMouthFix;

float ringEnergyAmount = 2.0f;
size_t prevRingCount;

bool MoreVoice = Common::reader.GetBoolean("Changes", "MoreVoice", false);

bool EnergyChange = Common::reader.GetBoolean("Restorations", "EnergyChange", true);

bool InfLivesCodeChange = Common::reader.GetBoolean("Changes", "InfLives", true);

const char* lastAnimDashRing = "";

bool WaterIdle = Common::reader.GetBoolean("Restorations", "WaterIdle", true);

bool HomingX = Common::reader.GetBoolean("Restorations", "XHoming", false);

bool BDrift = Common::reader.GetBoolean("Restorations", "BDrift", true);

bool UseRingLife = Common::reader.GetBoolean("Restorations", "RingLife", true);

bool WaterDrift = Common::reader.GetBoolean("Restorations", "WaterDrift", true);

bool ringLife = false;

static float deltaTime;

HOOK(int, __fastcall, MiscRestart, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	int result = originalMiscRestart(This, Edx, message);
	prevRingCount = 0;
	if (UseRingLife) { ringLife = false; }
	return result;
}

void Misc::OnHUDUpdate(const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	// Ring Energy
	if (EnergyChange) {
		if (prevRingCount < sonic->m_RingCount) {
			if (sonic->m_ChaosEnergy < 100.0f) sonic->m_ChaosEnergy = std::clamp(sonic->m_ChaosEnergy + ringEnergyAmount, 0.0f, 100.0f);
			prevRingCount = sonic->m_RingCount;
		}
		else {
			if (prevRingCount > sonic->m_RingCount) {
				prevRingCount = sonic->m_RingCount;
			}
		}
	}
}

// Unleashed HUD
HOOK(void, __stdcall, MiscLife, 0xE75520, Sonic::Player::CPlayerContext* context, int lifeCount, bool playSound)
{
	if (lifeCount > 0)
	{
		Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

		if (sonic && BlueBlurCommon::IsModern() && MoreVoice) {
			sonic->PlaySound(2002499, true);
		}

		if (sonic->m_RingCount % 100 == 0 && sonic->m_RingCount != 0 && UseRingLife) {
			ringLife = true;
		}
	}
	originalMiscLife(context, lifeCount, playSound);
}

HOOK(void, __fastcall, MiscLifeRing, 0xE761A0, int a1) {
	if (!ringLife) { originalMiscLifeRing(a1); }
}
SharedPtrTypeless airBoostParticleHandle;
SharedPtrTypeless airBoostParticleHandle2;
float airBoostTimer = -1;
float _airBoostActiveTime = -1;
bool airBoostParticle = false;

bool water = false;

// brianuuu
HOOK(void, __fastcall, CSonicStateAirBoostBegin, 0x1233380, hh::fnd::CStateMachineBase::CStateBase* This)
{
	originalCSonicStateAirBoostBegin(This);

	auto* sonic = (Sonic::Player::CPlayerSpeedContext*)This->GetContextBase();
	_airBoostActiveTime = sonic->m_spParameter->Get<float>(Sonic::Player::ePlayerSpeedParameter_AirBoostTime) - Misc::airBoostActiveTime;
	airBoostParticle = false;
}

void Misc::OnUpdate(const hh::fnd::SUpdateInfo& updateInfo)
{
	deltaTime = updateInfo.DeltaTime;

	const size_t liveCountAddr = Common::GetMultiLevelAddress(0x1E66B34, { 0x4, 0x1B4, 0x7C, 0x9FDC });
	if (liveCountAddr) {
		if ((::byte)Common::GetMultiLevelAddress(0xD59A67, { 0x6 }) == 150 && *(int*)liveCountAddr != 99 && InfLivesCodeChange)
			*(int*)liveCountAddr = 99;
	}
	
	if (!BlueBlurCommon::IsModern()) { return; }

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Hedgehog::Base::CSharedString state = sonic->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

	if (HomingX && sonic->m_spParameter->Get<bool>(Sonic::Player::ePlayerSpeedParameter_XButtonHoming) != true) {
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_XButtonHoming] = true;
	}

	if (!Common::GetSonicStateFlags()->OnWater && water) {
		water = false;
		sonic->StateFlag(eStateFlag_DisableGroundSmoke) = false;
	}


	if (Misc::fadeOutAirBoost) {
		if (_airBoostActiveTime > -1) {
			_airBoostActiveTime -= updateInfo.DeltaTime;
			if (_airBoostActiveTime <= 0 && !airBoostParticle) {
				if (input.IsDown(Sonic::eKeyState_X)) {
					void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
					Common::fCGlitterCreate(sonic, airBoostParticleHandle, middlematrixNode, (BlueBlurCommon::IsSuper() ? "ef_ch_sps_yh1_boost1" : "ef_ch_sng_yh1_boost1"), 0);
					Common::fCGlitterCreate(sonic, airBoostParticleHandle2, middlematrixNode, (BlueBlurCommon::IsSuper() ? "ef_ch_sps_yh1_boost2" : "ef_ch_sng_yh1_boost2"), 0);
				}
				airBoostTimer = Misc::airBoostEndTime;
				airBoostParticle = true;
			}
		}

		if (airBoostTimer > -1) {
			airBoostTimer -= updateInfo.DeltaTime;
			if (airBoostTimer <= 0) {
				Common::fCGlitterEnd(sonic, airBoostParticleHandle, false);
				Common::fCGlitterEnd(sonic, airBoostParticleHandle2, false);
				airBoostTimer = -1;
			}
		}
	}

	if (anim == "IdleInWater" && WaterIdle) {
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
			}
		}
	}
	else {
		if (lastAnimDashRing != "") {
			lastAnimDashRing = "";
		}
	}

	if (sonic->m_ChaosEnergy != 100 && Common::CheckCurrentStage("pam000")) // For Boost Gauge Starts Empty Code
		sonic->m_ChaosEnergy = 100;

	// B Button Drift
	if (BDrift && abs(sonic->m_Velocity.norm()) >= 30.0f && !sonic->m_Is2DMode && !Common::IsPlayerInForwardPath() && !sonic->StateFlag(eStateFlag_OutOfControl) && !strstr(anim.c_str(), "Grind")) {
		// Regular Drift
		if (input.IsDown(Sonic::eKeyState_B) && abs(input.LeftStickHorizontal) >= 0.85f && (state == "Walk" || state == "Sliding")) {
			sonic->ChangeState("Drift");
		}
		else if (input.IsUp(Sonic::eKeyState_B) && state == "Drift" && input.IsUp(Sonic::eKeyState_LeftTrigger) && input.IsUp(Sonic::eKeyState_RightTrigger)) {
			sonic->ChangeState("Walk");
		}
	}
}

bool driftParticle = false;
SharedPtrTypeless driftParticleHandle;
// Skyth (Unleashed Drift) & Briannu (06 Experience)
HOOK(void, __fastcall, CSonicStatePluginOnWaterUpdate, 0x119BED0, Hedgehog::Universe::TStateMachine<Sonic::Player::CPlayerSpeedContext>::TState* This)
{
	originalCSonicStatePluginOnWaterUpdate(This);
	if (!BlueBlurCommon::IsModern()) { return; }
	if (!WaterDrift) { return; }

	Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
	Hedgehog::Base::CSharedString state = sonic->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();

	if (!water) {
		water = true;
		sonic->StateFlag(eStateFlag_DisableGroundSmoke) = true;
	}

	if (sonic->StateFlag(eStateFlag_OnWater) && state == "Drift" && !sonic->StateFlag(eStateFlag_Boost))
	{
		if (!driftParticle) {
			driftParticle = true;
			auto RefNode = sonic->m_pPlayer->m_spCharacterModel->GetNode("Reference");
			Common::fCGlitterCreate(sonic, driftParticleHandle, &RefNode, "ef_ch_sns_yh1_watersplash2", 0);
		}

		if (sonic->m_Velocity.norm() >= 20.0f) {
			sonic->m_Velocity *= 1.0f - deltaTime * 0.7f;
			Common::GetSonicStateFlags()->AcceptBuoyancyForce = true;
			WRITE_NOP(0x119C0E5, 2); // float even if speed is 0
			WRITE_NOP(0xDED132, 3);  // don't reset AcceptBuoyancyForce
			WRITE_MEMORY(0xDFB98A, uint8_t, 0x90, 0x90, 0xF3, 0x0F, 0x10, 0x05, 0x00,   // always use BuoyantForceMaxGravityRate
				0xB5, 0x58, 0x01, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);      // and set it as 10.0f
			WRITE_MEMORY(0x119C00E, uint8_t, 0x0F, 0x57, 0xC0, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90); // Set WaterDecreaseForce = 0
		}
		else {
			WRITE_MEMORY(0x119C0E5, uint8_t, 0x76, 0x59);
			WRITE_MEMORY(0xDED132, uint8_t, 0x88, 0x59, 0x59);
			WRITE_MEMORY(0xDFB98A, uint8_t, 0x74, 0x3A, 0x8B, 0x86, 0x7C, 0x02, 0x00,
				0x00, 0x68, 0xA6, 0x00, 0x00, 0x00, 0xE8, 0x54, 0xF0, 0x73, 0xFF);
			WRITE_MEMORY(0x119C00E, uint8_t, 0x68, 0xA7, 0x00, 0x00, 0x00, 0xE8, 0xD8, 0xE9, 0x39, 0xFF);
		}
	}
	else
	{
		if (driftParticle) {
			driftParticle = false;
			Common::fCGlitterEnd(sonic, driftParticleHandle, false);
		}
		WRITE_MEMORY(0x119C0E5, uint8_t, 0x76, 0x59);
		WRITE_MEMORY(0xDED132, uint8_t, 0x88, 0x59, 0x59);
		WRITE_MEMORY(0xDFB98A, uint8_t, 0x74, 0x3A, 0x8B, 0x86, 0x7C, 0x02, 0x00,
			0x00, 0x68, 0xA6, 0x00, 0x00, 0x00, 0xE8, 0x54, 0xF0, 0x73, 0xFF);
		WRITE_MEMORY(0x119C00E, uint8_t, 0x68, 0xA7, 0x00, 0x00, 0x00, 0xE8, 0xD8, 0xE9, 0x39, 0xFF);
	}
}
// https://github.com/brianuuu/DllMods/blob/master/Source/NavigationLightdashSound/NavigationSound.cpp#L14
HOOK(void, __fastcall, MiscYPrompt, 0x5289A0, void* thisDeclaration, void* edx, uint32_t a2)
{
	// Disable Y button prompt.
	if (*(uint32_t*)(a2 + 16) == 3)
		return;

	originalMiscYPrompt(thisDeclaration, edx, a2);
}

std::vector<std::string> SUModelMods = { "Chip Bracelet (Unleashed)", "Pure SU Sonic", "SU Marza Sonic", "Unleashed Sonic Model"};

bool DisableBoard = Common::reader.GetBoolean("Changes", "DisableBoard", false);

//Patch "Never Receive Boost From Rings" in "Gameplay" by "Hyper"
void NOP(int floatInstrAddr, int paramStrAddr)
{
	WRITE_MEMORY(floatInstrAddr, ::byte, 0xD9, 0xEE); /* fldz */
	WRITE_NOP(floatInstrAddr + 2, 6);
	WRITE_MEMORY(paramStrAddr, ::byte, 0x00);
}

void Misc::Install()
{

	Misc::fadeOutAirBoost = Common::reader.GetBoolean("Restorations", "AirBoostFade", true);
	Misc::airBoostActiveTime = 0.3f;
	Misc::airBoostEndTime = 0.3f;

	if (Common::reader.GetBoolean("Restorations", "AirBoostFade", true)) {
		INSTALL_HOOK(CSonicStateAirBoostBegin);
	}

	INSTALL_HOOK(CSonicStatePluginOnWaterUpdate);

	if (EnergyChange) {
		NOP(0x120628B, 0x15FA690); /* ChaosEnergyRecoverRateByRing */
		NOP(0x1206335, 0x15FA6DC); /* ChaosEnergyRecoverRateByRingBonus */
		NOP(0x12063DF, 0x15FA72C); /* ChaosEnergyRecoverRateByRingPenalty */
	}

	// Unwiished Fall
	if (Common::reader.GetBoolean("Changes", "WiiFall", false)) {
		// Thanks Hyper.
		WRITE_MEMORY(0x111913A, uint8_t, 0x72, 0x27);
		WRITE_STRING(0x15E812C, "sn_wall_fly02_loop");
	}

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
		WRITE_MEMORY(0xD947CC, ::byte, 0xEB);
		WRITE_MEMORY(0xE6B750, ::byte, 0xC2, 0x04, 0x00);
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

	// https://github.com/brianuuu/DllMods/blob/master/Source/Sonic06DefinitiveExperience/NextGenSonic.cpp
	if(Common::reader.GetBoolean("Restorations", "DPadDisable", false))
		WRITE_JUMP(0xD97B56, (void*)0xD97B9E); // Disable D-Pad Input

	INSTALL_HOOK(MiscYPrompt);
	INSTALL_HOOK(MiscRestart);

	INSTALL_HOOK(MiscLife);
	if (UseRingLife) { INSTALL_HOOK(MiscLifeRing); }
}