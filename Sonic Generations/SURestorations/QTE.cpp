boost::shared_ptr<Sonic::CGameObjectCSD> spQTE;
Chao::CSD::RCPtr<Chao::CSD::CProject> rcQTE;
Chao::CSD::RCPtr<Chao::CSD::CScene> m_bg;
Chao::CSD::RCPtr<Chao::CSD::CScene> m_timer;

Chao::CSD::RCPtr<Chao::CSD::CScene> qte_txt_1; // Nice
Chao::CSD::RCPtr<Chao::CSD::CScene> qte_txt_2; // Great!
Chao::CSD::RCPtr<Chao::CSD::CScene> qte_txt_3; // Cool!!
Chao::CSD::RCPtr<Chao::CSD::CScene> qte_txt_4; // YOU FAILED

Chao::CSD::RCPtr<Chao::CSD::CScene> qte_btn_1;
Chao::CSD::RCPtr<Chao::CSD::CScene> qte_btn_2;
Chao::CSD::RCPtr<Chao::CSD::CScene> qte_btn_3;
Chao::CSD::RCPtr<Chao::CSD::CScene> qte_btn_4;
Chao::CSD::RCPtr<Chao::CSD::CScene> qte_btn_5;

Chao::CSD::RCPtr<Chao::CSD::CScene> qte_btn_eff_1;
Chao::CSD::RCPtr<Chao::CSD::CScene> qte_btn_eff_2;
Chao::CSD::RCPtr<Chao::CSD::CScene> qte_btn_eff_3;
Chao::CSD::RCPtr<Chao::CSD::CScene> qte_btn_eff_4;
Chao::CSD::RCPtr<Chao::CSD::CScene> qte_btn_eff_5;

void KillScreenQTE()
{
	if (spQTE)
	{
		spQTE->SendMessage(spQTE->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spQTE = nullptr;
	}
}

void __fastcall CHudSonicStageRemoveCallbackQTE(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	KillScreenQTE();
	Chao::CSD::CProject::DestroyScene(rcQTE.Get(), m_bg);
	Chao::CSD::CProject::DestroyScene(rcQTE.Get(), m_timer);
	Chao::CSD::CProject::DestroyScene(rcQTE.Get(), qte_txt_1);
	Chao::CSD::CProject::DestroyScene(rcQTE.Get(), qte_txt_2);
	Chao::CSD::CProject::DestroyScene(rcQTE.Get(), qte_txt_3);
	Chao::CSD::CProject::DestroyScene(rcQTE.Get(), qte_txt_4);
	rcQTE = nullptr;
}

void CreateScreenQTE(Sonic::CGameObject* pParentGameObject)
{
	if (rcQTE && !spQTE)
		pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spQTE = boost::make_shared<Sonic::CGameObjectCSD>(rcQTE, 0.5f, "HUD", false), "main", pParentGameObject);
}

