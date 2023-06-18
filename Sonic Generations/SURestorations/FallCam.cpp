float factor = 0.0f;
bool pause = false;

bool fadePlayed = false;

bool fallDead = false;

boost::shared_ptr<Sonic::CGameObject> fadeSingleton;

Timer* fadeIntroTimer = new Timer(3.5f);

const float deadToRestart = 0.25f;

class FadeObject : public Sonic::CGameObject {
	boost::shared_ptr<Sonic::CGameObjectCSD> spFade;
	Chao::CSD::RCPtr<Chao::CSD::CProject> rcFade;
	Chao::CSD::RCPtr<Chao::CSD::CScene> rect;
public:
	~FadeObject() {
		if (spFade) {
			spFade->SendMessage(spFade->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
			spFade = nullptr;
		}
		Chao::CSD::CProject::DestroyScene(rcFade.Get(), rect);
		rcFade = nullptr;
		rect = nullptr;
	}

    void AddCallback
    (
        const Hedgehog::Base::THolder<Sonic::CWorld>& worldHolder,
        Sonic::CGameDocument* pGameDocument,
        const boost::shared_ptr<Hedgehog::Database::CDatabase>& spDatabase
    ) override
    {
        // Update unit 1 is unaffected by time slowing down
        Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
        pGameDocument->AddUpdateUnit("1", this);

		Sonic::CCsdDatabaseWrapper wrapperFade(m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

		auto spCsdProjectFade = wrapperFade.GetCsdProject("ui_fade");

		rcFade = spCsdProjectFade->m_rcProject;
		rect = rcFade->CreateScene("fade");
		CSDCommon::FreezeMotion(*rect);
		rect->SetHideFlag(true);

		spFade = boost::make_shared<Sonic::CGameObjectCSD>(rcFade, 0.5f, "HUD_A1", false);
		Sonic::CGameDocument::GetInstance()->AddGameObject(spFade, "main", this);
    }

	void Kill() {
		SendMessage(m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		fadeSingleton = nullptr;
	}

	void Play(int id, float speed = 1.0f) {
		switch (id)
		{
		default:
			CSDCommon::PlayAnimation(*rect, "Fade_Black", Chao::CSD::eMotionRepeatType_PlayOnce, speed, 0.0f, 0.0f, true);
			break;
		case 0:
			CSDCommon::PlayAnimation(*rect, "Fade_Black", Chao::CSD::eMotionRepeatType_PlayOnce, speed, 0.0f, 0.0f, true);
			break;
		case 1:
			CSDCommon::PlayAnimation(*rect, "Fade_White", Chao::CSD::eMotionRepeatType_PlayOnce, speed, 0.0f, 0.0f, true);
			break;
		}
	}

	void SetVisibility(bool visible) {
		rect->SetHideFlag(!visible);
	}
};

HOOK(bool, __fastcall, FallCam_MsgStartPause, 0x010BC130, void* This, void* Edx, void* a2)
{
	pause = true;
	return originalFallCam_MsgStartPause(This, Edx, a2);
}

HOOK(int, __fastcall, FallCam_MsgFinishPause, 0x010BC110, void* This, void* Edx, void* a2)
{
	pause = false;
	return originalFallCam_MsgFinishPause(This, Edx, a2);
}

void PlayFade(int id, float speed = 1.0f) {
	auto* context = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (fadeSingleton)
	{
		((FadeObject*)fadeSingleton.get())->Kill();
	}
	fadeSingleton = boost::make_shared<FadeObject>();
	context->m_pPlayer->m_pMember->m_pGameDocument->AddGameObject(fadeSingleton);
	((FadeObject*)fadeSingleton.get())->Play(id, speed);
}

void StopFade() {
	if (fadeSingleton)
	{
		((FadeObject*)fadeSingleton.get())->Kill();
	}
}

HOOK(void, __fastcall, FallCam_CCameraUpdateParallel, 0x10FB770, Sonic::CCamera* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo) {
	auto& camera = This->m_MyCamera;
	auto* context = Sonic::Player::CPlayerSpeedContext::GetInstance();

	if (!context) {
		originalFallCam_CCameraUpdateParallel(This, Edx, in_rUpdateInfo);
		return;
	}

	Hedgehog::Math::CVector playerRight = context->m_spMatrixNode->m_Transform.m_Rotation * Hedgehog::Math::CVector::UnitX();
	Hedgehog::Math::CVector playerUp = context->m_spMatrixNode->m_Transform.m_Rotation * Hedgehog::Math::CVector::UnitY();
	Hedgehog::Math::CVector playerForward = context->m_spMatrixNode->m_Transform.m_Rotation * Hedgehog::Math::CVector::UnitZ();
	Hedgehog::Base::CSharedString state = context->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
	bool isDeadFall = (Common::GetSonicStateFlags()->Dead && (state == "Fall" || state == "NormalDamageDeadAir")) || fallDead;

	float time = deadToRestart + fadeIntroTimer->GetDuration();
	context->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_DeadToRestartTime] = time;
	context->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_DeadBgmWaitTime] = time - 0.1f;
	context->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_DeadBgmFadeTime] = 0.1f;

