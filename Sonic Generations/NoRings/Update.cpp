enum RingType {
	DamageCancel = 0,
	Dead = 1,
	DamageShock = 2,
	DamageFreeze = 3,
	CrashWall = 4,
	DamageNearFar = 5,
	Stop = 6,
	Omochao = 7,
};
static const int RingTypeLength = 7;

std::string RingTypeToString(RingType type) {
	switch (type)
	{
	case DamageCancel:
		return "DamageCancel";
		break;
	case Dead:
		return "Dead";
		break;
	case DamageShock:
		return "DamageShock";
		break;
	case DamageFreeze:
		return "DamageFreeze";
		break;
	case CrashWall:
		return "CrashWall";
		break;
	case DamageNearFar:
		return "DamageNearFar";
		break;
	case Stop:
		return "Stop";
		break;
	case Omochao:
		return "Omochao";
		break;
	default:
		return "null";
		break;
	}
}

static RingType currentRingType = RingType::DamageCancel;
static int currentRingTypeIndex = 0;

static bool forceDivingDamage = true;
static bool forceGrindingDamage = true;

static INIReader saveReader("save.ini");

static std::fstream save;

static bool firstTime = true;

static float timeSinceLastRing = 0.0f;

static float omochaoTime = -1;

static int ringsTouched = 0;

static bool firstRingTouch = true;

void WriteSaveIni() {
	save.open("save.ini", std::ios::out);  // open a file to perform write operation using file object
	if (save.is_open()) //checking whether the file is open
	{
		save << "[RingType]\nType="+std::to_string(currentRingTypeIndex)+
			"\n[ModInfo]\nFirstTime="+(firstTime ? "True" : "False");   //inserting text
		save.close();    //close the file object
	}
}

// NextGenSonic.cpp - brianuuu
FUNCTION_PTR(int, __stdcall, SetOutOfControl, 0xE5AC00, CSonicContext* context, float duration);

boost::shared_ptr<Sonic::CGameObjectCSD> spDecoText;
Chao::CSD::RCPtr<Chao::CSD::CProject> rcDecoText;
Chao::CSD::RCPtr<Chao::CSD::CScene> deco_text;
Chao::CSD::RCPtr<Chao::CSD::CScene> deco_text_2;
Chao::CSD::RCPtr<Chao::CSD::CScene> deco_text_3;
Chao::CSD::RCPtr<Chao::CSD::CScene> deco_text_4;

void KillScreenDeco()
{
	if (spDecoText)
	{
		spDecoText->SendMessage(spDecoText->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spDecoText = nullptr;
	}
}

void __fastcall CHudSonicStageRemoveCallbackDeco(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	KillScreenDeco();
	Chao::CSD::CProject::DestroyScene(rcDecoText.Get(), deco_text);
	Chao::CSD::CProject::DestroyScene(rcDecoText.Get(), deco_text_2);
	Chao::CSD::CProject::DestroyScene(rcDecoText.Get(), deco_text_3);
	Chao::CSD::CProject::DestroyScene(rcDecoText.Get(), deco_text_4);
	rcDecoText = nullptr;
	deco_text = nullptr;
	deco_text_2 = nullptr;
	deco_text_3 = nullptr;
	deco_text_4 = nullptr;
}

void CreateScreenDeco(Sonic::CGameObject* pParentGameObject)
{
	if (rcDecoText && !spDecoText)
		pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spDecoText = boost::make_shared<Sonic::CGameObjectCSD>(rcDecoText, 0.5f, "HUD", false), "main", pParentGameObject);
}

