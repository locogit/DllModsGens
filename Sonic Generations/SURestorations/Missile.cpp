boost::shared_ptr<Sonic::CGameObjectCSD> spLockOn;
Chao::CSD::RCPtr<Chao::CSD::CProject> rcLockOn;
Chao::CSD::RCPtr<Chao::CSD::CScene> cursor_enemy;
bool cursorHidden = true;
float missileTimer;

void KillScreenMissile()
{
	if (spLockOn)
	{
		spLockOn->SendMessage(spLockOn->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spLockOn = nullptr;
	}
}

void __fastcall CHudSonicStageRemoveCallbackMissile(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	KillScreenMissile();
	Chao::CSD::CProject::DestroyScene(rcLockOn.Get(), cursor_enemy);
	rcLockOn = nullptr;
	cursor_enemy = nullptr;
}

void CreateScreenMissile(Sonic::CGameObject* pParentGameObject)
{
	if (rcLockOn && !spLockOn)
		pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spLockOn = boost::make_shared<Sonic::CGameObjectCSD>(rcLockOn, 0.5f, "HUD", false), "main", pParentGameObject);
}

HOOK(void, __fastcall, CHudSonicStageDelayProcessImpMissile, 0x109A8D0, Sonic::CGameObject* This) {
	originalCHudSonicStageDelayProcessImpMissile(This);
	if (!BlueBlurCommon::IsModern()) { return; }
	CHudSonicStageRemoveCallbackMissile(This, nullptr, nullptr);

	Sonic::CCsdDatabaseWrapper wrapperLockOn(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	auto spCsdProjectLockOn = wrapperLockOn.GetCsdProject("ui_lcursor_elauncher");

	size_t& flags = ((size_t*)This)[151];

	rcLockOn = spCsdProjectLockOn->m_rcProject;
	cursor_enemy = rcLockOn->CreateScene("cursor_enemy");
	CSDCommon::FreezeMotion(*cursor_enemy);
	cursor_enemy->SetHideFlag(true);
	cursorHidden = true;

	CreateScreenMissile(This);
}

void Missile::OnHUDUpdate(const hh::fnd::SUpdateInfo& in_rUpdateInfo) {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (!cursorHidden && BlueBlurCommon::IsModern() && cursor_enemy) {
		Hedgehog::Math::CVector& position = Sonic::Player::CPlayerSpeedContext::GetInstance()->m_spMatrixNode->m_Transform.m_Position;
		const auto camera = Sonic::CGameDocument::GetInstance()->GetWorld()->GetCamera();
		hh::math::CVector4 screenPosition = camera->m_MyCamera.m_View * hh::math::CVector4(position.x(), position.y(), position.z(), 1.0f);
		screenPosition = camera->m_MyCamera.m_Projection * screenPosition;
		screenPosition.head<2>() /= screenPosition.w();
		screenPosition.x() = ((screenPosition.x() * 0.5f + 0.5f) * 1280.0f);
		screenPosition.y() = (screenPosition.y() * -0.5f + 0.5f) * 720.0f - 30;
		cursor_enemy->SetPosition(screenPosition.x(), screenPosition.y());
	}
}

HOOK(int, __fastcall, ProcMsgRestartStageMissile, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	if (!cursorHidden && BlueBlurCommon::IsModern() && cursor_enemy) {
		cursorHidden = true;
		cursor_enemy->SetHideFlag(true);
	}
	return originalProcMsgRestartStageMissile(This, Edx, message);
}

HOOK(void, __fastcall, HudResult_MsgStartGoalResultMissile, 0x10B58A0, uint32_t* This, void* Edx, void* message)
{
	originalHudResult_MsgStartGoalResultMissile(This, Edx, message);
	if (!cursorHidden && BlueBlurCommon::IsModern() && cursor_enemy) {
		cursorHidden = true;
		cursor_enemy->SetHideFlag(true);
	}
}

HOOK(__int8, __fastcall, missile, 0x60EFF0, DWORD** This, int a2, int* a3, void* Edx) {
	if (!BlueBlurCommon::IsModern()) { return originalmissile(This, a2, a3, Edx); }
	missileTimer = 1.5f;
	if (cursorHidden && cursor_enemy) {
		cursor_enemy->SetHideFlag(false);
		CSDCommon::PlayAnimation(*cursor_enemy, "Lock_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 0, 1);
		cursorHidden = false;
	}
	return originalmissile(This, a2, a3, Edx);
}

void Missile::OnUpdate(const hh::fnd::SUpdateInfo& updateInfo) {
	if (!BlueBlurCommon::IsModern()) { return; }

	if (missileTimer > 0) { missileTimer -= updateInfo.DeltaTime; }
	if (missileTimer <= 0 && !cursorHidden && cursor_enemy)
	{
		cursorHidden = true;
		cursor_enemy->SetHideFlag(true);
	}
}

void Missile::Install() {
	INSTALL_HOOK(missile);
	INSTALL_HOOK(CHudSonicStageDelayProcessImpMissile);
	INSTALL_HOOK(ProcMsgRestartStageMissile);
	INSTALL_HOOK(HudResult_MsgStartGoalResultMissile);
}