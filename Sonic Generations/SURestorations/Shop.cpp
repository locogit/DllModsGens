
Chao::CSD::RCPtr<Chao::CSD::CProject> rcShop;
Chao::CSD::RCPtr<Chao::CSD::CScene> bg_1, bg_2, bg_3, bg_4, bg_3_num, bg_5, bg5_select, select_1, shop_nametag;
boost::shared_ptr<Sonic::CGameObjectCSD> spShop;


void __fastcall CHudShopRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	Shop::KillScreen();
	Chao::CSD::CProject::DestroyScene(rcShop.Get(), bg_1);
	Chao::CSD::CProject::DestroyScene(rcShop.Get(), bg_2);
	Chao::CSD::CProject::DestroyScene(rcShop.Get(), bg_3);
	Chao::CSD::CProject::DestroyScene(rcShop.Get(), bg_4);
	Chao::CSD::CProject::DestroyScene(rcShop.Get(), bg_5);
	Chao::CSD::CProject::DestroyScene(rcShop.Get(), bg5_select);
	Chao::CSD::CProject::DestroyScene(rcShop.Get(), bg_3_num);
	Chao::CSD::CProject::DestroyScene(rcShop.Get(), shop_nametag);
	Chao::CSD::CProject::DestroyScene(rcShop.Get(), select_1);
	rcShop = nullptr;
}
void Shop::CreateScreen(Sonic::CGameObject* pParentGameObject)
{
	if (rcShop && !spShop)
		pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spShop = boost::make_shared<Sonic::CGameObjectCSD>(rcShop, 0.5f, "HUD", false), "main", pParentGameObject);
}
void Shop::KillScreen()
{
	if (spShop)
	{
		spShop->SendMessage(spShop->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spShop = nullptr;
	}
}
void Shop::ToggleScreen(const bool visible, Sonic::CGameObject* pParentGameObject)
{
	if (visible)
		CreateScreen(pParentGameObject);
	else
		KillScreen();
}

HOOK(void*, __fastcall, Shop_UpdateApplication, 0xE7BED0, Sonic::CGameObject* This, void* Edx, float elapsedTime, uint8_t a3)
{
	return originalShop_UpdateApplication(This, Edx, elapsedTime, a3);
}
//sub_43FE50(Sonic::CGameObject *this, int dummy, Hedgehog::Base::CHolderBase *a3, int dummy2)
//HOOK(void, __fastcall, sub_43FE50, 0x43FE50, Sonic::CGameObject* This, int a2, int a3, int dummy2, void* Edx)
//{
//	originalsub_43FE50(This, a2, a3, dummy2, Edx);
//}
HOOK(void, __fastcall, ShopInit, 0x441E10, Sonic::CGameObject* This, int a2, int a3, void** a4, void* Edx)
{
	CHudShopRemoveCallback(This, nullptr, nullptr);
	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	auto spCsdProject = wrapper.GetCsdProject("ui_shop_swa");
	rcShop = spCsdProject->m_rcProject;
	bg_1 = rcShop->CreateScene("bg_1");
	bg_2 = rcShop->CreateScene("bg_2");
	bg_3 = rcShop->CreateScene("bg_3");
	bg_4 = rcShop->CreateScene("bg_4");
	bg_5 = rcShop->CreateScene("bg_5");
	select_1 = rcShop->CreateScene("select_1");
	bg5_select = rcShop->CreateScene("select_2");
	bg_3_num = rcShop->CreateScene("bg_3_num");
	bg_3_num->GetNode("num")->SetText("1");
	shop_nametag = rcShop->CreateScene("shop_nametag");
	CSDCommon::PlayAnimation(*bg_1, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
	CSDCommon::PlayAnimation(*bg_2, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
	CSDCommon::PlayAnimation(*bg_4, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
	CSDCommon::PlayAnimation(*bg_5, "Size_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 0, 12, 12);
	CSDCommon::PlayAnimation(*bg5_select, "Size_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 0, 12, 12);
	CSDCommon::PlayAnimation(*shop_nametag, "Size_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 5000);
	CSDCommon::FreezeMotion(*bg_5, 12);
	CSDCommon::FreezeMotion(*bg5_select, 12);
	CSDCommon::FreezeMotion(*select_1, 0);

	bg_3->SetHideFlag(true);
	bg_3_num->SetHideFlag(true);
	bg_5->SetHideFlag(true);
	bg5_select->SetHideFlag(true);

	bg_1->SetHideFlag(true);
	bg_2->SetHideFlag(true);
	bg_3->SetHideFlag(true);
	bg_4->SetHideFlag(true);
	bg_5->SetHideFlag(true);
	bg5_select->SetHideFlag(true);
	select_1->SetHideFlag(true);
	shop_nametag->SetHideFlag(true);

	Shop::CreateScreen(This);

	originalShopInit(This, a2, a3, a4, Edx);
}

//ogObj = This;
//CHudSonicStageRemoveCallback(This, nullptr, nullptr);
//Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());
//
//auto spCsdProject = wrapper.GetCsdProject("ui_worldmap");
//rcShop = spCsdProject->m_rcProject;
//headerBG = rcShop->CreateScene("worldmap_header_bg");
//headerIMG = rcShop->CreateScene("worldmap_header_img");
//footerBG = rcShop->CreateScene("worldmap_footer_bg");
//footerIMG = rcShop->CreateScene("worldmap_footer_img");
//CSDCommon::PlayAnimation(*headerBG, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 60);
//CSDCommon::PlayAnimation(*headerIMG, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
//CSDCommon::PlayAnimation(*footerBG, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 60);
//CSDCommon::PlayAnimation(*footerIMG, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
//
//TitleOption::CreateScreen(This);
//sub_43FE50()

bool ranOnce;
void ShowBGBuy()
{

	CSDCommon::PlayAnimation(*bg_3, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
	bg_3->SetHideFlag(false);
	bg_3_num->SetHideFlag(false);
	bg_5->SetHideFlag(false);
	bg5_select->SetHideFlag(false);

	CSDCommon::PlayAnimation(*bg_5, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 5);
}
void HideBGBuy()
{
	CSDCommon::PlayAnimation(*bg_5, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 5, 100);
	CSDCommon::PlayAnimation(*bg_3, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, true, true);
	bg_3_num->SetHideFlag(true);
	bg5_select->SetHideFlag(true);
}
bool playingScroll;
HOOK(int, __fastcall, sub_43F030, 0x43F030, int* This, int a2, void* Edx)
{
	auto inputPtr = &Sonic::CInputState::GetInstance()->m_PadStates[Sonic::CInputState::GetInstance()->m_CurrentPadStateIndex];
	playingScroll = inputPtr->IsDown(Sonic::eKeyState_LeftStickDown) || inputPtr->IsDown(Sonic::eKeyState_LeftStickUp);
	//This[70] = selection, true is no
	if (playingScroll)
	{
		CSDCommon::PlayAnimation(*bg5_select, "Scroll_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, This[70] ? 10 : 0, This[70] ? 20 : 10);
		playingScroll = false;
	}

	if (This[58] == 3)
		HideBGBuy();
	return originalsub_43F030(This, a2, Edx);
}
//index is Shop-This[189]
int indexSecondary = 0;
int indexCache = 0;
HOOK(void, __stdcall, SetShopSkillIndex, 0x440FF0, int a1, int index)
{
	originalSetShopSkillIndex(a1, index);

	bool goingUp = false;
	auto inputPtr = &Sonic::CInputState::GetInstance()->m_PadStates[Sonic::CInputState::GetInstance()->m_CurrentPadStateIndex];
	if (inputPtr->IsDown(Sonic::eKeyState_LeftStickDown))
	{
		indexSecondary++;
	}
	if (inputPtr->IsDown(Sonic::eKeyState_LeftStickUp))
	{
		goingUp = true;
		indexSecondary--;
	}

	Common::ClampInt(indexSecondary, 0, 4);
	if (index == 0 && indexCache == 8)
		indexSecondary = 0;
	if (index == 8 && indexCache == 0)
		indexSecondary = 4;

	//CSDCommon::PlayAnimation(*select_1, "Scroll_Anim", Chao::CSD::eMotionRepeatType_PlayOnce,1, indexCache * 10, indexSecondary * 10);

	if (!goingUp)
		CSDCommon::PlayAnimation(*select_1, "Scroll_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, (indexSecondary - 1) * 10, indexSecondary * 10);
	else
		CSDCommon::PlayAnimation(*select_1, "Scroll_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 40 + ((3 - indexSecondary) * 10), 40 + ((4 - indexSecondary) * 10));



	indexCache = index;

}
HOOK(void*, __fastcall, sub_43F9E0, 0x43F9E0, Sonic::CGameObject* This, char a2, void* Edx)
{
	return originalsub_43F9E0(This, a2, Edx);
}
HOOK(volatile signed __int32*, __fastcall, Shop_BuyNewSkillsInit, 0x442BD0, void* This)
{

	return originalShop_BuyNewSkillsInit(This);
}

void __declspec(naked) ShowBGBuyOnBuy()
{
	static uint32_t pAddr = 0x004409DE;
	__asm
	{
		call ShowBGBuy
		mov     ecx, [ebp + 34Ch]
		jmp[pAddr]
	}
}
void __declspec(naked) HideBGBuyOnCancel()
{
	static uint32_t pAddr = 0x0043F6D5;
	__asm
	{
		call HideBGBuy
		mov  dword ptr[ebp + 0E8h], 5
		jmp[pAddr]
	}
}
bool intro = false;
HOOK(void, __fastcall, sub_442960, 0x442960, int* This) {
	originalsub_442960(This);
	intro = false;
	bg_1->SetHideFlag(true);
	bg_2->SetHideFlag(true);
	bg_3->SetHideFlag(true);
	bg_3_num->SetHideFlag(true);
	bg_4->SetHideFlag(true);
	bg_5->SetHideFlag(true);
	bg5_select->SetHideFlag(true);
	select_1->SetHideFlag(true);
	shop_nametag->SetHideFlag(true);
}
HOOK(void, __fastcall, sub_442810, 0x442810, int* This) {
	intro = false;
	bg_1->SetHideFlag(true);
	bg_2->SetHideFlag(true);
	bg_3->SetHideFlag(true);
	bg_3_num->SetHideFlag(true);
	bg_4->SetHideFlag(true);
	bg_5->SetHideFlag(true);
	bg5_select->SetHideFlag(true);
	select_1->SetHideFlag(true);
	shop_nametag->SetHideFlag(true);
	originalsub_442810(This);
}
HOOK(void, __fastcall, sub_442E20, 0x442E20, int This) {
	originalsub_442E20(This);
	if (intro) return;
	intro = true;
	bg_1->SetHideFlag(false);
	bg_2->SetHideFlag(false);
	bg_4->SetHideFlag(false);
	select_1->SetHideFlag(false);
	shop_nametag->SetHideFlag(false);

	bg_3->SetHideFlag(true);
	bg_3_num->SetHideFlag(true);
	bg_5->SetHideFlag(true);
	bg5_select->SetHideFlag(true);

	CSDCommon::PlayAnimation(*bg_1, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
	CSDCommon::PlayAnimation(*bg_2, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
	CSDCommon::PlayAnimation(*bg_4, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
	CSDCommon::PlayAnimation(*bg_5, "Size_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 0, 12, 12);
	CSDCommon::PlayAnimation(*bg5_select, "Size_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 0, 12, 12);
	CSDCommon::PlayAnimation(*shop_nametag, "Size_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 5000);
	CSDCommon::FreezeMotion(*bg_5, 12);
	CSDCommon::FreezeMotion(*bg5_select, 12);
	CSDCommon::FreezeMotion(*select_1, 0);
}
void Shop::Install()
{
	WRITE_STRING(0x016A8534, "ui_shop_swa");
	/*WRITE_JUMP(0x0043F29C, 0x0043F33E);*/
	WRITE_MEMORY(0x16E14D8, void*, CHudShopRemoveCallback);

	//THANK YOU SONIC TEAM !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	WRITE_JUMP(0x004411B0, 0x004412AD); //Disable Act UI stuff for the skills, fixes a crash caused by setting the amount of items from 6 to 5
	WRITE_MEMORY(0x0044032F, uint8_t, 5);
	WRITE_MEMORY(0x00440355, uint8_t, 5);
	WRITE_MEMORY(0x00440375, uint8_t, 5);
	WRITE_MEMORY(0x00440CAC, uint8_t, 5);
	WRITE_MEMORY(0x00440D73, uint8_t, 5);
	WRITE_MEMORY(0x00440FD6, uint8_t, 5);
	WRITE_MEMORY(0x00440F11, uint8_t, 5);
	WRITE_MEMORY(0x0044102A, uint8_t, 5);
	WRITE_MEMORY(0x00440C50, uint8_t, 5);

	WRITE_JUMP(0x004409D8, ShowBGBuyOnBuy);
	WRITE_JUMP(0x0043F6CB, HideBGBuyOnCancel);
	//WRITE_JUMP(0x0043F55F, HideBGBuyOnCancel);
	INSTALL_HOOK(ShopInit);
	//INSTALL_HOOK(Shop_BuyNewSkillsInit);
	INSTALL_HOOK(SetShopSkillIndex);
	INSTALL_HOOK(sub_43F030);
	INSTALL_HOOK(sub_43F9E0);
	INSTALL_HOOK(sub_442960);
	INSTALL_HOOK(sub_442E20);
	INSTALL_HOOK(sub_442810);
	//INSTALL_HOOK(sub_43FE50);
	INSTALL_HOOK(Shop_UpdateApplication);

}