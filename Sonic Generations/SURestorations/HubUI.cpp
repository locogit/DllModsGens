boost::shared_ptr<Sonic::CGameObjectCSD> spTownScreen;
Chao::CSD::RCPtr<Chao::CSD::CProject> rcTownScreen;
Chao::CSD::RCPtr<Chao::CSD::CScene> info, cam;

int ringCount = 0;

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

void HubUI::Save() {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	ringCount += sonic->m_RingCount;
	Common::saveData.rings = ringCount;
}

Sonic::CGameObject *ThisObjReference;

HOOK(void, __fastcall, CHudPlayableMenuStart, 0x108DEB0, Sonic::CGameObject *This, int a2, int a3, void **a4, void* Edx) {
	ThisObjReference = This;
	originalCHudPlayableMenuStart(This, a2, a3, a4, Edx);
	RemoveHubCallbackTownScreen(This, nullptr, nullptr);

	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	auto spCsdProjectTown = wrapper.GetCsdProject("ui_townscreen");

	rcTownScreen = spCsdProjectTown->m_rcProject;
	info = rcTownScreen->CreateScene("info");
	info->SetPosition(0, 0);
	CSDCommon::FreezeMotion(*info);

	cam = rcTownScreen->CreateScene("cam");
	cam->SetPosition(0, 0);
	CSDCommon::FreezeMotion(*cam);

	CSDCommon::PlayAnimation(*info, "Usual_so_Anim", Chao::CSD::eMotionRepeatType_Loop, 1, 0);
	CSDCommon::PlayAnimation(*cam, "Usual_so_Anim", Chao::CSD::eMotionRepeatType_Loop, 1, 0);

	ringCount = std::clamp(Common::saveData.rings, 0, 999999);

	char text[256];
	sprintf(text, "%d", ringCount);
	info->GetNode("ring_num")->SetText(text);

	info->GetNode("S_medal_lv_num")->SetText("7");
	info->GetNode("S_medal_num")->SetText("[200]");

	info->GetNode("M_medal_lv_num")->SetText("7");
	info->GetNode("M_medal_num")->SetText("[200]");

	CreateScreenTownScreen(This);
}
void HubUI::SetHide(bool hide) {
	if (spTownScreen) {
		info->SetHideFlag(hide);
		cam->SetHideFlag(hide);
	}
}
void HubUI::Install() {
	ringCount = std::clamp(Common::saveData.rings, 0, 999999);
	INSTALL_HOOK(CHudPlayableMenuStart);
}