HOOK(void, __fastcall, CHudSonicStageDelayProcessImpQTE, 0x109A8D0, Sonic::CGameObject* This) {
	originalCHudSonicStageDelayProcessImpQTE(This);
	if (BlueBlurCommon::IsModern()) {
		CHudSonicStageRemoveCallbackQTE(This, nullptr, nullptr);

		Sonic::CCsdDatabaseWrapper wrapperQTE(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

		auto spCsdProjectQTE = wrapperQTE.GetCsdProject("ui_quicktime");

		size_t& flags = ((size_t*)This)[151];

		rcQTE = spCsdProjectQTE->m_rcProject;
		m_bg = rcQTE->CreateScene("m_bg");
		CSDCommon::FreezeMotion(*m_bg);
		m_bg->SetHideFlag(true);

		m_timer = rcQTE->CreateScene("m_timer");
		CSDCommon::FreezeMotion(*m_timer);
		m_timer->SetHideFlag(true);

		qte_txt_1 = rcQTE->CreateScene("qte_txt_1");
		CSDCommon::FreezeMotion(*qte_txt_1);
		qte_txt_1->SetHideFlag(true);

		qte_txt_2 = rcQTE->CreateScene("qte_txt_2");
		CSDCommon::FreezeMotion(*qte_txt_2);
		qte_txt_2->SetHideFlag(true);

		qte_txt_3 = rcQTE->CreateScene("qte_txt_3");
		CSDCommon::FreezeMotion(*qte_txt_3);
		qte_txt_3->SetHideFlag(true);

		qte_txt_4 = rcQTE->CreateScene("qte_txt_4");
		CSDCommon::FreezeMotion(*qte_txt_4);
		qte_txt_4->SetHideFlag(true);

		qte_btn_eff_1 = rcQTE->CreateScene("qte_multi_effect");
		CSDCommon::FreezeMotion(*qte_btn_eff_1);
		qte_btn_eff_1->SetHideFlag(true);

		qte_btn_eff_2 = rcQTE->CreateScene("qte_multi_effect");
		CSDCommon::FreezeMotion(*qte_btn_eff_2);
		qte_btn_eff_2->SetHideFlag(true);

		qte_btn_eff_3 = rcQTE->CreateScene("qte_multi_effect");
		CSDCommon::FreezeMotion(*qte_btn_eff_3);
		qte_btn_eff_3->SetHideFlag(true);

		qte_btn_eff_4 = rcQTE->CreateScene("qte_multi_effect");
		CSDCommon::FreezeMotion(*qte_btn_eff_4);
		qte_btn_eff_4->SetHideFlag(true);

		qte_btn_eff_5 = rcQTE->CreateScene("qte_multi_effect");
		CSDCommon::FreezeMotion(*qte_btn_eff_5);
		qte_btn_eff_5->SetHideFlag(true);

		CreateScreenQTE(This);
	}
}

float timeMult = 1.0f;
bool trick = false;
enum QTEPromptSize {
	Size3 = 3,
	Size4 = 4,
	Size5 = 5,
};
enum QTEButtonType {
	Button,
	Bumper,
};
struct QTEPrompt {
	QTEPromptSize size;
	std::vector<Sonic::EKeyState> buttonPrompt = {};

} currentQTEPrompt;
std::vector<Sonic::EKeyState> buttons = {
	Sonic::eKeyState_A,
	Sonic::eKeyState_B,
	Sonic::eKeyState_X,
	Sonic::eKeyState_Y,
	Sonic::eKeyState_LeftBumper,
	Sonic::eKeyState_RightBumper,
};
int buttonPromptIndex = 0;
Sonic::EKeyState IsAnyInputTapped() {
	for (auto& button : buttons) {
		if (Common::IsInputTapped(button)) {
			return button;
		}
	}
	return Sonic::eKeyState_None;
}

const char* GetQTESizeAnim() {
	switch (currentQTEPrompt.size)
	{
	case QTEPromptSize::Size3:
		return "Size_Anim_3";
		break;
	case QTEPromptSize::Size4:
		return "Size_Anim_4";
		break;
	case QTEPromptSize::Size5:
		return "Size_Anim_5";
		break;
	default:
		return "Size_Anim_3";
		break;
	}
}
QTEButtonType GetQTEButtonType(int index) {
	Sonic::EKeyState key = currentQTEPrompt.buttonPrompt[index];
	if (key == Sonic::eKeyState_LeftBumper || key == Sonic::eKeyState_RightBumper) {
		return QTEButtonType::Bumper;
	}

	if (key == Sonic::eKeyState_A || key == Sonic::eKeyState_B || key == Sonic::eKeyState_X || key == Sonic::eKeyState_Y) {
		return QTEButtonType::Button;
	}

	return QTEButtonType::Button;
}
const char* GetQTEButtonName(QTEButtonType type) {
	switch (type)
	{
	case Button:
		return "btn_1";
		break;
	case Bumper:
		return "btn_2";
		break;
	default:
		return "btn_1";
		break;
	}
}
int GetQTEPatternIndex(Sonic::EKeyState key) {
	switch (key)
	{
	case Sonic::eKeyState_A:
		return 0;
		break;
	case Sonic::eKeyState_B:
		return 1;
		break;
	case Sonic::eKeyState_X:
		return 2;
		break;
	case Sonic::eKeyState_Y:
		return 3;
		break;
	case Sonic::eKeyState_LeftBumper:
		return 0;
		break;
	case Sonic::eKeyState_RightBumper:
		return 1;
		break;
	default:
		return 0;
		break;
	}
}

float trickToFallTimer = 0.0f;
float trickTimer = 0.0f;

float qte_txtTimer = 0.0f;
bool qte_txtTimerActive = false;
bool qte_txtFinish = false;

float qteLength = 2.0f;
float qteTimeSpent = 0.0f;

bool outOfControlTrick = false;

Eigen::Vector3f trickStartVel = Eigen::Vector3f::Zero();

HOOK(int, __fastcall, ProcMsgRestartStageQTE, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	if (BlueBlurCommon::IsModern() && spQTE) {
		m_bg->SetHideFlag(true);
		m_timer->SetHideFlag(true);

		qte_txt_1->SetHideFlag(true);
		qte_txt_2->SetHideFlag(true);
		qte_txt_3->SetHideFlag(true);
		qte_txt_4->SetHideFlag(true);

		if (qte_btn_1 && qte_btn_2 && qte_btn_3 && qte_btn_4 && qte_btn_5) {
			qte_btn_1->SetHideFlag(true);
			qte_btn_2->SetHideFlag(true);
			qte_btn_3->SetHideFlag(true);
			qte_btn_4->SetHideFlag(true);
			qte_btn_5->SetHideFlag(true);
		}

		qte_btn_eff_1->SetHideFlag(true);
		qte_btn_eff_2->SetHideFlag(true);
		qte_btn_eff_3->SetHideFlag(true);
		qte_btn_eff_4->SetHideFlag(true);
		qte_btn_eff_5->SetHideFlag(true);

		trick = false;
		*(bool*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x19D }) = false;
		*(float*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x1A4 }) = 1.0f;
		timeMult = 1.0f;
		trickToFallTimer = 0.65f;
	}
	return originalProcMsgRestartStageQTE(This, Edx, message);
}

