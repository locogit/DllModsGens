boost::shared_ptr<Sonic::CGameObjectCSD> spMedal;
Chao::CSD::RCPtr<Chao::CSD::CProject> rcMedal;
Chao::CSD::RCPtr<Chao::CSD::CScene> u_info, medal_get_s, medal_get_m;

float xAspectOffsetMedal = 0.0f;
float yAspectOffsetMedal = 0.0f;

void CreateScreenMedal(Sonic::CGameObject* pParentGameObject)
{
	if (rcMedal && !spMedal)
		pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spMedal = boost::make_shared<Sonic::CGameObjectCSD>(rcMedal, 0.5f, "HUD", false), "main", pParentGameObject);
}
void KillScreenMedal()
{
	if (spMedal) {
		spMedal->SendMessage(spMedal->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spMedal = nullptr;
	}
}
void __fastcall CHudSonicStageRemoveCallbackMedal(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	KillScreenMedal();
	Chao::CSD::CProject::DestroyScene(rcMedal.Get(), u_info);
	Chao::CSD::CProject::DestroyScene(rcMedal.Get(), medal_get_s);
	Chao::CSD::CProject::DestroyScene(rcMedal.Get(), medal_get_m);
	rcMedal = nullptr;
	u_info = nullptr;
	medal_get_s = nullptr;
	medal_get_m = nullptr;
}
//Brianuu/Skyth
void CalculateAspectOffsetsMedal()
{
	if (*(size_t*)0x6F23C6 != 0x75D8C0D9) // Widescreen Support
	{
		const float aspect = (float)*(size_t*)0x1DFDDDC / (float)*(size_t*)0x1DFDDE0;

		if (aspect * 9.0f > 16.0f)
		{
			xAspectOffsetMedal = 720.0f * aspect - 1280.0f;
			yAspectOffsetMedal = 0.0f;
		}
		else
		{
			xAspectOffsetMedal = 0.0f;
			yAspectOffsetMedal = 1280.0f / aspect - 720.0f;
		}
	}
	else
	{
		xAspectOffsetMedal = 0.0f;
		yAspectOffsetMedal = 0.0f;
	}
}
bool medalVisible = false;
HOOK(void, __fastcall, CHudSonicStageDelayProcessImpMedal, 0x109A8D0, Sonic::CGameObject* This) {
	originalCHudSonicStageDelayProcessImpMedal(This);
	if (BlueBlurCommon::IsModern()) {
		CHudSonicStageRemoveCallbackMedal(This, nullptr, nullptr);

		CalculateAspectOffsetsMedal();

		Sonic::CCsdDatabaseWrapper wrapperMedal(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

		auto spCsdProjectMedal = wrapperMedal.GetCsdProject("medal");

		size_t& flags = ((size_t*)This)[151];

		rcMedal = spCsdProjectMedal->m_rcProject;

		u_info = rcMedal->CreateScene("u_info");
		u_info->SetHideFlag(true);
		CSDCommon::FreezeMotion(*u_info);
		u_info->SetPosition(xAspectOffsetMedal, 0);

		medal_get_s = rcMedal->CreateScene("medal_get_s");
		CSDCommon::FreezeMotion(*medal_get_s);
		medal_get_s->SetPosition(xAspectOffsetMedal, 0);

		medal_get_m = rcMedal->CreateScene("medal_get_m");
		CSDCommon::FreezeMotion(*medal_get_m);
		medal_get_m->SetPosition(xAspectOffsetMedal, 0);

		flags &= ~(0x1 | 0x2 | 0x4 | 0x200 | 0x800); // Mask to prevent crash when game tries accessing the elements we disabled later on

		CreateScreenMedal(This);
	}
}
bool renderGameHudMedal;
float medalTimer = 0.0f;
float medalEndTimer = 0.0f;
bool medalEnd = false;
class CObjGetMedal : public Sonic::CGameObject
{
	hh::math::CMatrix44 m_Transform;
	hh::math::CQuaternion m_Rotation;
	hh::math::CQuaternion m_TargetRotation;
	float m_Factor{};
	float m_LifeTime{};
	boost::shared_ptr<hh::mr::CSingleElement> m_spModel;
	Sonic::Player::CPlayerSpeedContext* context = Sonic::Player::CPlayerSpeedContext::GetInstance();
	bool SunMedal = true;

public:
	CObjGetMedal(bool _sunMedal) : m_Rotation(hh::math::CQuaternion(0, 0, 1, 0))
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
		SunMedal = _sunMedal;
	}
	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& worldHolder, Sonic::CGameDocument* pGameDocument,
		const boost::shared_ptr<Hedgehog::Database::CDatabase>& spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		pGameDocument->AddUpdateUnit("f", this);

		m_spModel = boost::make_shared<hh::mr::CSingleElement>(hh::mr::CMirageDatabaseWrapper(spDatabase.get()).GetModelData((SunMedal) ? "cmn_obj_spcialring_HD" : "medal_moon"));
		AddRenderable("Object", m_spModel, false);
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

		rMatrix.col(0) /= LERP(1.0f, scale / 0.15f);
		rMatrix.col(1) /= LERP(1.0f, scale / 0.15f);
		rMatrix.col(2) /= LERP(1.0f, scale / 0.15f);

		rTransform(0, 3) = LERP(rTransform(0, 3), 0.65f);
		rTransform(1, 3) = LERP(rTransform(1, 3), (SunMedal) ? -0.629f : -0.74f);
		rTransform(2, 3) = LERP(rTransform(2, 3), 0.1f);
		rTransform(3, 3) = LERP(rTransform(3, 3), 1.0f);

		rTransform = invProj * rTransform.matrix();
		rTransform.rotate(m_Rotation);
		rTransform = invView * rTransform;

		(&rTransform)[1] = rTransform;

#undef LERP

		m_Rotation = m_Rotation.slerp(updateInfo.DeltaTime * 3.5f, m_TargetRotation);

		m_LifeTime += updateInfo.DeltaTime;
		m_Factor += updateInfo.DeltaTime * 0.125f;
		m_Factor *= 1.08f;
		m_Factor = min(1.0f, m_Factor);

		if (m_LifeTime > 0.8f)
		{
			CSDCommon::PlayAnimation(*u_info, (SunMedal) ? "get_sun" : "get_moon", Chao::CSD::eMotionRepeatType_PlayOnce, 0, 1);
			SendMessage(m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		}
	}

	bool ProcessMessage(Hedgehog::Universe::Message& message, bool flag) override
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
class CObjMedal : public Sonic::CGameObject
{
	boost::shared_ptr<hh::mr::CSingleElement> m_spModel;
	Sonic::Player::CPlayerSpeedContext* context = Sonic::Player::CPlayerSpeedContext::GetInstance();
	float rot = 0.0f;
	bool collect = false;
	bool SunMedal = true;
	Hedgehog::Math::CVector m_Position;
	SharedPtrTypeless collectHandle;
public:
	CObjMedal(bool _sunMedal, Hedgehog::Math::CVector _position)
	{
		SunMedal = _sunMedal;
		m_Position = _position;
	}
	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& worldHolder, Sonic::CGameDocument* pGameDocument,
		const boost::shared_ptr<Hedgehog::Database::CDatabase>& spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		pGameDocument->AddUpdateUnit("f", this);

		m_spModel = boost::make_shared<hh::mr::CSingleElement>(hh::mr::CMirageDatabaseWrapper(spDatabase.get()).GetModelData((SunMedal) ? "cmn_obj_spcialring_HD" : "medal_moon"));
		AddRenderable("Object", m_spModel, true);
	}

	void UpdateParallel(const Hedgehog::Universe::SUpdateInfo& updateInfo) override
	{
		rot += updateInfo.DeltaTime * 1.5f;
		Hedgehog::Math::CQuaternion m_Rotation = Hedgehog::Math::CQuaternion::Identity() * Eigen::AngleAxis<float>(rot, Eigen::Vector3f::UnitY());
		m_spModel->m_spInstanceInfo->m_Transform = (Eigen::Translation3f(m_Position + Eigen::Vector3f::UnitY() * 0.5f) * m_Rotation).matrix();
		float dist = abs((m_Position - context->m_spMatrixNode->m_Transform.m_Position).norm());
		float distThreshold = (context->StateFlag(eStateFlag_Boost)) ? 1.5f : 1.0f;
		if (dist <= distThreshold && !collect) {
			collect = true;
			SendMessage(m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
			void* middlematrixNode = (void*)((uint32_t)context + 0x30);
			Common::fCGlitterCreate(context, collectHandle, middlematrixNode, (SunMedal) ? "medal_collect_sun" : "medal_collect_moon", 1);
			if (!medalVisible) {
				CSDCommon::PlayAnimation(*u_info, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 0, 1);
				medalVisible = true;
			}
			medalEnd = false;
			medalTimer = 3.0f;
			medalEndTimer = 0.0f;
			context->m_pPlayer->m_pMember->m_pGameDocument->AddGameObject(boost::make_shared<CObjGetMedal>(SunMedal));
			context->PlaySound(2002500, true);
		}
	}

	bool ProcessMessage(Hedgehog::Universe::Message& message, bool flag) override
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
HOOK(void, __fastcall, CHudSonicStageUpdateParallelMedal, 0x1098A50, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo) {
	originalCHudSonicStageUpdateParallelMedal(This, Edx, in_rUpdateInfo);
	if (BlueBlurCommon::IsModern()) {
		renderGameHudMedal = *(bool*)0x1A430D8;
		Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();
		if (input.IsTapped(Sonic::eKeyState_DpadDown)) {
			sonic->m_pPlayer->m_pMember->m_pGameDocument->AddGameObject(boost::make_shared<CObjMedal>(true, Hedgehog::Math::CVector(-225.895f, 6.17f, -1829.407f)));
			sonic->m_pPlayer->m_pMember->m_pGameDocument->AddGameObject(boost::make_shared<CObjMedal>(false, Hedgehog::Math::CVector(-230.059f, 6.17f, -1829.407f)));
		}
		if (medalVisible) {
			if (medalTimer > 0) medalTimer -= in_rUpdateInfo.DeltaTime;
			if (medalTimer <= 0) {
				if (!medalEnd) {
					medalEnd = true;
					medalEndTimer = .3f;
					CSDCommon::PlayAnimation(*u_info, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, u_info->m_MotionEndFrame, false, true);
				}
			}
		}
		if (medalEnd) {
			if (medalEndTimer > 0) medalEndTimer -= in_rUpdateInfo.DeltaTime;
			if (medalEndTimer <= 0) {
				medalEnd = false;
				medalVisible = false;
				CSDCommon::FreezeMotion(*u_info);
			}
		}
		if (renderGameHudMedal) {
			u_info->SetHideFlag(!medalVisible);
			medal_get_s->SetHideFlag(!medalVisible);
			medal_get_m->SetHideFlag(!medalVisible);
		}
		else {
			u_info->SetHideFlag(true);
			medal_get_s->SetHideFlag(true);
			medal_get_m->SetHideFlag(true);
		}
	}
}
HOOK(int, __fastcall, ProcMsgRestartStageMedal, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	return originalProcMsgRestartStageMedal(This, Edx, message);
}
HOOK(void, __fastcall, HudResult_MsgStartGoalResultMedal, 0x10B58A0, uint32_t* This, void* Edx, void* message)
{
	originalHudResult_MsgStartGoalResultMedal(This, Edx, message);
}
void Medal::Install() {
	INSTALL_HOOK(CHudSonicStageDelayProcessImpMedal);
	INSTALL_HOOK(CHudSonicStageUpdateParallelMedal);
	INSTALL_HOOK(ProcMsgRestartStageMedal);
	INSTALL_HOOK(HudResult_MsgStartGoalResultMedal);
}