boost::shared_ptr<Sonic::CGameObjectCSD> spTownScreen;
Chao::CSD::RCPtr<Chao::CSD::CProject> rcTownScreen;
Chao::CSD::RCPtr<Chao::CSD::CScene> info, cam;

int ringCount = 0;
std::string hubFileName = "hub.sav";

std::string readHubFile(int index)
{
	std::string line;
	std::ifstream hubFile(hubFileName);
	int currentLine = 0;
	if (hubFile.is_open())
	{
		while (!hubFile.eof()) {
			currentLine++;
			std::getline(hubFile, line);
			if (currentLine == index) break;
		}
		return line;
	}
	else {
		return "NULL";
	}
}

void writeToHubFile() {
	std::ofstream hubFile(hubFileName);
	if (hubFile.is_open())
	{
		hubFile << std::to_string(ringCount) + "\n";
		hubFile.close();
	}
}

void CreateScreenTownScreen(Sonic::CGameObject* pParentGameObject)
{
	if (rcTownScreen && !spTownScreen)
		pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spTownScreen = boost::make_shared<Sonic::CGameObjectCSD>(rcTownScreen, 0.5f, "HUD", false), "main", pParentGameObject);
}

void KillScreenTownScreen()
{
	if (spTownScreen) {
		spTownScreen->SendMessage(spTownScreen->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spTownScreen = nullptr;
	}
}

void __fastcall RemoveHubCallbackTownScreen(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	KillScreenTownScreen();
	if(info)
		Chao::CSD::CProject::DestroyScene(rcTownScreen.Get(), info);
	if(cam)
		Chao::CSD::CProject::DestroyScene(rcTownScreen.Get(), cam);
	rcTownScreen = nullptr;
}

HOOK(void, __fastcall, HudResult_MsgStartGoalResultHUB, 0x10B58A0, uint32_t* This, void* Edx, void* message)
{
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	ringCount += sonic->m_RingCount;
	writeToHubFile();
	originalHudResult_MsgStartGoalResultHUB(This, Edx, message);
}

HOOK(void, __fastcall, CHudPlayableMenuStart, 0x108DEB0, Sonic::CGameObject *This, int a2, int a3, void **a4, void* Edx) {
	originalCHudPlayableMenuStart(This, a2, a3, a4, Edx);
	RemoveHubCallbackTownScreen(This, nullptr, nullptr);

	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	auto spCsdProjectTown = wrapper.GetCsdProject("ui_townscreen");

	size_t& flags = ((size_t*)This)[151];

	rcTownScreen = spCsdProjectTown->m_rcProject;
	info = rcTownScreen->CreateScene("info");
	info->SetPosition(0, 0);
	CSDCommon::FreezeMotion(*info);

	cam = rcTownScreen->CreateScene("cam");
	cam->SetPosition(0, 0);
	CSDCommon::FreezeMotion(*cam);

	CSDCommon::PlayAnimation(*info, "Usual_so_Anim", Chao::CSD::eMotionRepeatType_Loop, 1, 0);
	CSDCommon::PlayAnimation(*cam, "Usual_so_Anim", Chao::CSD::eMotionRepeatType_Loop, 1, 0);

	ringCount = std::clamp(stoi(readHubFile(1)), 0, 999999);

	char text[256];
	sprintf(text, "%d", ringCount);
	info->GetNode("ring_num")->SetText(text);

	info->GetNode("S_medal_lv_num")->SetText("7");
	info->GetNode("S_medal_num")->SetText("[200]");

	info->GetNode("M_medal_lv_num")->SetText("7");
	info->GetNode("M_medal_num")->SetText("[200]");

	flags &= ~(0x1 | 0x2 | 0x4 | 0x200 | 0x800); // Mask to prevent crash when game tries accessing the elements we disabled later on

	CreateScreenTownScreen(This);
}

void HubUI::Install() {
	ringCount = std::clamp(stoi(readHubFile(1)), 0, 999999);
	INSTALL_HOOK(HudResult_MsgStartGoalResultHUB);
	INSTALL_HOOK(CHudPlayableMenuStart);
	WRITE_MEMORY(0x16A467C, void*, RemoveHubCallbackTownScreen);
}