	if (Common::GetSonicStateFlags()->Dead) {
		if (!fadePlayed) {
			fadePlayed = true;
			fadeIntroTimer->GetOnComplete() = [] {	PlayFade(0, 1.0f); };
			fadeIntroTimer->Start();
		}
		fadeIntroTimer->Update(in_rUpdateInfo.DeltaTime);
	}
	else {
		fadePlayed = false;
	}

	if (!isDeadFall) {
		originalFallCam_CCameraUpdateParallel(This, Edx, in_rUpdateInfo);
	}
	else {
		if (pause) {
			camera.m_View = (Eigen::Translation3f(camera.m_Position) * Hedgehog::Math::CQuaternion(camera.m_View.rotation().inverse())).inverse().matrix();
			camera.m_InputView = camera.m_View;
			originalFallCam_CCameraUpdateParallel(This, Edx, in_rUpdateInfo);
			return;
		}
		else if (!context->m_Is2DMode) {
			factor += in_rUpdateInfo.DeltaTime * 0.05;
			camera.m_Direction = context->m_HorizontalRotation * Eigen::Vector3f::UnitZ();
			const Eigen::AngleAxisf rotPitch(Math::angleBetween(camera.m_Direction, context->m_spMatrixNode->m_Transform.m_Position - camera.m_Position), playerRight);
			//angleBetween(playerUp, context->m_spMatrixNode->m_Transform.m_Position - camera.m_Position) for yaw but don't use it
			const Eigen::AngleAxisf rotYaw(180.0f * DEG_TO_RAD, playerUp);
			const Eigen::AngleAxisf rotRoll(0.0f * DEG_TO_RAD, playerForward);
			Hedgehog::Math::CQuaternion rotationSlerp = Hedgehog::Math::CQuaternion(camera.m_View.rotation().inverse()).slerp(factor, rotPitch * rotYaw * rotRoll * context->m_spMatrixNode->m_Transform.m_Rotation);
			camera.m_View = (Eigen::Translation3f(camera.m_Position) * rotationSlerp).inverse().matrix();
			camera.m_InputView = camera.m_View;
		}
	}
}

HOOK(int, __fastcall, FallCam_MsgRestartStage, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	int result = originalFallCam_MsgRestartStage(This, Edx, message);

	StopFade();

	fallDead = false;

	factor = 0.0f;

	return result;
}

HOOK(DWORD*, __fastcall, EventFallDead, 0x5156D0, DWORD* This, char a2) {
	fallDead = true;
	return originalEventFallDead(This, a2);
}

void FallCam::Install() {
	INSTALL_HOOK(EventFallDead);
	INSTALL_HOOK(FallCam_MsgStartPause);
	INSTALL_HOOK(FallCam_MsgFinishPause);
	INSTALL_HOOK(FallCam_CCameraUpdateParallel);
	INSTALL_HOOK(FallCam_MsgRestartStage);
}