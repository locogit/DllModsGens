bool SUMouthFix;

float ringEnergyAmount = 2.0f;
size_t prevRingCount;
float previousChaosEnergy;

bool MoreVoice = Common::reader.GetBoolean("Changes", "MoreVoice", false);

bool EnergyChange = Common::reader.GetBoolean("Restorations", "EnergyChange", true);

bool InfLivesCodeChange = Common::reader.GetBoolean("Changes", "InfLives", true);

const char* lastAnimDashRing = "";

bool WaterIdle = Common::reader.GetBoolean("Restorations", "WaterIdle", true);

bool HomingX = Common::reader.GetBoolean("Restorations", "XHoming", true);

HOOK(int, __fastcall, MiscRestart, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	int result = originalMiscRestart(This, Edx, message);
	prevRingCount = 0;
	return result;
}
HOOK(void, __fastcall, SonicMiscUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	if ((byte)Common::GetMultiLevelAddress(0xD59A67, { 0x6 }) == 150 && *Common::GetPlayerLives() != 99 && InfLivesCodeChange)
		*Common::GetPlayerLives() = 99;

	originalSonicMiscUpdate(This, _, updateInfo);

	if (BlueBlurCommon::IsModern()) {

		Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
		Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
		Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();

		if (HomingX && sonic->m_spParameter->Get<bool>(Sonic::Player::ePlayerSpeedParameter_XButtonHoming) != true) {
			sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_XButtonHoming] = true;
		}

		if (EnergyChange) {
			if (prevRingCount < sonic->m_RingCount) {
				if (sonic->m_ChaosEnergy < 100.0f && previousChaosEnergy + ringEnergyAmount < 100.0f)
					sonic->m_ChaosEnergy = max(previousChaosEnergy + ringEnergyAmount, 0);
				prevRingCount = sonic->m_RingCount;
			}
			else {
				if (prevRingCount > sonic->m_RingCount) {
					prevRingCount = sonic->m_RingCount;
				}
				previousChaosEnergy = sonic->m_ChaosEnergy;
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

// Unleashed HUD
HOOK(void, __stdcall, MiscLife, 0xE75520, Sonic::Player::CPlayerContext* context, int lifeCount, bool playSound)
{
	originalMiscLife(context, lifeCount, playSound);

	if (lifeCount > 0)
	{
		Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		if (sonic && BlueBlurCommon::IsModern()) {
			sonic->PlaySound(2002499, true);
		}
	}
}
std::vector<std::string> SUModelMods = { "Chip Bracelet (Unleashed)", "Pure SU Sonic", "SU Marza Sonic", "Unleashed Sonic Model"};

bool DisableBoard = Common::reader.GetBoolean("Changes", "DisableBoard", false);

void Misc::Install()
{
	// Unwiished Fall
	if (Common::reader.GetBoolean("Changes", "WiiFall", false)) WRITE_MEMORY(0x15E812C, const char*, "sn_wall_fly02_loop");

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

	// https://github.com/brianuuu/DllMods/blob/master/Source/Sonic06DefinitiveExperience/NextGenSonic.cpp
	if(Common::reader.GetBoolean("Restorations", "DPadDisable", true))
		WRITE_JUMP(0xD97B56, (void*)0xD97B9E); // Disable D-Pad Input

	INSTALL_HOOK(SonicMiscUpdate);
	INSTALL_HOOK(MiscYPrompt);

	INSTALL_HOOK(MiscRestart);

	if (MoreVoice) { INSTALL_HOOK(MiscLife); }
}