HOOK(void, __fastcall, HudResult_MsgStartGoalResultQTE, 0x10B58A0, uint32_t* This, void* Edx, void* message)
{
	originalHudResult_MsgStartGoalResultQTE(This, Edx, message);
	if (BlueBlurCommon::IsModern() && spQTE) {
		m_bg->SetHideFlag(true);
		m_timer->SetHideFlag(true);

		qte_txt_1->SetHideFlag(true);
		qte_txt_2->SetHideFlag(true);
		qte_txt_3->SetHideFlag(true);
		qte_txt_4->SetHideFlag(true);

		if (qte_btn_1 && qte_btn_2 && qte_btn_3 && qte_btn_4 && qte_btn_5) {
			qte_btn_1->SetHideFlag(true);
			qte_btn_2->SetHideFlag(true);
			qte_btn_3->SetHideFlag(true);
			qte_btn_4->SetHideFlag(true);
			qte_btn_5->SetHideFlag(true);
		}

		qte_btn_eff_1->SetHideFlag(true);
		qte_btn_eff_2->SetHideFlag(true);
		qte_btn_eff_3->SetHideFlag(true);
		qte_btn_eff_4->SetHideFlag(true);
		qte_btn_eff_5->SetHideFlag(true);

		trick = false;
		*(bool*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x19D }) = false;
		*(float*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x1A4 }) = 1.0f;
		timeMult = 1.0f;
		trickToFallTimer = 0.65f;
	}
}
void ButtonEffect(int index) {
	switch (index)
	{
	case 0:
		qte_btn_eff_1->SetHideFlag(false);
		CSDCommon::PlayAnimation(*qte_btn_eff_1, "Effect_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		CSDCommon::PlayAnimation(*qte_btn_1, "Effect_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		break;
	case 1:
		qte_btn_eff_2->SetHideFlag(false);
		CSDCommon::PlayAnimation(*qte_btn_eff_2, "Effect_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		CSDCommon::PlayAnimation(*qte_btn_2, "Effect_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		break;
	case 2:
		qte_btn_eff_3->SetHideFlag(false);
		CSDCommon::PlayAnimation(*qte_btn_eff_3, "Effect_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		CSDCommon::PlayAnimation(*qte_btn_3, "Effect_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		break;
	case 3:
		qte_btn_eff_4->SetHideFlag(false);
		CSDCommon::PlayAnimation(*qte_btn_eff_4, "Effect_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		CSDCommon::PlayAnimation(*qte_btn_4, "Effect_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		break;
	case 4:
		qte_btn_eff_5->SetHideFlag(false);
		CSDCommon::PlayAnimation(*qte_btn_eff_5, "Effect_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		CSDCommon::PlayAnimation(*qte_btn_5, "Effect_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		break;
	default:
		break;
	}
}
void SetupButtons() {
	Chao::CSD::CProject::DestroyScene(rcQTE.Get(), qte_btn_1);
	qte_btn_1 = rcQTE->CreateScene(GetQTEButtonName(GetQTEButtonType(0)));
	qte_btn_1->GetNode("img")->SetPatternIndex(GetQTEPatternIndex(currentQTEPrompt.buttonPrompt[0]));
	CSDCommon::PlayAnimation(*qte_btn_1, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);

	Chao::CSD::CProject::DestroyScene(rcQTE.Get(), qte_btn_2);
	qte_btn_2 = rcQTE->CreateScene(GetQTEButtonName(GetQTEButtonType(1)));
	qte_btn_2->GetNode("img")->SetPatternIndex(GetQTEPatternIndex(currentQTEPrompt.buttonPrompt[1]));
	CSDCommon::PlayAnimation(*qte_btn_2, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);

	Chao::CSD::CProject::DestroyScene(rcQTE.Get(), qte_btn_3);
	qte_btn_3 = rcQTE->CreateScene(GetQTEButtonName(GetQTEButtonType(2)));
	qte_btn_3->GetNode("img")->SetPatternIndex(GetQTEPatternIndex(currentQTEPrompt.buttonPrompt[2]));
	CSDCommon::PlayAnimation(*qte_btn_3, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);

	Chao::CSD::CProject::DestroyScene(rcQTE.Get(), qte_btn_4);
	qte_btn_4 = rcQTE->CreateScene(GetQTEButtonName(GetQTEButtonType(3)));
	qte_btn_4->GetNode("img")->SetPatternIndex(GetQTEPatternIndex(currentQTEPrompt.buttonPrompt[3]));
	CSDCommon::PlayAnimation(*qte_btn_4, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);

	Chao::CSD::CProject::DestroyScene(rcQTE.Get(), qte_btn_5);
	qte_btn_5 = rcQTE->CreateScene(GetQTEButtonName(GetQTEButtonType(4)));
	qte_btn_5->GetNode("img")->SetPatternIndex(GetQTEPatternIndex(currentQTEPrompt.buttonPrompt[4]));
	CSDCommon::PlayAnimation(*qte_btn_5, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);

	// TODO: Implement animations L_pattern & R_pattern for bumpers

	switch (currentQTEPrompt.size)
	{
	case QTEPromptSize::Size3:
		qte_btn_1->GetNode("position")->SetPosition(525.0f, 365.0f);
		qte_btn_eff_1->GetNode("position")->SetPosition(525.0f, 365.0f);

		qte_btn_2->GetNode("position")->SetPosition(635.0f, 365.0f);
		qte_btn_eff_2->GetNode("position")->SetPosition(635.0f, 365.0f);

		qte_btn_3->GetNode("position")->SetPosition(745.0f, 365.0f);
		qte_btn_eff_3->GetNode("position")->SetPosition(745.0f, 365.0f);

		qte_btn_4->SetHideFlag(true);
		qte_btn_5->SetHideFlag(true);
		break;
	case QTEPromptSize::Size4:
		qte_btn_1->GetNode("position")->SetPosition(465.0f, 365.0f);
		qte_btn_eff_1->GetNode("position")->SetPosition(465.0f, 365.0f);

		qte_btn_2->GetNode("position")->SetPosition(575.0f, 365.0f);
		qte_btn_eff_2->GetNode("position")->SetPosition(575.0f, 365.0f);

		qte_btn_3->GetNode("position")->SetPosition(685.0f, 365.0f);
		qte_btn_eff_3->GetNode("position")->SetPosition(685.0f, 365.0f);

		qte_btn_4->GetNode("position")->SetPosition(795.0f, 365.0f);
		qte_btn_eff_4->GetNode("position")->SetPosition(795.0f, 365.0f);

		qte_btn_5->SetHideFlag(true);
		break;
	case QTEPromptSize::Size5:
		qte_btn_1->GetNode("position")->SetPosition(425.0f, 365.0f);
		qte_btn_eff_1->GetNode("position")->SetPosition(425.0f, 365.0f);

		qte_btn_2->GetNode("position")->SetPosition(540.0f, 365.0f);
		qte_btn_eff_2->GetNode("position")->SetPosition(540.0f, 365.0f);

		qte_btn_3->GetNode("position")->SetPosition(655.0f, 365.0f);
		qte_btn_eff_3->GetNode("position")->SetPosition(655.0f, 365.0f);

		qte_btn_4->GetNode("position")->SetPosition(780.0f, 365.0f);
		qte_btn_eff_4->GetNode("position")->SetPosition(780.0f, 365.0f);

		qte_btn_5->GetNode("position")->SetPosition(885.0f, 365.0f);
		qte_btn_eff_5->GetNode("position")->SetPosition(885.0f, 365.0f);
		break;
	}
}
void FinishTrick(Sonic::Player::CPlayerSpeedContext* sonic, bool failed) {
	m_bg->SetHideFlag(true);
	m_timer->SetHideFlag(true);

	qte_txtTimer = 0.5f;
	qte_txtTimerActive = true;

	*(bool*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x19D }) = false;
	*(float*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x1A4 }) = 1.0f;

	sonic->StateFlag(eStateFlag_OutOfControl) = true;
	outOfControlTrick = true;

	trick = false;
	timeMult = 1.0f;

	if (failed) {
		sonic->m_Velocity *= 0.5f;
		qte_txt_4->SetHideFlag(false);
		CSDCommon::PlayAnimation(*qte_txt_4, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);

		qte_btn_1->SetHideFlag(true);
		qte_btn_2->SetHideFlag(true);
		qte_btn_3->SetHideFlag(true);
		qte_btn_4->SetHideFlag(true);
		qte_btn_5->SetHideFlag(true);
	}
	else {
		int randAnim = rand() % (3 - 0 + 1) + 0;
		switch (randAnim)
		{
		case 1:
			sonic->ChangeAnimation("Trick_u2");
			break;
		case 2:
			sonic->ChangeAnimation("Trick_u1");
			break;
		case 3:
			sonic->ChangeAnimation("Trick_u0");
			break;
		default:
			sonic->ChangeAnimation("Trick_r2");
			break;
		}

		trickToFallTimer = 0.65f;
		static SharedPtrTypeless voiceHandle;
		Common::SonicContextPlaySound(voiceHandle, 3002013, 0);

		if (qteTimeSpent / qteLength >= 0.8f) {
			qte_txt_1->SetHideFlag(false);
			CSDCommon::PlayAnimation(*qte_txt_1, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		}
		else if (qteTimeSpent / qteLength < 0.8f && qteTimeSpent / qteLength >= 0.4f) {
			qte_txt_2->SetHideFlag(false);
			CSDCommon::PlayAnimation(*qte_txt_2, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		}
		else if (qteTimeSpent / qteLength < 0.4f) {
			qte_txt_3->SetHideFlag(false);
			CSDCommon::PlayAnimation(*qte_txt_3, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		}

	}
}
HOOK(void, __fastcall, CHudSonicStageUpdateParallelQTE, 0x1098A50, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	originalCHudSonicStageUpdateParallelQTE(This, Edx, in_rUpdateInfo);
	if (qte_txtTimerActive) {
		qte_txtTimer -= in_rUpdateInfo.DeltaTime;
		if (qte_txtTimer <= 0.0f && !qte_txtFinish) {
			qte_txtFinish = true;
			CSDCommon::PlayAnimation(*qte_txt_1, "Finish_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
			CSDCommon::PlayAnimation(*qte_txt_2, "Finish_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
			CSDCommon::PlayAnimation(*qte_txt_3, "Finish_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
			CSDCommon::PlayAnimation(*qte_txt_4, "Fnish_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		}
	}
	if (trick) {
		// TODO: fix bar not moving

		float frameBeforeBG = m_bg->m_MotionFrame;
		CSDCommon::PlayAnimation(*m_bg, "Timer_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, frameBeforeBG);

		float frameBeforeTimer = m_timer->m_MotionFrame;
		CSDCommon::PlayAnimation(*m_timer, "Timer_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, frameBeforeTimer);

		m_timer->m_MotionSpeed = 1.0f / qteLength;

		float frameBeforeBG2 = m_bg->m_MotionFrame;
		CSDCommon::PlayAnimation(*m_bg, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, frameBeforeBG2);

		float frameBeforeTimer2  = m_timer->m_MotionFrame;
		CSDCommon::PlayAnimation(*m_timer, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, frameBeforeTimer2);
	}
}

HOOK(void, __fastcall, SonicQTEUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
	Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();
	if (trick) {
		timeMult = Common::Lerp(timeMult, 0.015f, updateInfo.DeltaTime * 3.0f);
		qteTimeSpent += updateInfo.DeltaTime;
		if (qteTimeSpent > qteLength) { qteTimeSpent = qteLength; }

		if (trickTimer > 0) { trickTimer -= updateInfo.DeltaTime; }
		else {
			FinishTrick(sonic, true);
		}

		if (anim != "JumpSpring") {
			sonic->ChangeAnimation("JumpSpring");
		}

		if (IsAnyInputTapped() == currentQTEPrompt.buttonPrompt[buttonPromptIndex]) {
			ButtonEffect(buttonPromptIndex);
			if (buttonPromptIndex + 1 >= currentQTEPrompt.buttonPrompt.size()) {
				FinishTrick(sonic, false);
			}
			else {
				buttonPromptIndex++;
			}
		}
		else {
			if (IsAnyInputTapped() != Sonic::eKeyState_None) {
				FinishTrick(sonic, true);
			}
		}
	}
	else {
		trickToFallTimer -= updateInfo.DeltaTime;

		if (strstr(anim.c_str(), "Trick_") && trickToFallTimer <= 0.0f && !sonic->m_Grounded) {
			sonic->ChangeAnimation("Fall");
		}
	}

	if (state == "AdlibTrick" && !trick) {
		currentQTEPrompt.buttonPrompt = { Sonic::eKeyState_LeftBumper, Sonic::eKeyState_X, Sonic::eKeyState_RightBumper, Sonic::eKeyState_Y, Sonic::eKeyState_A };
		currentQTEPrompt.size = QTEPromptSize::Size3;
		currentQTEPrompt.buttonPrompt.resize(static_cast<int>(currentQTEPrompt.size));
		buttonPromptIndex = 0;
		trick = true;
		qte_txtTimerActive = false;
		qte_txtFinish = false;
		qteTimeSpent = 0.0f;
		trickTimer = (100.0f / 60.0f) * qteLength;
		trickStartVel = sonic->m_Velocity;

		sonic->ChangeState("Fall");

		sonic->StateFlag(eStateFlag_EndBoost) = true;
		sonic->StateFlag(eStateFlag_OutOfControl) = true;
		sonic->m_HorizontalVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * Eigen::Vector3f::UnitZ() * 20.0f;

		m_bg->SetHideFlag(false);
		m_timer->SetHideFlag(false);

		CSDCommon::PlayAnimation(*m_bg, GetQTESizeAnim(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);
		CSDCommon::PlayAnimation(*m_timer, GetQTESizeAnim(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 1);

		qte_txt_1->SetHideFlag(true);
		qte_txt_2->SetHideFlag(true);
		qte_txt_3->SetHideFlag(true);
		qte_txt_4->SetHideFlag(true);

		SetupButtons();

		*(bool*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x19D }) = true;
		*(float*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x1A4 }) = 0.015f;
	}

	if (!trick && state != "Fall" && outOfControlTrick) {
		outOfControlTrick = false;
		sonic->StateFlag(eStateFlag_OutOfControl) = false;
	}

	Hedgehog::Universe::SUpdateInfo info = updateInfo;
	info.DeltaTime *= timeMult;
	originalSonicQTEUpdate(This, _, info);
}
void QTE::Install() {
	INSTALL_HOOK(SonicQTEUpdate);
	INSTALL_HOOK(CHudSonicStageDelayProcessImpQTE);
	INSTALL_HOOK(ProcMsgRestartStageQTE);
	INSTALL_HOOK(HudResult_MsgStartGoalResultQTE);
	INSTALL_HOOK(CHudSonicStageUpdateParallelQTE);
}