HOOK(void, __fastcall, CHudSonicStageDelayProcessImpDeco, 0x109A8D0, Sonic::CGameObject* This) {
	originalCHudSonicStageDelayProcessImpDeco(This);
	if (BlueBlurCommon::IsBossSuper()) { return; }

	CHudSonicStageRemoveCallbackDeco(This, nullptr, nullptr);

	Sonic::CCsdDatabaseWrapper wrapperDecoText(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	auto spCsdProjectDecoText = wrapperDecoText.GetCsdProject("ui_deco_text_ring");

	size_t& flags = ((size_t*)This)[151];

	rcDecoText = spCsdProjectDecoText->m_rcProject;
	deco_text = rcDecoText->CreateScene("deco_text");
	CSDCommon::PlayAnimation(*deco_text, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f, 0.0f);

	deco_text_2 = rcDecoText->CreateScene("deco_text_2");
	CSDCommon::PlayAnimation(*deco_text_2, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f, 0.0f);

	deco_text_3 = rcDecoText->CreateScene("deco_text_3");
	CSDCommon::PlayAnimation(*deco_text_3, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f, 0.0f);

	deco_text_4 = rcDecoText->CreateScene("deco_text_4");
	CSDCommon::PlayAnimation(*deco_text_4, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f, 0.0f);
	deco_text_4->SetHideFlag(true);

	CreateScreenDeco(This);
}

HOOK(void, __fastcall, CHudSonicStageUpdateParallelDeco, 0x1098A50, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo) {
	originalCHudSonicStageUpdateParallelDeco(This, Edx, in_rUpdateInfo);
	if (BlueBlurCommon::IsBossSuper()) { return; }
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	const char* text = format("Ring Type : %s", RingTypeToString(currentRingType).c_str());

	deco_text->GetNode("Text_sdw")->SetText(text);
	deco_text->GetNode("Text_yellow")->SetText(text);
	deco_text->GetNode("Text_blue")->SetText(text);

	timeSinceLastRing += in_rUpdateInfo.DeltaTime * *(float*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x1A0 });

	int s = (int)timeSinceLastRing;
	int minutes = floor(s / 60);
	int seconds = floor(s % 60);
	int milliseconds = (int)((timeSinceLastRing - s) * 100);
	const char* timeText = format("Time Since Last Ring : %02d:%02d.%02d", minutes, seconds, milliseconds);

	deco_text_2->GetNode("Text_sdw")->SetText(timeText);
	deco_text_2->GetNode("Text_yellow")->SetText(timeText);
	deco_text_2->GetNode("Text_blue")->SetText(timeText);

	const char* touchedText = format("Rings Touched : %d", ringsTouched);

	deco_text_3->GetNode("Text_sdw")->SetText(touchedText);
	deco_text_3->GetNode("Text_yellow")->SetText(touchedText);
	deco_text_3->GetNode("Text_blue")->SetText(touchedText);

	if (omochaoTime != -1) {
		omochaoTime += in_rUpdateInfo.DeltaTime;
		if (omochaoTime >= 4.0f) {
			*(float*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x1A0 }) = 1.0f;
			*(bool*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x19C }) = false;
			omochaoTime = -1;
			CSDCommon::PlayAnimation(*deco_text_4, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 0.35f, 0.0f, 0.0f, false, true);
		}
	}
}

void OnRing(hh::fnd::Message& msg) {
	if (BlueBlurCommon::IsBossSuper()) { return; }
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (sonic->m_pPlayer->m_ActorID == msg.m_SenderActorID)
	{
		timeSinceLastRing = 0.0f;

		ringsTouched++;

		CSDCommon::PlayAnimation(*deco_text_3, "Select_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f, 0.0f);

		static SharedPtrTypeless sound;

		switch (currentRingType)
		{
		case RingType::Dead:
			sonic->m_pPlayer->SendMessage(sonic->m_pPlayer->m_ActorID, boost::make_shared<Sonic::Message::MsgDead>(true));
			sonic->ChangeState(sonic->m_Grounded ? "NormalDamageDead" : "NormalDamageDeadAir");
			sonic->PlaySound(4002008, false);
			return;
		case Omochao:
			if (omochaoTime == -1) {
				Common::PlaySoundStatic(sound, 819004);
				*(bool*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x19C }) = true;
				*(float*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x1A0 }) = 0.0001f;
				omochaoTime = 0.0f;

				deco_text_4->SetHideFlag(false);
				CSDCommon::PlayAnimation(*deco_text_4, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 0.35f, 0.0f);

				const char* gotText = firstRingTouch ? "Omochao Says : You Got A Ring!" : "Omochao Says : You Got Another Ring!";
				deco_text_4->GetNode("Text_sdw")->SetText(gotText);
				deco_text_4->GetNode("Text_yellow")->SetText(gotText);
				deco_text_4->GetNode("Text_blue")->SetText(gotText);

				firstRingTouch = false;

				SetOutOfControl(BlueBlurCommon::IsModern() ? *pModernSonicContext : *pClassicSonicContext, 0.0004f);
				return;
			}
		}

		if ((Common::GetSonicStateFlags()->Diving || Common::GetSonicStateFlags()->DivingFloat) && forceDivingDamage) {
			sonic->ChangeState("DivingDamage");
			sonic->m_Velocity = Eigen::Vector3f::Zero();
			return;
		}

		if (Common::IsPlayerGrinding() && forceGrindingDamage) {
			sonic->ChangeState("GrindDamageMiddle");
			sonic->m_Velocity = Eigen::Vector3f::Zero();
			return;
		}

		switch (currentRingType)
		{
		case DamageCancel:
			sonic->ChangeState("DamageCancel");
			break;
		case DamageNearFar:
			sonic->ChangeState("DamageNearFar");
			sonic->PlaySound(4002008, false);
			break;
		case DamageShock:
			sonic->ChangeState("DamageShock");
			break;
		case CrashWall:
			sonic->ChangeState("CrashWall");
			break;
		case DamageFreeze:
			sonic->ChangeState("DamageFreeze");
			break;
		case Stop:
			sonic->m_Velocity = Eigen::Vector3f::Zero();
			sonic->m_HorizontalVelocity = Eigen::Vector3f::Zero();
			sonic->m_VerticalVelocity = Eigen::Vector3f::Zero();
			sonic->m_PreviousVelocity = Eigen::Vector3f::Zero();

			SetOutOfControl(BlueBlurCommon::IsModern() ? *pModernSonicContext : *pClassicSonicContext, 0.5f);
			break;
		default:
			break;
		}
	}
}

