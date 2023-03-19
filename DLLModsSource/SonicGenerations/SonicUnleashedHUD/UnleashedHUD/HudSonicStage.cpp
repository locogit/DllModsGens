#include "HudSonicStage.h"
Chao::CSD::RCPtr<Chao::CSD::CProject> rcPlayScreen;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcPlayerCount;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcTimeCount;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcRingCount;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcSpeedGauge;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcRingEnergyGauge;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcGaugeFrame;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcScoreCount;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcSpeedCount;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcInfoCustom;
boost::shared_ptr<Sonic::CGameObjectCSD> spPlayScreen;

Chao::CSD::RCPtr<Chao::CSD::CProject> rcMissionScreen;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcPosition;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcCountdown;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcItemCount;
boost::shared_ptr<Sonic::CGameObjectCSD> spMissionScreen;

Chao::CSD::RCPtr<Chao::CSD::CProject> rcPlayScreenEv;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcRingGet;
boost::shared_ptr<Sonic::CGameObjectCSD> spPlayScreenEv;

Chao::CSD::RCPtr<Chao::CSD::CProject> rcBossScreen;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcBossGaugeBG;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcBossGauge1;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcBossGauge2;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcBossGaugeBreakPoint;
boost::shared_ptr<Sonic::CGameObjectCSD> spBossScreen;
float bossGauge1Frame;
float bossGauge2Frame;
float boosGauge2Timer;
float bossGaugeBreakPointFrame;

size_t prevRingCount;
bool isMission;
size_t itemCountDenominator;
float speed;
size_t actionCount;
hh::math::CVector2 infoCustomPos;
bool HudSonicStage::scoreEnabled;
int lostRingCount;

float HudSonicStage::xAspectOffset = 0.0f;
float HudSonicStage::yAspectOffset = 0.0f;

boost::shared_ptr<Hedgehog::Sound::CSoundHandle> spSpeed01;
boost::shared_ptr<Hedgehog::Sound::CSoundHandle> spSpeed02[3];
boost::shared_ptr<Hedgehog::Sound::CSoundHandle> spSpeed03;

void CreateScreen(Sonic::CGameObject* pParentGameObject)
{
	if (rcPlayScreen && !spPlayScreen)
		pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spPlayScreen = boost::make_shared<Sonic::CGameObjectCSD>(rcPlayScreen, 0.5f, "HUD_B1", false), "main", pParentGameObject);

	if (rcMissionScreen && !spMissionScreen)
		pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spMissionScreen = boost::make_shared<Sonic::CGameObjectCSD>(rcMissionScreen, 0.5f, "HUD_B1", false), "main", pParentGameObject);

	if (rcPlayScreenEv && !spPlayScreenEv)
		pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spPlayScreenEv = boost::make_shared<Sonic::CGameObjectCSD>(rcPlayScreenEv, 0.5f, "HUD_B1", false), "main", pParentGameObject);
	
	if (rcBossScreen && !spBossScreen)
		pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spBossScreen = boost::make_shared<Sonic::CGameObjectCSD>(rcBossScreen, 0.5f, "HUD_B1", false), "main", pParentGameObject);
}

