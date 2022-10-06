// Recycled code from Brianuu's 06 Experience
float const c_chaosEnergyReward = 2.0f;
bool expCountDown = false;
float expTimer = 4;
float expTime = 0;
float expParticleTimer = 1.0f;
float expParticleTime = 0;
bool expParticleTimerPlay = false;
bool expHidden = false;
int expLevel = 0;
float expAmount = 0;
const std::string expFileName = "exp.sav";

boost::shared_ptr<Sonic::CGameObjectCSD> spExp;
Chao::CSD::RCPtr<Chao::CSD::CProject> rcExp;
Chao::CSD::RCPtr<Chao::CSD::CScene> exp_count;

float xAspectOffsetExp = 0.0f;
float yAspectOffsetExp = 0.0f;

SharedPtrTypeless ChaosEnergyHandle;

void CreateScreenEXP(Sonic::CGameObject* pParentGameObject)
{
	if (rcExp && !spExp)
		pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spExp = boost::make_shared<Sonic::CGameObjectCSD>(rcExp, 0.5f, "HUD", false), "main", pParentGameObject);
}
void KillScreenEXP()
{
	if (spExp) {
		spExp->SendMessage(spExp->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spExp = nullptr;
	}
}
void __fastcall CHudSonicStageRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	KillScreenEXP();
	Chao::CSD::CProject::DestroyScene(rcExp.Get(), exp_count);
	rcExp = nullptr;
	exp_count = nullptr;
}
//Brianuu/Skyth
void CalculateAspectOffsetsExp()
{
	if (*(size_t*)0x6F23C6 != 0x75D8C0D9) // Widescreen Support
	{
		const float aspect = (float)*(size_t*)0x1DFDDDC / (float)*(size_t*)0x1DFDDE0;

		if (aspect * 9.0f > 16.0f)
		{
			xAspectOffsetExp = 720.0f * aspect - 1280.0f;
			yAspectOffsetExp = 0.0f;
		}
		else
		{
			xAspectOffsetExp = 0.0f;
			yAspectOffsetExp = 1280.0f / aspect - 720.0f;
		}
	}
	else
	{
		xAspectOffsetExp = 0.0f;
		yAspectOffsetExp = 0.0f;
	}
}
HOOK(void, __fastcall, CHudSonicStageDelayProcessImpEXP, 0x109A8D0, Sonic::CGameObject* This) {
	originalCHudSonicStageDelayProcessImpEXP(This);
	if (BlueBlurCommon::IsModern() && Common::SUHud) {
		CHudSonicStageRemoveCallback(This, nullptr, nullptr);

		CalculateAspectOffsetsExp();

		Sonic::CCsdDatabaseWrapper wrapperExp(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

		boost::shared_ptr<Sonic::CCsdProject> spCsdProjectExp(new Sonic::CCsdProject);

		size_t& flags = ((size_t*)This)[151];

		wrapperExp.GetCsdProject(spCsdProjectExp, "exp");
		rcExp = spCsdProjectExp->m_rcProject;
		exp_count = rcExp->CreateScene("exp_count");
		char text[256];
		sprintf(text, "%02d", expLevel);
		exp_count->GetNode("exp")->SetText(text);
		//exp_count->GetNode("gauge")->SetScale(expAmount, 0.65f);
		exp_count->SetPosition(xAspectOffsetExp, 0);
		CSDCommon::FreezeMotion(*exp_count);
		expHidden = true;

		flags &= ~(0x1 | 0x2 | 0x4 | 0x200 | 0x800); // Mask to prevent crash when game tries accessing the elements we disabled later on

		CreateScreenEXP(This);
	}
}
std::string readExpFile(int index)
{
	std::string line;
	std::ifstream expFile(expFileName);
	int currentLine = 0;
	if (expFile.is_open())
	{
		while (!expFile.eof()) {
			currentLine++;
			std::getline(expFile, line);
			if (currentLine == index) break;
		}
		return line;
	}
	else {
		return "NULL";
	}
}
void writeToExpFile() {
	std::ofstream expFile(expFileName);
	if (expFile.is_open())
	{
		expFile << std::to_string(expLevel) + "\n";
		expFile << std::to_string(expAmount) + "\n";
		expFile.close();
	}
}
HOOK(void, __fastcall, ChaosEnergy_MsgGetHudPosition, 0x1096790, void* This, void* Edx, MsgGetHudPosition* message)
{
	if (message->m_type == 0)
	{
		Eigen::Vector3f sonicPosition;
		Eigen::Quaternionf sonicRotation;
		if (Common::GetPlayerTransform(sonicPosition, sonicRotation))
		{
			sonicPosition += sonicRotation * (Eigen::Vector3f::UnitY() * 0.5f); // half Sonic's height
			message->m_position = sonicPosition;
			return;
		}
	}

	originalChaosEnergy_MsgGetHudPosition(This, Edx, message);
}
bool renderGameHud;
bool maxEXP = false;
HOOK(void, __fastcall, CHudSonicStageUpdateParallelEXP, 0x1098A50, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo) {
	originalCHudSonicStageUpdateParallelEXP(This, Edx, in_rUpdateInfo);
	if (BlueBlurCommon::IsModern() && Common::SUHud) {
		renderGameHud = *(bool*)0x1A430D8;
		Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		if (expParticleTimerPlay) {
			if (expParticleTime > 0) {
				expParticleTime -= in_rUpdateInfo.DeltaTime;
			}
			else if (expParticleTime <= 0) {
				expParticleTimerPlay = false;
			}
		}
		if (expCountDown) {
			if (expTime > 0) {
				expTime -= in_rUpdateInfo.DeltaTime;
				float frameBefore = exp_count->m_MotionFrame;
				CSDCommon::PlayAnimation(*exp_count, "size", Chao::CSD::eMotionRepeatType_PlayOnce, 1, (maxEXP) ? 99 : std::clamp(expAmount / 0.63f,0.0f,99.0f));
				CSDCommon::PlayAnimation(*exp_count, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, frameBefore);
			}
			else if (expTime <= 0) {
				expHidden = false;
				CSDCommon::PlayAnimation(*exp_count, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, exp_count->m_MotionEndFrame, false, true);
				expCountDown = false;
			}
		}
		else {
			if (exp_count->m_MotionFrame <= 0) {
				expHidden = true;
			}
		}
		if (renderGameHud && Common::SUHud) {
			exp_count->SetHideFlag(expHidden);
		}
		else {
			exp_count->SetHideFlag(true);
		}
	}
}
void chaosEnergyParticle() {
	if (BlueBlurCommon::IsModern() && Common::SUHud) {
		Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
		printf("[SU Restorations] EXP Particle Collected\n");
		if (!expParticleTimerPlay && expParticleTime <= 0) {
			expParticleTimerPlay = true;
			expParticleTime = expParticleTimer;
			Common::fCGlitterCreate(sonic, ChaosEnergyHandle, middlematrixNode, "exp_particles", 0);
		}
		if (expHidden && expTime <= 0 && !expCountDown) {
			expHidden = false;
			expTime = expTimer;
			expCountDown = true;
			CSDCommon::PlayAnimation(*exp_count, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 0, 1);
		}
		if (!maxEXP) {
			expAmount += 0.35f;
			if (expAmount >= 63 && expLevel < 99) {
				expAmount -= 63;
				expLevel++;
			}
			expAmount = std::clamp(expAmount, 8.8f, 63.0f);
		}
		char text[256];
		sprintf(text, "%02d", expLevel);
		exp_count->GetNode("exp")->SetText((maxEXP) ? "99" : text);
		//exp_count->GetNode("gauge")->SetScale(expAmount, 0.65f);
	}
}
void __declspec(naked) addBoostFromChaosEnergy()
{
	static uint32_t fpAddBoostToSonicContext = 0xE5D990;
	static uint32_t returnAddress = 0x112459A;
	__asm
	{
		// Check Sonic context just in case
		mov		esi, PLAYER_CONTEXT
		cmp		dword ptr[esi], 0
		je		jump

		// Award Sonic 5 boost
		push	ecx
		push	edi
		push	0
		movss	xmm1, c_chaosEnergyReward
		mov		esi, [esi]
		call[fpAddBoostToSonicContext]
		call	chaosEnergyParticle
		pop		edi
		pop		ecx

		// original function
		jump :
		mov     ecx, [ecx + 0FCh]
			jmp[returnAddress]
	}
}
uint32_t __fastcall getEnemyChaosEnergyAmountImpl(uint32_t* pEnemy)
{
	printf("0x%08X\n", pEnemy[0]);
	switch (pEnemy[0])
	{
	case 0x016F593C: return 5; // EFighter
	case 0x016F70BC: return 3; // Spinner
	case 0x016FB1FC: return 5; // EFighterMissile
	case 0x016FB62C: return 4; // AirCannon
	case 0x016F912C: return 8; // Mole
	default: return 5;
	}
}
void __declspec(naked) getEnemyChaosEnergyAmount()
{
	static uint32_t returnAddress = 0xBE05EF;
	__asm
	{
		mov		ecx, esi
		call	getEnemyChaosEnergyAmountImpl
		mov		ecx, eax
		jmp[returnAddress]
	}
}
HOOK(int, __fastcall, ProcMsgRestartStageEXP, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	if (exp_count && Common::SUHud) {
		expCountDown = false;
		expTime = 0;
		expHidden = true;
		exp_count->SetHideFlag(true);
	}
	return originalProcMsgRestartStageEXP(This, Edx, message);
}
HOOK(void, __fastcall, HudResult_MsgStartGoalResultEXP, 0x10B58A0, uint32_t* This, void* Edx, void* message)
{
	if (!maxEXP && Common::SUHud)
		writeToExpFile();
	originalHudResult_MsgStartGoalResultEXP(This, Edx, message);
}
void EXP::Install() {
	maxEXP = Common::reader.GetBoolean("EXP", "Max", false);
	expLevel = stoi(readExpFile(1));
	expAmount = stof(readExpFile(2));
	INSTALL_HOOK(ChaosEnergy_MsgGetHudPosition);
	INSTALL_HOOK(CHudSonicStageDelayProcessImpEXP);
	INSTALL_HOOK(CHudSonicStageUpdateParallelEXP);
	INSTALL_HOOK(ProcMsgRestartStageEXP);
	INSTALL_HOOK(HudResult_MsgStartGoalResultEXP);
	// 06 Experience Code
	// Set absorb time to 1.2s
	static float ChaosEnergyParam[] =
	{
		3.0f,	// UpHeight
		0.3f,	// UpTime
		0.35f	// AbsorbTime
	};
	WRITE_MEMORY(0xC8EF3D, float*, ChaosEnergyParam);
	WRITE_MEMORY(0x11244A6, float*, ChaosEnergyParam);
	WRITE_JUMP(0xBE05E9, getEnemyChaosEnergyAmount);
	// Don't reward boost from enemy spawned chaos energy
	WRITE_JUMP(0xE60C6C, (void*)0xE60D79);
	// Spawn chaos energy base on currect trick level (visual only)
	WRITE_MEMORY(0x16D1970, uint32_t, 0, 0, 0, 0);
	// Give 0 chaos energy for board trick jump (visual only)
	WRITE_MEMORY(0x11A12E4, uint8_t, 0);
	// Award 5 boost when chaos energy reach Sonic
	WRITE_JUMP(0x1124594, addBoostFromChaosEnergy);
}