// from briannu i think
HOOK(void, __fastcall, CObjRingProcMsgHitEventCollision, 0x10534B0, Sonic::CGameObject3D* This, void* Edx, hh::fnd::Message& in_rMsg)
{	
	originalCObjRingProcMsgHitEventCollision(This, Edx, in_rMsg);
	OnRing(in_rMsg);
}

HOOK(void, __fastcall, CObjSuperRingProcMsgHitEventCollision, 0x11F2F10, Sonic::CGameObject3D* This, void* Edx, hh::fnd::Message& in_rMsg)
{
	originalCObjSuperRingProcMsgHitEventCollision(This, Edx, in_rMsg);
	OnRing(in_rMsg);
}

void Move(bool fwd) {
	static SharedPtrTypeless sound;
	Common::PlaySoundStatic(sound, 1000004);

	if (fwd) {
		if (currentRingTypeIndex == RingTypeLength) {
			currentRingTypeIndex = 0;
		}
		else {
			currentRingTypeIndex++;
		}
	}
	else {
		if (currentRingTypeIndex == 0) {
			currentRingTypeIndex = RingTypeLength;
		}
		else {
			currentRingTypeIndex--;
		}
	}

	currentRingType = (RingType)currentRingTypeIndex;

	CSDCommon::PlayAnimation(*deco_text, "Select_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f, 0.0f);

	WriteSaveIni();
}

bool fwdKB = false;
bool backKB = false;

HOOK(void, __fastcall, SonicUpdateRing, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicUpdateRing(This, _, updateInfo);
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

	if (input.IsTapped(Sonic::eKeyState_RightStick)) {

		Move(true);
	}

	if (input.IsTapped(Sonic::eKeyState_LeftStick)) {
		Move(false);
	}

	if ((GetAsyncKeyState(VK_F1) & 0x8000) && !backKB) {
		Move(false);
		backKB = true;
	}
	else if(GetAsyncKeyState(VK_F1) == 0) {
		backKB = false;
	}

	if ((GetAsyncKeyState(VK_F2) & 0x8000) && !fwdKB) {
		Move(true);
		fwdKB = true;
	}
	else if (GetAsyncKeyState(VK_F2) == 0) {
		fwdKB = false;
	}

	//DebugDrawText::log(format("Current Ring Type : %s", RingTypeToString(currentRingType).c_str()));

	if (!BlueBlurCommon::IsBossSuper()) { sonic->m_RingCount = 0; }
}

HOOK(int, __fastcall, Sonic_MsgRestartStage, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	int result = originalSonic_MsgRestartStage(This, Edx, message);

	if (!BlueBlurCommon::IsBossSuper()) { 
		timeSinceLastRing = 0.0f; 

		if (rcDecoText) { deco_text_4->SetHideFlag(true); }
		*(float*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x1A0 }) = 1.0f;
		*(bool*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x19C }) = false;
		omochaoTime = -1;
		firstRingTouch = true; 
	}

	return result;
}

void Update::Install()
{
	forceDivingDamage = Common::reader.GetBoolean("Main", "ForceDiving", true);
	forceGrindingDamage = Common::reader.GetBoolean("Main", "ForceRail", true);

	firstTime = saveReader.GetBoolean("ModInfo", "FirstTime", true);
	currentRingTypeIndex = saveReader.GetInteger("RingType", "Type", 0);
	currentRingType = (RingType)currentRingTypeIndex;

	if (saveReader.GetBoolean("ModInfo", "FirstTime", true)) {
		MessageBoxA(nullptr, "Thank you for downloading 'Ring Allergy : Reimagined'!\nPlease note that to change the ring type you need to click on the joysticks (F1 & F2 for keyboard).", "Ring Allergy : Reimagined", MB_OK);
		firstTime = false;
		WriteSaveIni();
	}

	INSTALL_HOOK(CObjRingProcMsgHitEventCollision);
	INSTALL_HOOK(CObjSuperRingProcMsgHitEventCollision);
	INSTALL_HOOK(SonicUpdateRing);
	INSTALL_HOOK(Sonic_MsgRestartStage);

	INSTALL_HOOK(CHudSonicStageDelayProcessImpDeco);
	INSTALL_HOOK(CHudSonicStageUpdateParallelDeco);
	WRITE_MEMORY(0x16A467C, void*, CHudSonicStageRemoveCallbackDeco);

	// Remove message delay for MsgFinishPause, this can cause HUD to not show up
	// anymore if we pause before HUD show up when time is slowed down (thanks brianuuu)
	WRITE_MEMORY(0x10A1500, uint8_t, 0xD9, 0xEE, 0x90, 0x90, 0x90, 0x90);
}