void KillScreen()
{
	if (spPlayScreen)
	{
		spPlayScreen->SendMessage(spPlayScreen->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spPlayScreen = nullptr;
	}

	if (spMissionScreen)
	{
		spMissionScreen->SendMessage(spMissionScreen->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spMissionScreen = nullptr;
	}

	if (spPlayScreenEv)
	{
		spPlayScreenEv->SendMessage(spPlayScreenEv->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spPlayScreenEv = nullptr;
	}

	if (spBossScreen)
	{
		spBossScreen->SendMessage(spBossScreen->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spBossScreen = nullptr;
	}
}

void ToggleScreen(const bool visible, Sonic::CGameObject* pParentGameObject)
{
	if (visible)
		CreateScreen(pParentGameObject);
	else
		KillScreen();
}

hh::math::CVector2 SetMissionScenePosition(Chao::CSD::CScene* pScene, const size_t index)
{
	char name[4];
	sprintf(name, "%02d", index);
	const auto position = rcPosition->GetNode(name)->GetPosition();
	pScene->SetPosition(position.x(), position.y());
	return position;
}

void FreezeMotion(Chao::CSD::CScene* pScene, bool end = true)
{
	pScene->SetMotionFrame(end ? pScene->m_MotionEndFrame : 0.0f);
	pScene->m_MotionSpeed = 0.0f;
	pScene->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	pScene->m_MotionDisableFlag = true;
}

void GetTime(Sonic::CGameDocument* pGameDocument, size_t* minutes, size_t* seconds, size_t* milliseconds)
{
	static uint32_t pAddr = 0xD61570;
	__asm
	{
		mov ecx, minutes
		mov edi, seconds
		mov esi, milliseconds
		mov eax, pGameDocument
		call[pAddr]
	}
}

float GetTime(Sonic::CGameDocument* pGameDocument)
{
	const auto pMember = (uint8_t*)pGameDocument->m_pMember;
	return max(0, max(0, *(float*)(pMember + 0x184)) + *(float*)(pMember + 0x18C));
}

void HandleInfoCustom(const Chao::CSD::RCPtr<Chao::CSD::CScene>& rcScene)
{
	constexpr float scale = 0.9f;

	if (const auto rcIconCustom = rcScene->GetNode("icon_custom_0"))
		rcIconCustom->SetScale(scale, scale);

	if (const auto rcIconCustom = rcScene->GetNode("icon_chao_5"))
		rcIconCustom->SetScale(scale, scale);

	rcScene->GetNode("position")->SetScale(0.8f, 0.8f);
	rcScene->GetNode("bg")->SetHideFlag(true);
	rcScene->GetNode("icon_btn")->SetHideFlag(true);
	rcScene->GetNode("brilliance")->SetHideFlag(true);
	rcScene->SetPosition(infoCustomPos.x() + (rcCountdown ? 34.0f : -15.0f), infoCustomPos.y() - 103.0f);
}

const Chao::CSD::RCPtr<Chao::CSD::CScene>& GetGpSonicSafeScene(void* This)
{
	return *(Chao::CSD::RCPtr<Chao::CSD::CScene>*)(*(char**)((char*)This + 0xAC) + 0x14);
}

void CreateRingGet()
{
	if (rcRingGet)
		return;

	rcRingGet = rcPlayScreenEv->CreateScene("ring_get");
	if (rcCountdown)
		rcRingGet->GetNode("ring_img")->SetScale(0.72f, 0.72f);

	FreezeMotion(rcRingGet.Get());
}

void SetRingGetPosition(const hh::math::CVector2& position, float offset = 0.0f)
{
	if (rcRingGet)
		rcRingGet->SetPosition(position.x() + offset - (rcCountdown ? 106 : 128), position.y() - (rcCountdown ? 199.5f : 200));
}

void __declspec(naked) GetScoreEnabled()
{
	static uint32_t returnAddress = 0x109C254;
	__asm
	{
		mov	HudSonicStage::scoreEnabled, 1
		jmp[returnAddress]
	}
}

HOOK(void, __fastcall, ProcMsgSetPinballHud, 0x1095D40, Sonic::CGameObject* This, void* Edx, hh::fnd::Message& in_rMsg)
{
	// Update score
	if ((*((char*)&in_rMsg + 16) & 1) && rcScoreCount)
	{
		char text[32];
		sprintf(text, "%08d", *(size_t*)((char*)&in_rMsg + 20));
		rcScoreCount->GetNode("score")->SetText(text);
		rcScoreCount->Update();
	}

	originalProcMsgSetPinballHud(This, Edx, in_rMsg);
}

void __fastcall CHudSonicStageRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	KillScreen();

	Chao::CSD::CProject::DestroyScene(rcPlayScreen.Get(), rcPlayerCount);

	if (isMission)
		Chao::CSD::CProject::DestroyScene(rcMissionScreen.Get(), rcTimeCount);
	else
		Chao::CSD::CProject::DestroyScene(rcPlayScreen.Get(), rcTimeCount);

	if (isMission && !rcSpeedGauge)
		Chao::CSD::CProject::DestroyScene(rcMissionScreen.Get(), rcRingCount);
	else
		Chao::CSD::CProject::DestroyScene(rcPlayScreen.Get(), rcRingCount);

	Chao::CSD::CProject::DestroyScene(rcPlayScreen.Get(), rcSpeedGauge);
	Chao::CSD::CProject::DestroyScene(rcPlayScreen.Get(), rcRingEnergyGauge);
	Chao::CSD::CProject::DestroyScene(rcPlayScreen.Get(), rcGaugeFrame);
	Chao::CSD::CProject::DestroyScene(rcPlayScreen.Get(), rcScoreCount);

	if (rcSpeedCount)
		Chao::CSD::CProject::DestroyScene(rcPlayScreen.Get(), rcSpeedCount);

	if (isMission)
		Chao::CSD::CProject::DestroyScene(rcMissionScreen.Get(), rcInfoCustom);
	else
		Chao::CSD::CProject::DestroyScene(rcPlayScreen.Get(), rcInfoCustom);

	Chao::CSD::CProject::DestroyScene(rcMissionScreen.Get(), rcPosition);
	Chao::CSD::CProject::DestroyScene(rcMissionScreen.Get(), rcCountdown);
	Chao::CSD::CProject::DestroyScene(rcMissionScreen.Get(), rcItemCount);

	Chao::CSD::CProject::DestroyScene(rcPlayScreenEv.Get(), rcRingGet);

	Chao::CSD::CProject::DestroyScene(rcBossScreen.Get(), rcBossGaugeBG);
	Chao::CSD::CProject::DestroyScene(rcBossScreen.Get(), rcBossGauge1);
	Chao::CSD::CProject::DestroyScene(rcBossScreen.Get(), rcBossGauge2);
	Chao::CSD::CProject::DestroyScene(rcBossScreen.Get(), rcBossGaugeBreakPoint);

	rcPlayScreen = nullptr;
	rcMissionScreen = nullptr;
	rcPlayScreenEv = nullptr;
	rcBossScreen = nullptr;
	isMission = false;
	actionCount = 1;
}

HOOK(void, __fastcall, ProcMsgGetMissionLimitTime, 0xD0F0E0, Sonic::CGameObject* This, void* Edx, hh::fnd::Message& in_rMsg)
{
	originalProcMsgGetMissionLimitTime(This, Edx, in_rMsg);
	if (!rcCountdown)
		return;

	const float limitTime = *(float*)((char*)&in_rMsg + 16);
	const float elapsedTime = GetTime(**This->m_pMember->m_pGameDocument);
	const float remainingTime = limitTime - elapsedTime;

	char text[16];
	sprintf(text, "%02d", (int)(remainingTime * 100.0f) % 100);
	rcCountdown->GetNode("time001_l")->SetText(text);

	sprintf(text, "%02d", (int)remainingTime % 60);
	rcCountdown->GetNode("time010_l")->SetText(text);

	sprintf(text, "%02d", (int)(remainingTime / 60));
	rcCountdown->GetNode("time100_l")->SetText(text);

	rcCountdown->SetMotionFrame(elapsedTime / limitTime * rcCountdown->m_MotionEndFrame);
}

HOOK(void, __fastcall, ProcMsgGetMissionCondition, 0xD0F130, Sonic::CGameObject* This, void* Edx, hh::fnd::Message& in_rMsg)
{
	originalProcMsgGetMissionCondition(This, Edx, in_rMsg);
	itemCountDenominator = *(size_t*)((char*)&in_rMsg + 20);
}

HOOK(void, __fastcall, ProcMsgNotifyLapTimeHud, 0x1097640, Sonic::CGameObject* This, void* Edx, hh::fnd::Message& in_rMsg)
{
	if (!rcPlayScreen)
		return;

	rcSpeedCount = rcPlayScreen->CreateScene("speed_count");
	rcSpeedCount->SetPosition(HudSonicStage::xAspectOffset, 0);
	rcSpeedCount->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;

	const auto playerContext = Sonic::Player::CPlayerSpeedContext::GetInstance();
	speed = playerContext->GetVelocity().norm() / (playerContext->m_Is2DMode ? 45.0f : 90.0f) * 1000.0f;

	spSpeed01 = nullptr;
	for (size_t i = 0; i < _countof(spSpeed02); i++)
		spSpeed02[i] = nullptr;

	spSpeed03 = nullptr;
}

HOOK(void, __fastcall, CCountdownUpdate, 0x124F360, void* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	GetGpSonicSafeScene(This)->SetHideFlag(true);
	originalCCountdownUpdate(This, Edx, in_rUpdateInfo);
}

HOOK(void, __fastcall, CLastBossGaugeNewUpdate, 0x124E930, void* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	HandleInfoCustom(GetGpSonicSafeScene(This));
	originalCLastBossGaugeNewUpdate(This, Edx, in_rUpdateInfo);
}

HOOK(void, __fastcall, ProcMsgChangeCustomHud, 0x1096FF0, Sonic::CGameObject* This, void* Edx, hh::fnd::Message& in_rMsg)
{
	size_t* spriteIndex = (size_t*)((char*)&in_rMsg + 16);
	size_t* spriteCount = (size_t*)((char*)&in_rMsg + 20);

	if (rcInfoCustom)
	{
		actionCount = *spriteCount > 0 ? max(actionCount, *spriteCount) : 0;

		char text[16];

		sprintf(text, "%d", *spriteCount);
		rcInfoCustom->GetNode("num_nume")->SetText(text);

		sprintf(text, "%d", actionCount);
		rcInfoCustom->GetNode("num_deno")->SetText(text);
	}

	*spriteCount = *spriteCount > 0 ? 1 : 0;

	originalProcMsgChangeCustomHud(This, Edx, in_rMsg);
}

HOOK(void, __fastcall, CHudSonicStageDelayProcessImp, 0x109A8D0, Sonic::CGameObject* This)
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	lostRingCount = 0;

	HudSonicStage::scoreEnabled = false;
	originalCHudSonicStageDelayProcessImp(This);
	CHudSonicStageRemoveCallback(This, nullptr, nullptr);

	HudSonicStage::CalculateAspectOffsets();

	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	auto spCsdProject = wrapper.GetCsdProject("ui_playscreen");
	rcPlayScreen = spCsdProject->m_rcProject;

	spCsdProject = wrapper.GetCsdProject("ui_missionscreen");
	rcMissionScreen = spCsdProject->m_rcProject;

	spCsdProject = wrapper.GetCsdProject("ui_playscreen_ev");
	rcPlayScreenEv = spCsdProject->m_rcProject;

	spCsdProject = wrapper.GetCsdProject("ui_boss_gauge");
	rcBossScreen = spCsdProject->m_rcProject;

	rcPosition = rcMissionScreen->CreateScene("position");
	rcPosition->SetPosition(0, 0);

	isMission = Common::IsCurrentStageMission();

	size_t& flags = ((size_t*)This)[151];

	float offset = 0.0f;

	if (flags & 0x1 && *(uint8_t*)0x1098C82 != 0xEB) // Lives (accounts for Disable Lives patch)
	{
		rcPlayerCount = rcPlayScreen->CreateScene("player_count");
		rcPlayerCount->SetPosition(0.0f, 0.0f);
	}
	else
		offset = -50.0f;

	if (flags & 0x2000) // Countdown
	{
		rcCountdown = rcMissionScreen->CreateScene("time_count", "conditional_timer_so");
		FreezeMotion(rcCountdown.Get());
	}

	if (!rcCountdown)
	{
		if (flags & 0x2) // Time
		{
			if (isMission)
				rcTimeCount = rcMissionScreen->CreateScene("time_count", "normal_so");
			else
			{
				rcTimeCount = rcPlayScreen->CreateScene("time_count");
				rcTimeCount->SetPosition(0, offset);
			}
		}
	}
	else
	{
		offset += -50.0f;
	}

	if (flags & 0x20000) // Mission Target
	{
		rcItemCount = rcMissionScreen->CreateScene("item_count", "conditional_meet_so");
		FreezeMotion(rcItemCount.Get());

		rcItemCount->GetNode("icons")->SetHideFlag(true);

		char text[16];
		sprintf(text, "%03d", itemCountDenominator);
		rcItemCount->GetNode("num_deno")->SetText(text);
	}

	if (flags & 0x200) // Boost Gauge
	{
		rcSpeedGauge = rcPlayScreen->CreateScene("so_speed_gauge");
		rcRingEnergyGauge = rcPlayScreen->CreateScene("so_ringenagy_gauge");
		rcGaugeFrame = rcPlayScreen->CreateScene("gauge_frame");

		rcSpeedGauge->SetPosition(0, HudSonicStage::yAspectOffset);
		rcRingEnergyGauge->SetPosition(0, HudSonicStage::yAspectOffset);
		rcGaugeFrame->SetPosition(0, HudSonicStage::yAspectOffset);

		FreezeMotion(rcSpeedGauge.Get());
		FreezeMotion(rcRingEnergyGauge.Get());
		FreezeMotion(rcGaugeFrame.Get());

		// Disable ring drop
		WRITE_JUMP(0xE66005, (void*)0xE66210);
	}
	else
	{
		WRITE_MEMORY(0xE66005, uint8_t, 0x80, 0xBF, 0x72, 0x01, 0x00);
	}

	if (HudSonicStage::scoreEnabled) // Score
	{
		if (isMission)
		{
			if (rcCountdown) offset += 50.0f;
			if (rcItemCount) offset += 50.0f;
		}

		rcScoreCount = rcPlayScreen->CreateScene("score_count");
		rcScoreCount->SetPosition(0, offset);
	}

	if (flags & 0x400204 || Common::GetCurrentStageID() == SMT_bsd) // Rings
	{
		// Re-purpose score for Classic
		if (!rcSpeedGauge)
		{
			if (isMission)
				rcRingCount = rcMissionScreen->CreateScene("score_count", rcCountdown ? "conditional_meet_so" : "normal_so");
			else
				rcRingCount = rcPlayScreen->CreateScene("score_count");

			rcRingCount->GetNode("txt")->SetPatternIndex(1);

			if (const auto rcTxtLarge = rcRingCount->GetNode("txt_l"))
				rcTxtLarge->SetPatternIndex(1);

			CreateRingGet();

			rcRingCount->SetPosition(0, offset + (rcScoreCount ? 50 : 0));
			rcRingGet->SetPosition(0, offset + (rcScoreCount ? 50 : 0));

			FreezeMotion(rcRingCount.Get(), false);
		}
		else
		{
			rcRingCount = rcPlayScreen->CreateScene("ring_count");
			rcRingCount->SetPosition(0, HudSonicStage::yAspectOffset);
		}
	}

	if (isMission)
	{
		rcInfoCustom = rcMissionScreen->CreateScene(rcCountdown ? "item_count" : "item_count_s", "conditional_meet_so");
		rcInfoCustom->SetHideFlag(true);
		rcInfoCustom->GetNode("txt")->SetPatternIndex(1);

		if (const auto rcIcons = rcInfoCustom->GetNode("icons"))
			rcIcons->SetHideFlag(true);
	}
	else
	{
		rcInfoCustom = rcPlayScreen->CreateScene("item_count", "conditional_meet_so");

		float infoCustomOffset = offset + (rcScoreCount ? 50 : 0);
		infoCustomOffset += !rcSpeedGauge && rcRingCount ? 50 : 0;

		infoCustomPos.x() = 128.0f;
		infoCustomPos.y() = 200 + infoCustomOffset;

		rcInfoCustom->SetPosition(0, infoCustomOffset);
	}

	FreezeMotion(rcInfoCustom.Get(), false);

	if (Common::IsCurrentStageBossBattle() && (Common::GetCurrentStageID() & 0xFF) != SMT_bsd)
	{
		rcBossGaugeBG = rcBossScreen->CreateScene("gauge_bg");
		rcBossGaugeBG->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		rcBossGaugeBG->SetPosition(HudSonicStage::xAspectOffset, 0);

		rcBossGauge1 = rcBossScreen->CreateScene("gauge_1");
		rcBossGauge1->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		rcBossGauge1->SetPosition(HudSonicStage::xAspectOffset, 0);

		rcBossGauge2 = rcBossScreen->CreateScene("gauge_2");
		rcBossGauge2->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		rcBossGauge2->SetPosition(HudSonicStage::xAspectOffset, 0);

		rcBossGaugeBreakPoint = rcBossScreen->CreateScene("gauge_breakpoint");
		rcBossGaugeBreakPoint->SetMotion("position");
		rcBossGaugeBreakPoint->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		rcBossGaugeBreakPoint->SetPosition(HudSonicStage::xAspectOffset, 0);
		rcBossGaugeBreakPoint->SetHideFlag(true);

		bossGauge1Frame = 100.0f;
		bossGauge2Frame = 100.0f;
		boosGauge2Timer = 0.0f;
		bossGaugeBreakPointFrame = 0.0f;
	}

	flags &= ~(0x1 | 0x2 | 0x400004 | 0x200 | 0x800 | 0x1000000); // Mask to prevent crash when game tries accessing the elements we disabled later on

	CreateScreen(This);
}

class CObjDropRing : public Sonic::CGameObject
{
	hh::math::CMatrix44 m_Transform;
	hh::math::CQuaternion m_Rotation;
	hh::math::CQuaternion m_TargetRotation;

	hh::math::CVector2 m_2DPosition;
	hh::math::CVector2 m_2DVelocity;
	float m_LifeTime{};
	boost::shared_ptr<hh::mr::CSingleElement> m_spModel;

public:
	CObjDropRing()
	{
		const auto spCamera = Sonic::CGameDocument::GetInstance()->GetWorld()->GetCamera();
		const hh::math::CMatrix viewTransform = spCamera->m_MyCamera.m_View * hh::math::CMatrix::Identity();
		m_TargetRotation = viewTransform.rotation().inverse();
		m_Transform = spCamera->m_MyCamera.m_Projection * viewTransform.matrix();
		m_Transform.normalize();
		m_Rotation = m_TargetRotation;
	}

	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& worldHolder, Sonic::CGameDocument* pGameDocument,
		const boost::shared_ptr<Hedgehog::Database::CDatabase>& spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		pGameDocument->AddUpdateUnit("f", this);

		m_spModel = boost::make_shared<hh::mr::CSingleElement>(hh::mr::CMirageDatabaseWrapper(spDatabase.get()).GetModelData("cmn_obj_ring_HD"));
		AddRenderable("Sparkle_FB", m_spModel, false);

		constexpr float speed = 4.2f;
		float angle = ((float)std::rand() / RAND_MAX) * PI;
		float width = (float)*(size_t*)0x1DFDDDC;
		float height = (float)*(size_t*)0x1DFDDE0;
		m_2DVelocity = hh::math::CVector2(std::cosf(angle) / width * height, std::sinf(angle)) * speed;

		m_2DPosition = hh::math::CVector2(-0.7765625f, -0.7833333333333333f);
		m_LifeTime = 0.0f;
	}

	void UpdateParallel(const Hedgehog::Universe::SUpdateInfo& updateInfo) override
	{
		const auto spCamera = m_pMember->m_pGameDocument->GetWorld()->GetCamera();

		const hh::math::CMatrix44 invProj = spCamera->m_MyCamera.m_Projection.inverse();
		const hh::math::CMatrix invView = spCamera->m_MyCamera.m_View.inverse();

		constexpr float gravity = -9.81f;
		Hedgehog::Math::CVector2 const velPrev = m_2DVelocity;
		m_2DVelocity += Hedgehog::Math::CVector2::UnitY() * gravity * updateInfo.DeltaTime;
		m_2DPosition += (velPrev + m_2DVelocity) * 0.5f * updateInfo.DeltaTime;

		auto& rTransform = m_spModel->m_spInstanceInfo->m_Transform;
		auto& rMatrix = rTransform.matrix();

		rTransform = m_Transform;

		const float scale = max(rMatrix.col(0).head<3>().norm(),
			max(rMatrix.col(1).head<3>().norm(), rMatrix.col(2).head<3>().norm()));

		rMatrix.col(0) /= (scale / 0.2f);
		rMatrix.col(1) /= (scale / 0.2f);
		rMatrix.col(2) /= (scale / 0.2f);

		rTransform(0, 3) = m_2DPosition.x();
		rTransform(1, 3) = m_2DPosition.y();
		rTransform(2, 3) = 0.1f;
		rTransform(3, 3) = 1.0f;

		rTransform = invProj * rTransform.matrix();
		rTransform.rotate(m_Rotation);
		rTransform = invView * rTransform;

		(&rTransform)[1] = rTransform;

		m_Rotation = m_TargetRotation;

		m_LifeTime += updateInfo.DeltaTime;
		if (m_LifeTime > 2.0f)
		{
			SendMessage(m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		}
	}

	bool ProcessMessage(Hedgehog::Universe::Message& message,bool flag) override
	{
		if (flag)
		{
			if (std::strstr(message.GetType(), "MsgRestartStage") != nullptr
			 || std::strstr(message.GetType(), "MsgStageClear") != nullptr)
			{
				SendMessage(m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
				return true;
			}
		}

		return Sonic::CGameObject::ProcessMessage(message, flag);
	}
};

HOOK(void, __fastcall, CHudSonicStageUpdateParallel, 0x1098A50, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	originalCHudSonicStageUpdateParallel(This, Edx, in_rUpdateInfo);

	ToggleScreen(*(bool*)0x1A430D8, This); // ms_IsRenderGameMainHud

	if (!spPlayScreen)
		return;

	char text[256];
	size_t rowIndex = 1;

	const size_t flags = ((size_t*)This)[151];

	if (rcPlayerCount)
	{
		const size_t liveCountAddr = Common::GetMultiLevelAddress(0x1E66B34, { 0x4, 0x1B4, 0x7C, 0x9FDC });
		if (liveCountAddr)
		{
			sprintf(text, "%02d", max(0, *(int*)liveCountAddr));
			rcPlayerCount->GetNode("player")->SetText(text);
		}
	}

	if (rcTimeCount)
	{
		size_t minutes, seconds, milliseconds;
		GetTime(**This->m_pMember->m_pGameDocument, &minutes, &seconds, &milliseconds);

		sprintf(text, "%02d", milliseconds);
		rcTimeCount->GetNode("time001")->SetText(text);

		sprintf(text, "%02d", seconds);
		rcTimeCount->GetNode("time010")->SetText(text);

		sprintf(text, "%02d", minutes);
		rcTimeCount->GetNode("time100")->SetText(text);

		if (rcPlayerCount)
			rowIndex++;

		if (isMission)
			SetMissionScenePosition(rcTimeCount.Get(), rowIndex++);
	}

	if (rcCountdown)
		SetMissionScenePosition(rcCountdown.Get(), rowIndex++);

	const auto playerContext = Sonic::Player::CPlayerSpeedContext::GetInstance();

	if (rcSpeedGauge && playerContext)
		rcSpeedGauge->SetMotionFrame(playerContext->m_HorizontalVelocity.norm() / (playerContext->m_Is2DMode ? 45.0f : 90.0f) * 100.0f);

	float gaugeSize = 50.0f;

	if (rcRingEnergyGauge && playerContext)
	{
		rcRingEnergyGauge->SetMotion("total_quantity");
		rcRingEnergyGauge->SetMotionFrame(gaugeSize);
		rcRingEnergyGauge->Update(0.0f);

		playerContext->m_ChaosEnergy = min(playerContext->m_ChaosEnergy, playerContext->GetMaxChaosEnergy());

		rcRingEnergyGauge->SetMotion("size");
		rcRingEnergyGauge->SetMotionFrame(playerContext->m_ChaosEnergy / playerContext->GetMaxChaosEnergy() * 100.0f);
		rcRingEnergyGauge->Update(0.0f);
	}

	if (rcGaugeFrame)
		rcGaugeFrame->SetMotionFrame(gaugeSize);

	if (rcRingCount && playerContext)
	{
		sprintf(text, "%03d", playerContext->m_RingCount);

		// Classic
		if (!rcSpeedGauge)
		{
			rcRingCount->GetNode("score")->SetText(text);

			if (const auto rcScoreLarge = rcRingCount->GetNode("score_l"))
				rcScoreLarge->SetText(text);

			if (isMission)
				SetRingGetPosition(SetMissionScenePosition(rcRingCount.Get(), rowIndex++));
		}

		// Modern
		else
			rcRingCount->GetNode("num_ring")->SetText(text);
	}

	if (rcRingGet && playerContext && prevRingCount < playerContext->m_RingCount && rcRingGet->m_MotionDisableFlag)
	{
		rcRingGet->SetMotion("get_Anim");
		rcRingGet->SetMotionFrame(0.0f);
		rcRingGet->m_MotionDisableFlag = false;
		rcRingGet->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		rcRingGet->m_MotionSpeed = 1.0f;
		rcRingGet->Update();
	}

	if (playerContext)
		prevRingCount = playerContext->m_RingCount;
	else
		prevRingCount = 0;

	if (rcInfoCustom)
	{
		const bool multiple = *(size_t*)((char*)This + 572) > 0;
		const bool single = *(bool*)((char*)This + 580) || strcmp((const char*)0x1E774D4, "blb") == 0;
		const bool visible = multiple || single;

		rcInfoCustom->SetHideFlag(!visible);

		if (visible)
		{
			if (single && !(flags & 0x100)) // Ignore Cream mission
			{
				rcInfoCustom->GetNode("num_nume")->SetText("1");
				rcInfoCustom->GetNode("num_deno")->SetText("1");
			}

			if (isMission)
				infoCustomPos = SetMissionScenePosition(rcInfoCustom.Get(), rowIndex++);

			for (size_t i = 0; i < 2; i++)
			{
				const auto& rcScene = ((Chao::CSD::RCPtr<Chao::CSD::CScene>*)((char*)This + 0x1C8))[i];
				if (rcScene)
					HandleInfoCustom(rcScene);
			}
		}
	}

	if (rcItemCount)
	{
		const size_t count = *(size_t*)((char*)This + 0x300);
		sprintf(text, "%03d", count);
		rcItemCount->GetNode("num_nume")->SetText(text);
		rcItemCount->SetMotionFrame(count >= itemCountDenominator ? rcItemCount->m_MotionEndFrame : 0.0f);

		const auto position = SetMissionScenePosition(rcItemCount.Get(), rowIndex++);

		// Mission ring count (shamelessly taken from Brian)
		const bool missionRingCount = *(bool*)((uint32_t)This + 620);
		
		if (missionRingCount)
		{
			const size_t prevMissionRingCount = *(size_t*)((char*)This + 768);
			if (prevMissionRingCount != playerContext->m_RingCount)
			{
				*(size_t*)((char*)This + 772) = prevMissionRingCount;
				*(size_t*)((char*)This + 768) = playerContext->m_RingCount;
				*(size_t*)((char*)This + 764) = 1;
			}

			// We don't need the actual ring count if it's shown here anyway
			if (rcRingCount && !rcSpeedGauge)
			{
				if (isMission)
					Chao::CSD::CProject::DestroyScene(rcMissionScreen.Get(), rcRingCount);
				else
					Chao::CSD::CProject::DestroyScene(rcPlayScreen.Get(), rcRingCount);
			}

			CreateRingGet();
			SetRingGetPosition(position, 32);
		}

		for (size_t i = 0; i < 2; i++)
		{
			const auto& rcMissionTarget = ((Chao::CSD::RCPtr<Chao::CSD::CScene>*)((char*)This + 0x130))[i];
			if (!rcMissionTarget)
				continue;

			if (missionRingCount)
			{
				rcMissionTarget->SetHideFlag(true);
				continue;
			}

			rcMissionTarget->GetNode("bg")->SetHideFlag(true);
			rcMissionTarget->GetNode("SXY")->SetHideFlag(true);
			rcMissionTarget->GetNode("num_deno")->SetHideFlag(true);
			rcMissionTarget->GetNode("img_slash")->SetHideFlag(true);

			const auto rcImgIcon = rcMissionTarget->GetNode("img_icon");
			rcImgIcon->SetPosition(0, 0);
			rcMissionTarget->Update();
			const auto position = rcItemCount->GetNode("ring")->GetPosition() - rcImgIcon->GetPosition();
			rcImgIcon->SetPosition(position.x(), position.y());
		}
	}

	if (rcSpeedCount)
	{
		if (rcSpeedCount->m_MotionDisableFlag)
			Chao::CSD::CProject::DestroyScene(rcPlayScreen.Get(), rcSpeedCount);

		else
		{
			const float frames[] = { 87, 105, 123, 145 };

			if (rcSpeedCount->m_MotionFrame < frames[0])
			{
				if (!spSpeed01)
					spSpeed01 = playerContext->PlaySound(1000016, 0);
			}
			else
				spSpeed01 = nullptr;

			sprintf(text, " %03d", rand() % 1000);
			sprintf(text + 16, "%04d", (int)speed);

			for (size_t i = 0; i < 4; i++)
			{
				if (rcSpeedCount->m_MotionFrame > frames[i])
				{
					text[3 - i] = text[19 - i];

					auto& rHandle = i < 3 ? spSpeed02[i] : spSpeed03;
					if (!rHandle)
						rHandle = playerContext->PlaySound(i < 3 ? 1000017 : 1000018, 0);
				}
			}

			char* pText = text[0] == ' ' ? text + 1 : text;

			rcSpeedCount->GetNode("num_speed")->SetText(pText);
			rcSpeedCount->GetNode("num_speed_deep")->SetText(pText);
			rcSpeedCount->GetNode("num_speed_pale")->SetText(pText);
		}
	}

	// Hide pin_score
	if (const auto rcPinScore = *(Chao::CSD::RCPtr<Chao::CSD::CScene>*)((char*)This + 0x128))
		rcPinScore->SetHideFlag(true);

	// Handle lost rings
	if (lostRingCount > 0 && rcRingEnergyGauge)
	{
		lostRingCount--;
		This->m_pMember->m_pGameDocument->AddGameObject(boost::make_shared<CObjDropRing>());
	}

	// Handle boss health
	if (rcBossGauge1 && rcBossGauge1->m_MotionDisableFlag)
	{
		rcBossGauge1->SetMotion("Size_Anim");
		rcBossGauge1->SetMotionFrame(bossGauge1Frame);
		rcBossGauge1->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		rcBossGauge1->m_MotionSpeed = 0.0f;
		rcBossGauge1->Update();
		
		if (bossGaugeBreakPointFrame > 0.0f)
		{
			rcBossGaugeBreakPoint->SetHideFlag(false);
			rcBossGaugeBreakPoint->SetMotion("position");
			rcBossGaugeBreakPoint->SetMotionFrame(bossGaugeBreakPointFrame);
			rcBossGaugeBreakPoint->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			rcBossGaugeBreakPoint->m_MotionSpeed = 0.0f;
			rcBossGaugeBreakPoint->Update();
		}

		if (bossGauge1Frame != bossGauge2Frame)
		{
			boosGauge2Timer += in_rUpdateInfo.DeltaTime;
			if (boosGauge2Timer > 0.5f)
			{
				bossGauge2Frame = max(bossGauge1Frame, bossGauge2Frame - in_rUpdateInfo.DeltaTime * 30.0f);
				rcBossGauge2->SetMotion("Size_Anim");
				rcBossGauge2->SetMotionFrame(bossGauge2Frame);
				rcBossGauge2->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				rcBossGauge2->m_MotionSpeed = 0.0f;
				rcBossGauge2->Update();
			}
		}
		else
		{
			boosGauge2Timer = 0.0f;
		}
	}
}

class CObjGetItem : public Sonic::CGameObject
{
	hh::math::CMatrix44 m_Transform;
	hh::math::CQuaternion m_Rotation;
	hh::math::CQuaternion m_TargetRotation;
	float m_Factor{};
	boost::shared_ptr<hh::mr::CSingleElement> m_spModel;

public:
	CObjGetItem(const hh::math::CMatrix& in_rTransform) : m_Rotation(hh::math::CQuaternion::Identity())
	{
		const auto spCamera = Sonic::CGameDocument::GetInstance()->GetWorld()->GetCamera();
		const hh::math::CMatrix viewTransform = spCamera->m_MyCamera.m_View * in_rTransform;
		m_TargetRotation = viewTransform.rotation().inverse();
		m_Transform = spCamera->m_MyCamera.m_Projection * viewTransform.matrix();
		m_Transform.normalize();
	}

	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& worldHolder, Sonic::CGameDocument* pGameDocument,
		const boost::shared_ptr<Hedgehog::Database::CDatabase>& spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		pGameDocument->AddUpdateUnit("f", this);

		m_spModel = boost::make_shared<hh::mr::CSingleElement>(hh::mr::CMirageDatabaseWrapper(spDatabase.get()).GetModelData("cmn_obj_ring_HD"));
		AddRenderable("Sparkle_FB", m_spModel, false);
	}

	void UpdateParallel(const Hedgehog::Universe::SUpdateInfo& updateInfo) override
	{
		const auto spCamera = m_pMember->m_pGameDocument->GetWorld()->GetCamera();

		const hh::math::CMatrix44 invProj = spCamera->m_MyCamera.m_Projection.inverse();
		const hh::math::CMatrix invView = spCamera->m_MyCamera.m_View.inverse();

		auto& rTransform = m_spModel->m_spInstanceInfo->m_Transform;
		auto& rMatrix = rTransform.matrix();

#define LERP(a, b) ((1.0f - m_Factor) * (a) + (b) * m_Factor)

		rTransform = m_Transform;

		const float scale = max(rMatrix.col(0).head<3>().norm(),
			max(rMatrix.col(1).head<3>().norm(), rMatrix.col(2).head<3>().norm()));

		rMatrix.col(0) /= LERP(1.0f, scale / 0.2f);
		rMatrix.col(1) /= LERP(1.0f, scale / 0.2f);
		rMatrix.col(2) /= LERP(1.0f, scale / 0.2f);

		rTransform(0, 3) = LERP(rTransform(0, 3), -0.7765625f);
		rTransform(1, 3) = LERP(rTransform(1, 3), -0.7833333333333333f);
		rTransform(2, 3) = LERP(rTransform(2, 3), 0.1f);
		rTransform(3, 3) = LERP(rTransform(3, 3), 1.0f);

		rTransform = invProj * rTransform.matrix();
		rTransform.rotate(m_Rotation);
		rTransform = invView * rTransform;

		(&rTransform)[1] = rTransform;

#undef LERP

		m_Rotation = m_Rotation.slerp(updateInfo.DeltaTime * 6.0f, m_TargetRotation);

		m_Factor += updateInfo.DeltaTime * 0.125f;
		m_Factor *= 1.2732395f;

		if (m_Factor > 1.0f)
		{
			SendMessage(m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());

			const auto rcScene = spPlayScreen->m_rcProject->CreateScene("ring_get");
			rcScene->SetPosition(0, HudSonicStage::yAspectOffset);
			rcScene->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayThenDestroy;
		}
	}

	bool ProcessMessage(Hedgehog::Universe::Message& message,bool flag) override
	{
		if (flag)
		{
			if (std::strstr(message.GetType(), "MsgRestartStage") != nullptr
			 || std::strstr(message.GetType(), "MsgStageClear") != nullptr)
			{
				SendMessage(m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
				return true;
			}
		}

		return Sonic::CGameObject::ProcessMessage(message, flag);
	}
};

class CObjGetLife : public Sonic::CGameObject
{
	hh::math::CMatrix44 m_Transform;
	hh::math::CQuaternion m_Rotation;
	hh::math::CQuaternion m_TargetRotation;
	float m_Factor{};
	float m_LifeTime{};
	boost::shared_ptr<hh::mr::CSingleElement> m_spModel;

public:
	CObjGetLife() : m_Rotation(hh::math::CQuaternion(0, 0, 1, 0))
	{
		const auto spCamera = Sonic::CGameDocument::GetInstance()->GetWorld()->GetCamera();
		auto* context = Sonic::Player::CPlayerSpeedContext::GetInstance();
		const hh::math::CMatrix viewTransform = spCamera->m_MyCamera.m_View * (Eigen::Translation3f(context->m_spMatrixNode->m_Transform.m_Position) * hh::math::CQuaternion::Identity());
		m_TargetRotation = viewTransform.rotation().inverse();
		m_Transform = spCamera->m_MyCamera.m_Projection * viewTransform.matrix();
		m_Transform.normalize();
		const hh::math::CMatrix viewTransform2 = hh::math::CQuaternion(0, 0, 1, 0) * spCamera->m_MyCamera.m_View;
		m_Rotation = viewTransform2.rotation().inverse();
		m_LifeTime = 0.0f;
	}

	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& worldHolder, Sonic::CGameDocument* pGameDocument,
		const boost::shared_ptr<Hedgehog::Database::CDatabase>& spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		pGameDocument->AddUpdateUnit("f", this);

		m_spModel = boost::make_shared<hh::mr::CSingleElement>(hh::mr::CMirageDatabaseWrapper(spDatabase.get()).GetModelData("cmn_obj_oneup_HUD"));
		AddRenderable("Sparkle_FB", m_spModel, false);
	}

	void UpdateParallel(const Hedgehog::Universe::SUpdateInfo& updateInfo) override
	{
		const auto spCamera = m_pMember->m_pGameDocument->GetWorld()->GetCamera();

		const hh::math::CMatrix44 invProj = spCamera->m_MyCamera.m_Projection.inverse();
		const hh::math::CMatrix invView = spCamera->m_MyCamera.m_View.inverse();

		auto& rTransform = m_spModel->m_spInstanceInfo->m_Transform;
		auto& rMatrix = rTransform.matrix();

#define LERP(a, b) ((1.0f - m_Factor) * (a) + (b) * m_Factor)

		rTransform = m_Transform;

		const float scale = max(rMatrix.col(0).head<3>().norm(),
			max(rMatrix.col(1).head<3>().norm(), rMatrix.col(2).head<3>().norm()));

		rMatrix.col(0) /= LERP(1.0f, scale / 0.33f);
		rMatrix.col(1) /= LERP(1.0f, scale / 0.33f);
		rMatrix.col(2) /= LERP(1.0f, scale / 0.33f);

		rTransform(0, 3) = LERP(rTransform(0, 3), 0.0f);
		rTransform(1, 3) = LERP(rTransform(1, 3), 0.6018518518518519f);
		rTransform(2, 3) = LERP(rTransform(2, 3), 0.1f);
		rTransform(3, 3) = LERP(rTransform(3, 3), 1.0f);

		rTransform = invProj * rTransform.matrix();
		rTransform.rotate(m_Rotation);
		rTransform = invView * rTransform;

		(&rTransform)[1] = rTransform;

#undef LERP

		m_Rotation = m_Rotation.slerp(updateInfo.DeltaTime * 3.0f, m_TargetRotation);

		m_LifeTime += updateInfo.DeltaTime;
		m_Factor += updateInfo.DeltaTime * 0.125f;
		m_Factor *= 1.06f;
		m_Factor = min(1.0f, m_Factor);

		if (m_LifeTime > 2.0f)
		{
			SendMessage(m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		}
	}

	bool ProcessMessage(Hedgehog::Universe::Message& message,bool flag) override
	{
		if (flag)
		{
			if (std::strstr(message.GetType(), "MsgRestartStage") != nullptr
			 || std::strstr(message.GetType(), "MsgStageClear") != nullptr)
			{
				SendMessage(m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
				return true;
			}
		}

		return Sonic::CGameObject::ProcessMessage(message, flag);
	}
};

HOOK(void, __fastcall, CObjRingProcMsgHitEventCollision, 0x10534B0, Sonic::CGameObject3D* This, void* Edx, hh::fnd::Message& in_rMsg)
{
	auto const* context = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (rcSpeedGauge && context->m_pPlayer->m_ActorID == in_rMsg.m_SenderActorID)
	{
		This->m_pMember->m_pGameDocument->AddGameObject(boost::make_shared<CObjGetItem>(This->m_spMatrixNodeTransform->m_Transform.m_Matrix));
	}

	originalCObjRingProcMsgHitEventCollision(This, Edx, in_rMsg);
}

HOOK(int, __fastcall, ProcMsgRestartStage, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	if (rcSpeedCount)
		Chao::CSD::CProject::DestroyScene(rcPlayScreen.Get(), rcSpeedCount);

	lostRingCount = 0;

	return originalProcMsgRestartStage(This, Edx, message);
}

HOOK(void, __stdcall, CPlayerGetLife, 0xE75520, Sonic::Player::CPlayerContext* context, int lifeCount, bool playSound)
{
	originalCPlayerGetLife(context, lifeCount, playSound);

	if (lifeCount > 0)
	{
		context->m_pPlayer->m_pMember->m_pGameDocument->AddGameObject(boost::make_shared<CObjGetLife>());
		if (playSound)
		{
			context->PlaySound(4001009, 0);
		}
	}
}

HOOK(void, __fastcall, ProcMsgDamageModern, 0xE27890, uint32_t* This, void* Edx, void* message)
{
	auto const* context = Sonic::Player::CPlayerSpeedContext::GetInstance();
	int ringCountPrev = context->m_RingCount;
	bool wasDamaged = context->StateFlag(eStateFlag_NoDamage) > 0;
	originalProcMsgDamageModern(This, Edx, message);

	if (!wasDamaged && ringCountPrev > 0)
	{
		int ringCountDiff = ringCountPrev - context->m_RingCount;
		if (ringCountDiff > 0)
		{
			lostRingCount = max(0, min(50, ringCountDiff));
		}
	}
}

//---------------------------------------------------
// Boss Health
//---------------------------------------------------
void HudSonicStage_BossSetHealth(float health, float maxHealth)
{
    bossGauge1Frame = health * 100.0f / maxHealth;
	bossGauge1Frame = max(0.0f, min(100.0f, bossGauge1Frame));
}

void HudSonicStage_BossSetBreakPoint(float health, float maxHealth)
{
	bossGaugeBreakPointFrame = health * 100.0f / maxHealth;
	bossGaugeBreakPointFrame = max(0.0f, min(100.0f, bossGaugeBreakPointFrame));
}

void __declspec(naked) HudSonicStage_CRivalMetalSonicLastHit()
{
    static uint32_t returnAddress = 0xCC8B73;
    __asm
    {
        add     eax, 0FFFFFFFFh
        mov     [edi + 200h], eax
        fldz
        sub     esp, 8
        jmp     [returnAddress]
    }
}

int HudSonicStage_MetalSonicMaxHealth = 0;
HOOK(bool, __fastcall, HudSonicStage_CRivalMetalSonicProcessMessage, 0xCD2760, hh::fnd::CMessageActor* This, void* Edx, hh::fnd::Message& message, bool flag)
{
    bool result = originalHudSonicStage_CRivalMetalSonicProcessMessage(This, Edx, message, flag);

    if (flag)
    {
        int health = ((int**)((uint32_t)This - 0x28))[104][128] + 2;

        if (std::strstr(message.GetType(), "MsgDummy") != nullptr)
        {
            HudSonicStage_MetalSonicMaxHealth = health;
            printf("[CustomHUD] Metal Sonic max health = %d\n", HudSonicStage_MetalSonicMaxHealth);
        }

        HudSonicStage_BossSetHealth(health, HudSonicStage_MetalSonicMaxHealth);
		HudSonicStage_BossSetBreakPoint(1, HudSonicStage_MetalSonicMaxHealth);
    }

    return result;
}

int HudSonicStage_DeathEggMaxHealth = 0;
HOOK(int, __fastcall, HudSonicStage_CBossDeathEggSetMaxHealth, 0xC46DA0, void* This, void* Edx, int a2, int a3)
{
    HudSonicStage_DeathEggMaxHealth = *(int*)(*(int*)(a3 + 4) + 4);
    return originalHudSonicStage_CBossDeathEggSetMaxHealth(This, Edx, a2, a3);
}

HOOK(bool, __fastcall, HudSonicStage_CBossDeathEggProcessMessage, 0xC67350, hh::fnd::CMessageActor* This, void* Edx, hh::fnd::Message& message, bool flag)
{
    bool result = originalHudSonicStage_CBossDeathEggProcessMessage(This, Edx, message, flag);

    if (flag)
    {
        int health = ((int*)((uint32_t)This - 0x28))[121];
        HudSonicStage_BossSetHealth(health, HudSonicStage_DeathEggMaxHealth);
		HudSonicStage_BossSetBreakPoint(2, HudSonicStage_DeathEggMaxHealth);
    }

    return result;
}

HOOK(bool, __fastcall, HudSonicStage_CBossPerfectChaosProcessMessage, 0xC122D0, hh::fnd::CMessageActor* This, void* Edx, hh::fnd::Message& message, bool flag)
{
    bool result = originalHudSonicStage_CBossPerfectChaosProcessMessage(This, Edx, message, flag);

    if (flag)
    {
        int* pGameObject = (int*)((uint32_t)This - 0x28);
        int health = pGameObject[121];
        bool finalHitInstance = *(bool*)((int)pGameObject + 500);

        // Only handles the first two hits
        if (!finalHitInstance && health > 0)
        {
            HudSonicStage_BossSetHealth(health + 1, 4);
			HudSonicStage_BossSetBreakPoint(1, 4);
        }
    }

    return result;
}

HOOK(bool, __fastcall, HudSonicStage_CBossPerfectChaosCStateDamageToFinalAttack, 0x5D1B10, void* This)
{
    HudSonicStage_BossSetHealth(1, 4);
    return originalHudSonicStage_CBossPerfectChaosCStateDamageToFinalAttack(This);
}

HOOK(void, __fastcall, HudSonicStage_CBossPerfectChaosCStateDefeated, 0x5D1C70, void* This)
{
    HudSonicStage_BossSetHealth(0, 4);
    originalHudSonicStage_CBossPerfectChaosCStateDefeated(This);
}

HOOK(bool, __fastcall, HudSonicStage_CBossEggDragoonProcessMessage, 0xC3F500, hh::fnd::CMessageActor* This, void* Edx, hh::fnd::Message& message, bool flag)
{
    bool result = originalHudSonicStage_CBossEggDragoonProcessMessage(This, Edx, message, flag);

    if (flag)
    {
        int* pGameObject = (int*)((uint32_t)This - 0x28);
        int maxHealth = pGameObject[121];
        int headHealth = pGameObject[124];
        int bellyHealth = pGameObject[125];
        HudSonicStage_BossSetHealth(headHealth + bellyHealth, maxHealth);
    }

    return result;
}

int HudSonicStage_SilverMaxHealth = 0;
HOOK(bool, __fastcall, HudSonicStage_CRivalSilverProcessMessage, 0xC8B8F0, hh::fnd::CMessageActor* This, void* Edx, hh::fnd::Message& message, bool flag)
{
    bool result = originalHudSonicStage_CRivalSilverProcessMessage(This, Edx, message, flag);

    if (flag)
    {
        int health = ((int**)((uint32_t)This - 0x28))[104][104] + 2;

        if (std::strstr(message.GetType(), "MsgDummy") != nullptr)
        {
            HudSonicStage_SilverMaxHealth = health;
            printf("[CustomHUD] Silver max health = %d\n", HudSonicStage_SilverMaxHealth);
        }

        HudSonicStage_BossSetHealth(health, HudSonicStage_SilverMaxHealth);
		HudSonicStage_BossSetBreakPoint(1, HudSonicStage_SilverMaxHealth);
    }

    return result;
}

int HudSonicStage_TimeEaterMaxHealth = 0;
HOOK(bool, __fastcall, HudSonicStage_CBossTimeEaterProcessMessage, 0xBFF390, hh::fnd::CMessageActor* This, void* Edx, hh::fnd::Message& message, bool flag)
{
    bool result = originalHudSonicStage_CBossTimeEaterProcessMessage(This, Edx, message, flag);

    if (flag)
    {
        int health = ((int*)((uint32_t)This - 0x28))[121] + 1;

        if (std::strstr(message.GetType(), "MsgDummy") != nullptr)
        {
            HudSonicStage_TimeEaterMaxHealth = health;
            printf("[CustomHUD] Time Eater max health = %d\n", HudSonicStage_TimeEaterMaxHealth);
        }

        HudSonicStage_BossSetHealth(health, HudSonicStage_TimeEaterMaxHealth);
		HudSonicStage_BossSetBreakPoint(1, HudSonicStage_TimeEaterMaxHealth);
    }

    return result;
}

void HudSonicStage::Install()
{
	INSTALL_HOOK(ProcMsgGetMissionLimitTime);
	INSTALL_HOOK(ProcMsgGetMissionCondition);
	INSTALL_HOOK(ProcMsgNotifyLapTimeHud);
	INSTALL_HOOK(CHudSonicStageDelayProcessImp);
	INSTALL_HOOK(CCountdownUpdate);
	INSTALL_HOOK(CLastBossGaugeNewUpdate);
	INSTALL_HOOK(ProcMsgChangeCustomHud);
	INSTALL_HOOK(CHudSonicStageUpdateParallel);
	INSTALL_HOOK(ProcMsgSetPinballHud);
	INSTALL_HOOK(CObjRingProcMsgHitEventCollision);
	INSTALL_HOOK(ProcMsgRestartStage);

	WRITE_JUMP(0x109C1DC, GetScoreEnabled);

	WRITE_MEMORY(0xB6AC1C, char*, "ui_lockon_cursor_gen"); // boss lock-on
	WRITE_MEMORY(0xDEC0CF, char*, "ui_lockon_cursor_swa"); // normal homing attack hud
	
	// Unleashed Ready GO!
	WRITE_MEMORY(0x109DAC8, char*, "Start_SWA");
	WRITE_MEMORY(0x109DBB5, char*, "Intro_Anim_act");

	// Unleashed mission success/fail
	WRITE_MEMORY(0x168E124, char*, "Clear_SWA");
	WRITE_MEMORY(0x168E130, char*, "Clear_SWA");
	WRITE_MEMORY(0x168E13C, char*, "Failed_SWA");
	WRITE_MEMORY(0x168E148, char*, "Failed_SWA");
	WRITE_MEMORY(0x168E154, char*, "Failed_SWA");
	WRITE_JUMP(0x42D661, (void*)0x42D68C);

	// Unleashed game over (remove yes no confirm, force yes)
	WRITE_MEMORY(0xCFE9FF, char*, "Game_over_SWA");
	WRITE_MEMORY(0xCFED0E, uint32_t, 4);
	WRITE_JUMP(0xCFEC3D, (void*)0xCFEC94);
	WRITE_JUMP(0xCFECB4, (void*)0xCFED0B);
	WRITE_JUMP(0xCFEDC7, (void*)0xCFE888);
	WRITE_NOP(0xCFDF9D, 19); // Don't play Game Over music
	WRITE_MEMORY(0xCFE6FD, uint8_t, 0xEB); // Don't play Game Over ticking sfx

	WRITE_MEMORY(0xDEBCA4, uint8_t, 0xEB);
	WRITE_MEMORY(0x109B1A4, uint8_t, 0xE9, 0xDC, 0x02, 0x00, 0x00); // Disable lives (patched differently to not clash with Disable Lives patch)
	WRITE_MEMORY(0x109B490, uint8_t, 0x90, 0xE9); // Disable time
	WRITE_MEMORY(0x109B5AD, uint8_t, 0x90, 0xE9); // Disable rings
	WRITE_MEMORY(0x109B6A7, uint8_t, 0x90, 0xE9); // Disable final boss rings
	WRITE_MEMORY(0x109B8F5, uint8_t, 0x90, 0xE9); // Disable boost gauge
	WRITE_MEMORY(0x109BC88, uint8_t, 0x90, 0xE9); // Disable boost button
	// WRITE_MEMORY(0x109BEF0, uint8_t, 0x90, 0xE9); // Disable mission countdown
	WRITE_MEMORY(0x109C3E2, uint8_t, 0x90, 0xE9); // Disable mission rank
	WRITE_MEMORY(0x16A467C, void*, CHudSonicStageRemoveCallback);

	// Unleashed 1up HUD object
	WRITE_JUMP(0xE7555F, (void*)0xE7565F);
	INSTALL_HOOK(CPlayerGetLife);
	WRITE_STRING(0x15E90DC, "");

	// Unleashed Drop Ring HUD
	INSTALL_HOOK(ProcMsgDamageModern);

	// Boss Health
	WRITE_JUMP(0xCC8B6E, HudSonicStage_CRivalMetalSonicLastHit);
	INSTALL_HOOK(HudSonicStage_CRivalMetalSonicProcessMessage);
	INSTALL_HOOK(HudSonicStage_CBossDeathEggSetMaxHealth);
	INSTALL_HOOK(HudSonicStage_CBossDeathEggProcessMessage);
	INSTALL_HOOK(HudSonicStage_CBossPerfectChaosProcessMessage);
	INSTALL_HOOK(HudSonicStage_CBossPerfectChaosCStateDamageToFinalAttack);
	INSTALL_HOOK(HudSonicStage_CBossPerfectChaosCStateDefeated);
	INSTALL_HOOK(HudSonicStage_CBossEggDragoonProcessMessage);
	INSTALL_HOOK(HudSonicStage_CRivalSilverProcessMessage);
	INSTALL_HOOK(HudSonicStage_CBossTimeEaterProcessMessage);

}

void HudSonicStage::CalculateAspectOffsets()
{
	if (*(size_t*)0x6F23C6 != 0x75D8C0D9) // Widescreen Support
	{
		const float aspect = (float)*(size_t*)0x1DFDDDC / (float)*(size_t*)0x1DFDDE0;

		if (aspect * 9.0f > 16.0f)
		{
			xAspectOffset = 720.0f * aspect - 1280.0f;
			yAspectOffset = 0.0f;
		}
		else
		{
			xAspectOffset = 0.0f;
			yAspectOffset = 1280.0f / aspect - 720.0f;
		}
	}
	else
	{
		xAspectOffset = 0.0f;
		yAspectOffset = 0.0f;
	}
}
