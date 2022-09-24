boost::shared_ptr<Sonic::CGameObjectCSD> spTownScreen;
Chao::CSD::RCPtr<Chao::CSD::CProject> rcTownScreen;
Chao::CSD::RCPtr<Chao::CSD::CScene> info, cam;
float xAspectOffsetTownScreen = 0.0f;
float yAspectOffsetTownScreen = 0.0f;
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
//Brianuu/Skyth
void CalculateAspectOffsetsTownScreen()
{
	if (*(size_t*)0x6F23C6 != 0x75D8C0D9) // Widescreen Support
	{
		const float aspect = (float)*(size_t*)0x1DFDDDC / (float)*(size_t*)0x1DFDDE0;

		if (aspect * 9.0f > 16.0f)
		{
			xAspectOffsetTownScreen = 720.0f * aspect - 1280.0f;
			yAspectOffsetTownScreen = 0.0f;
		}
		else
		{
			xAspectOffsetTownScreen = 0.0f;
			yAspectOffsetTownScreen = 1280.0f / aspect - 720.0f;
		}
	}
	else
	{
		xAspectOffsetTownScreen = 0.0f;
		yAspectOffsetTownScreen = 0.0f;
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

HOOK(void, __fastcall, CHudPlayableMenuStart, 0x108DEB0, Sonic::CGameObject *This, int a2, int a3, void **a4, void* Edx) {
	originalCHudPlayableMenuStart(This, a2, a3, a4, Edx);
	RemoveHubCallbackTownScreen(This, nullptr, nullptr);
	if (Common::SUHud && Common::UP && Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) {
		CalculateAspectOffsetsTownScreen();
		Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());
		boost::shared_ptr<Sonic::CCsdProject> spCsdProjectTown(new Sonic::CCsdProject);

		size_t& flags = ((size_t*)This)[151];

		wrapper.GetCsdProject(spCsdProjectTown, "ui_townscreen");
		rcTownScreen = spCsdProjectTown->m_rcProject;
		info = rcTownScreen->CreateScene("info");
		info->SetPosition(xAspectOffsetTownScreen, 0);
		CSDCommon::FreezeMotion(*info);

		cam = rcTownScreen->CreateScene("cam");
		cam->SetPosition(xAspectOffsetTownScreen, 0);
		CSDCommon::FreezeMotion(*cam);

		CSDCommon::PlayAnimation(*info, "Usual_so_Anim", Chao::CSD::eMotionRepeatType_Loop, 1, 0);
		CSDCommon::PlayAnimation(*cam, "Usual_so_Anim", Chao::CSD::eMotionRepeatType_Loop, 1, 0);

		info->GetNode("ring_num")->SetText("157861");

		info->GetNode("S_medal_lv_num")->SetText("7");
		info->GetNode("S_medal_num")->SetText("[200]");

		info->GetNode("M_medal_lv_num")->SetText("7");
		info->GetNode("M_medal_num")->SetText("[200]");

		flags &= ~(0x1 | 0x2 | 0x4 | 0x200 | 0x800); // Mask to prevent crash when game tries accessing the elements we disabled later on

		CreateScreenTownScreen(This);
	}
}
void HubUI::Install() {
	INSTALL_HOOK(CHudPlayableMenuStart);
	WRITE_MEMORY(0x16A467C, void*, RemoveHubCallbackTownScreen);
}