#include "HudResult.h"
#include "HudSonicStage.h"
#include "HudLoading.h"
#include "Configuration.h"

boost::shared_ptr<Sonic::CGameObjectCSD> spResult;
Chao::CSD::RCPtr<Chao::CSD::CProject> rcProjectResult;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcResultTitle;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcResultNum[HudResult::ResultNumType::COUNT];
Chao::CSD::RCPtr<Chao::CSD::CScene> rcResultNewRecordTime;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcResultNewRecordScore;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcResultRankText;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcResultRank;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcResultFooter;

boost::shared_ptr<Sonic::CGameObjectCSD> spItemResult;
Chao::CSD::RCPtr<Chao::CSD::CProject> rcItemProjectResult;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcItemResultWindow;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcItemResultContents;

bool m_isWerehog = false; // TODO: move this to configuration
float m_resultTimer = 0.0f;
HudResult::ModelType m_modelType = HudResult::ModelType::Gens;
HudResult::ResultState m_resultState = HudResult::ResultState::Idle;
HudResult::ResultState m_resultStateNew = HudResult::ResultState::Idle;

float m_enemyBonus = 0.0f;
float m_enemyChainBonus = 0.0f;
float m_trickBonus = 0.0f;
float m_trickBonusLimit = 0.0f;
float m_trickChainMultiplier = 0.0f;
float m_trickFinishBonus = 0.0f;
float m_rainbowRingBonus = 0.0f;

float m_stageTime = 0.0f;
HudResult::ResultData m_resultData;
HudResult::StageData m_stageData;
bool m_isEnemyChain = false;
int m_trickChain = 0;
HudResult::ResultSoundState m_soundState;

bool stageHasTricks = false;

float statusTimer = 0.0f;
float medalTimer = 0.0f;

HOOK(void, __fastcall, HudResult_MsgStartGoalResult, 0x10B58A0, uint32_t* This, void* Edx, void* message)
{
	// Don't run the message itself
}

void __fastcall HudResult_CHudResultRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	if (spResult)
	{
		printf("[Unleashed HUD] Result destroyed\n");
		spResult->SendMessage(spResult->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spResult = nullptr;
	}

	if (spItemResult)
	{
		printf("[Unleashed HUD] Item Result destroyed\n");
		spItemResult->SendMessage(spItemResult->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spItemResult = nullptr;
	}

	Chao::CSD::CProject::DestroyScene(rcProjectResult.Get(), rcResultTitle);
	for (int i = 0; i < HudResult::ResultNumType::COUNT; i++)
	{
		if (rcResultNum[i])
		{
			Chao::CSD::CProject::DestroyScene(rcProjectResult.Get(), rcResultNum[i]);
		}
	}
	Chao::CSD::CProject::DestroyScene(rcProjectResult.Get(), rcResultNewRecordTime);
	Chao::CSD::CProject::DestroyScene(rcProjectResult.Get(), rcResultNewRecordScore);
	Chao::CSD::CProject::DestroyScene(rcProjectResult.Get(), rcResultRankText);
	Chao::CSD::CProject::DestroyScene(rcProjectResult.Get(), rcResultRank);
	Chao::CSD::CProject::DestroyScene(rcProjectResult.Get(), rcResultFooter);

	Chao::CSD::CProject::DestroyScene(rcItemProjectResult.Get(), rcItemResultWindow);
	Chao::CSD::CProject::DestroyScene(rcItemProjectResult.Get(), rcItemResultContents);

	rcProjectResult = nullptr;
}

HOOK(int, __fastcall, HudResult_CStateGoalFadeBeforeBegin, 0xCFE080, uint32_t* This)
{
	int result = originalHudResult_CStateGoalFadeBeforeBegin(This);
	{
		m_resultTimer = 0.0f;
		m_resultState = HudResult::ResultState::Idle;
		m_resultStateNew = HudResult::ResultState::Idle;

		m_stageTime = *(float*)Common::GetMultiLevelAddress(This[2] + 0x60, { 0x8, 0x184 });
		m_resultData = *(HudResult::ResultData*)(This[2] + 0x16C);

		// Calculate scores
		uint32_t ringCount = Sonic::Player::CPlayerSpeedContext::GetInstance()->m_RingCount;
		m_stageData.m_ringScore = ringCount * 100.0f;
		if (!HudSonicStage::scoreEnabled)
		{
			// Set to Generations ring score if score is not enabled
			m_stageData.m_ringScore = min(5000, ringCount * 50);
		}
		else if (GetModuleHandle(TEXT("ScoreGenerations.dll")) != nullptr)
		{
			// Get speed score from Score Generations
			m_stageData.m_speedScore = ScoreGenerationsAPI::ComputeSpeedBonus();
		}
	}
	return result;
}

HOOK(void, __fastcall, HudResult_CStateGoalFadeInEnd, 0xCFA470, hh::fnd::CStateMachineBase::CStateBase* This)
{
	originalHudResult_CStateGoalFadeInEnd(This);

	// Result starts
	if (This->m_Time > *(float*)0x1A426B8)
	{
		m_resultTimer = 0.0f;
		m_resultStateNew = HudResult::ResultState::MainWait;
	}
}

HOOK(int, __fastcall, HudResult_CHudResultAddCallback, 0x10B8ED0, Sonic::CGameObject* This, void* Edx, int a2, int a3, int a4)
{
	int result = originalHudResult_CHudResultAddCallback(This, Edx, a2, a3, a4);
	HudResult_CHudResultRemoveCallback(This, nullptr, nullptr);

	HudSonicStage::CalculateAspectOffsets();

	printf("[Unleashed HUD] Result created\n");
	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	auto spCsdProject = wrapper.GetCsdProject("ui_result_swa");
	rcProjectResult = spCsdProject->m_rcProject;

	rcResultTitle = rcProjectResult->CreateScene("result_title");
	rcResultTitle->SetHideFlag(true);

	for (int i = 0; i < HudResult::ResultNumType::COUNT; i++)
	{
		rcResultNum[i] = rcProjectResult->CreateScene((std::string("result_num_") + std::to_string(i + 1)).c_str());
		rcResultNum[i]->SetHideFlag(true);
	}

	spCsdProject = wrapper.GetCsdProject("ui_itemresult");
	rcItemProjectResult = spCsdProject->m_rcProject;

	rcItemResultWindow = rcItemProjectResult->CreateScene("window");
	rcItemResultWindow->SetHideFlag(true);

	rcItemResultContents = rcItemProjectResult->CreateScene("contents");
	rcItemResultContents->SetHideFlag(true);
	rcItemResultContents->GetNode("btn")->SetHideFlag(1);

	if (rcProjectResult && !spResult)
	{
		spResult = boost::make_shared<Sonic::CGameObjectCSD>(rcProjectResult, 0.5f, "HUD", false);
		Sonic::CGameDocument::GetInstance()->AddGameObject(spResult, "main", This);
	}

	if (rcItemProjectResult && !spItemResult)
	{
		spItemResult = boost::make_shared<Sonic::CGameObjectCSD>(rcItemProjectResult, 0.5f, "HUD", false);
		Sonic::CGameDocument::GetInstance()->AddGameObject(spItemResult, "main", This);
	}

	HudResult::GetModelType();
	return result;
}

void HudResult_PlayMotion(Chao::CSD::RCPtr<Chao::CSD::CScene>& scene, char const* motion, bool loop = false)
{
	if (!scene) return;
	scene->SetHideFlag(false);
	scene->SetMotion(motion);
	scene->SetMotionFrame(0.0f);
	scene->m_MotionDisableFlag = false;
	scene->m_MotionSpeed = 1.0f;
	scene->m_MotionRepeatType = loop ? Chao::CSD::eMotionRepeatType_Loop : Chao::CSD::eMotionRepeatType_PlayOnce;
	scene->Update();
}

float const cResultMainDelay = 0.2833f;
float const cResultNewRecordDelay = 1.6667f;
float const cResultRankDelay = 0.9f;
float const cResultFooterDelay = 2.0f;
bool hasNewRecord = false;
HOOK(void, __fastcall, HudResult_CHudResultAdvance, 0x10B96D0, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	if (HudLoading::IsFadeOutCompleted())
	{
		originalHudResult_CHudResultAdvance(This, Edx, in_rUpdateInfo);
		if (*(uint32_t*)0x10B96E6 != 0xFFD285E8)
		{
			// We are finished
			HudResult_CHudResultRemoveCallback(This, nullptr, nullptr);
			HudStatus::Kill();
			WRITE_MEMORY(0x10B96E6, uint8_t, 0xE8, 0x85, 0xD2, 0xFF, 0xFF);
			WRITE_MEMORY(0x10B9A7C, uint8_t, 0xE8, 0xCF, 0x7A, 0x5A, 0xFF);
			WRITE_MEMORY(0x10B9976, uint8_t, 0xE8, 0xD5, 0x7B, 0x5A, 0xFF);
			return;
		}
	}

	FUNCTION_PTR(bool, __cdecl, IsFirstTimePlayStage, 0x10B7BB0);

	char const* motion_so_ev = m_isWerehog ? "Intro_ev_Anim" : "Intro_so_Anim";
	char const* motion_so_ev_2 = m_isWerehog ? "Intro_ev_Anim_2" : "Intro_so_Anim_2";

	// New states
	if (m_resultState != m_resultStateNew)
	{

		m_resultState = m_resultStateNew;
		switch (m_resultState)
		{
		case HudResult::ResultState::Idle:
		{
			printf("[Unleashed HUD] Result State: Idle\n");
			break;
		}
		case HudResult::ResultState::MainWait:
		{
			printf("[Unleashed HUD] Result State: Main Wait\n");

			m_soundState = HudResult::ResultSoundState();
			break;
		}
		case HudResult::ResultState::Main:
		{
			printf("[Unleashed HUD] Result State: Main\n");

			HudResult_PlayMotion(rcResultTitle, motion_so_ev);

			for (int i = 0; i < HudResult::ResultNumType::COUNT; i++)
			{
				switch (i)
				{
				case HudResult::ResultNumType::TIME:
				{
					int millisecond = (int)(m_stageTime * 100.0f) % 100;
					int second = (int)m_stageTime % 60;
					int minute = (int)m_stageTime / 60;
					static char buffer[16];
					sprintf(buffer, "%02d:%02d:%02d", minute, second, millisecond);
					rcResultNum[i]->GetNode("time_num")->SetText(buffer);
					HudResult_PlayMotion(rcResultNum[i], motion_so_ev);
					break;
				}
				case HudResult::ResultNumType::RINGS:
				{
					rcResultNum[i]->GetNode("num_2")->SetText(std::to_string((int)m_stageData.m_ringScore).c_str());
					HudResult_PlayMotion(rcResultNum[i], motion_so_ev);
					break;
				}
				case HudResult::ResultNumType::SPEED:
				{
					if (HudSonicStage::scoreEnabled && !Common::IsCurrentStageBossBattle())
					{
						rcResultNum[i]->GetNode("num_3")->SetText(std::to_string((int)m_stageData.m_speedScore).c_str());
						HudResult_PlayMotion(rcResultNum[i], motion_so_ev);
					}
					break;
				}
				case HudResult::ResultNumType::ENEMY:
				{
					if (HudSonicStage::scoreEnabled && !Common::IsCurrentStageBossBattle())
					{
						rcResultNum[i]->GetNode("num_4")->SetText(std::to_string((int)m_stageData.m_enemyScore).c_str());
						HudResult_PlayMotion(rcResultNum[i], motion_so_ev);
					}
					break;
				}
				case HudResult::ResultNumType::TRICKS:
				{
					if (HudSonicStage::scoreEnabled && !*pClassicSonicContext && !Common::IsCurrentStageBossBattle() && stageHasTricks)
					{
						rcResultNum[i]->GetNode("num_5")->SetText(std::to_string((int)m_stageData.m_trickScore).c_str());
						HudResult_PlayMotion(rcResultNum[i], motion_so_ev);
					}
					break;
				}
				case HudResult::ResultNumType::TOTAL:
				{
					rcResultNum[i]->GetNode("num_6")->SetText(std::to_string(m_resultData.m_score).c_str());
					HudResult_PlayMotion(rcResultNum[i], motion_so_ev);
					break;
				}
				default: break;
				}
			}
			break;
		}
		case HudResult::ResultState::NewRecord:
		{
			printf("[Unleashed HUD] Result State: New Record\n");

			hasNewRecord = false;
			constexpr float newRecordX = 0.628125f * 1280.0f;

			// New record data, based on sub_10B7F00
			rcResultNewRecordTime = rcProjectResult->CreateScene("result_newR");
			if (((1 << (0 & 0x1F)) & *(uint32_t*)((uint32_t)This + 580 + 4 * (0 >> 5))) != 0)
			{
				rcResultNewRecordTime->SetPosition(HudSonicStage::xAspectOffset / 2 + newRecordX, HudSonicStage::yAspectOffset / 2 + 0.32083333f * 720.0f);
				HudResult_PlayMotion(rcResultNewRecordTime, motion_so_ev);
				hasNewRecord = true;
			}
			else
			{
				rcResultNewRecordTime->SetHideFlag(true);
			}

			rcResultNewRecordScore = rcProjectResult->CreateScene("result_newR");
			if (((1 << (2 & 0x1F)) & *(uint32_t*)((uint32_t)This + 580 + 4 * (2 >> 5))) != 0)
			{
				rcResultNewRecordScore->SetPosition(HudSonicStage::xAspectOffset / 2 + newRecordX, HudSonicStage::yAspectOffset / 2 + 0.7861111f * 720.0f);
				HudResult_PlayMotion(rcResultNewRecordScore, motion_so_ev);
				hasNewRecord = true;
			}
			else
			{
				rcResultNewRecordScore->SetHideFlag(true);
			}

			break;
		}
		case HudResult::ResultState::Rank:
		{
			printf("[Unleashed HUD] Result State: Rank\n");

			rcResultRankText = rcProjectResult->CreateScene("result_rank");
			HudResult_PlayMotion(rcResultRankText, "Intro_Anim");

			std::string rankSceneName;
			switch (m_resultData.m_perfectRank)
			{
			case HudResult::ResultRankType::S: rankSceneName = "result_rank_S"; break;
			case HudResult::ResultRankType::A: rankSceneName = "result_rank_A"; break; 
			case HudResult::ResultRankType::B: rankSceneName = "result_rank_B"; break; 
			case HudResult::ResultRankType::C: rankSceneName = "result_rank_C"; break; 
			case HudResult::ResultRankType::D: rankSceneName = "result_rank_D"; break; 
			default: rankSceneName = "result_rank_E"; break;
			}

			rcResultRank = rcProjectResult->CreateScene(rankSceneName.c_str());
			HudResult_PlayMotion(rcResultRank, "Intro_Anim");
			break;
		}
		case HudResult::ResultState::Footer:
		{
			printf("[Unleashed HUD] Result State: Footer\n");

			rcResultFooter = rcProjectResult->CreateScene("result_footer");

			break;
		}
		}

		m_resultTimer = 0.0f;
	}

	// Loop animation for rank S,A,B
	if (rcResultRank && rcResultRank->m_MotionDisableFlag &&
	(m_resultData.m_perfectRank == HudResult::ResultRankType::S || m_resultData.m_perfectRank == HudResult::ResultRankType::A || m_resultData.m_perfectRank == HudResult::ResultRankType::B))
	{
		HudResult_PlayMotion(rcResultRank, "Usual_Anim", true);
	}

	m_resultTimer += in_rUpdateInfo.DeltaTime;

	// Bar sliding sfx
	if (m_resultState == HudResult::ResultState::Main)
	{
		static float barTime[] = { 1.0f, 1.18333f, 1.3333f, 1.45f };
		for (int i = 0; i < (HudSonicStage::scoreEnabled && !Common::IsCurrentStageBossBattle() ? (*pClassicSonicContext ? 3 : 4) : 1); i++)
		{
			if (m_resultTimer > barTime[i] && !m_soundState.m_bar[i])
			{
				m_soundState.m_bar[i] = true;
				static SharedPtrTypeless barSoundHandle;
				Common::PlaySoundStatic(barSoundHandle, 1000020);
			}
		}
	}

	// Total/New record sfx
	if (m_resultState == HudResult::ResultState::NewRecord)
	{
		if (m_resultTimer >= 0.0667f && !m_soundState.m_total)
		{
			m_soundState.m_total = true;

			if (hasNewRecord)
			{
				static SharedPtrTypeless newRecordSoundHandle;
				Common::PlaySoundStatic(newRecordSoundHandle, 1000053);
			}

			static SharedPtrTypeless totalSoundHandle;
			Common::PlaySoundStatic(totalSoundHandle, 1000021);
		}
	}

	// Rank slam/rank quote sfx
	if (m_resultState == HudResult::ResultState::Rank)
	{
		if (m_resultTimer >= 0.1667f && !m_soundState.m_rank)
		{
			m_soundState.m_rank = true;

			uint32_t cueID;
			switch (m_resultData.m_perfectRank)
			{
			case HudResult::ResultRankType::S: cueID = 1000041; break;
			case HudResult::ResultRankType::A: cueID = 1000042; break;
			case HudResult::ResultRankType::B: cueID = 1000043; break;
			case HudResult::ResultRankType::C: cueID = 1000044; break;
			case HudResult::ResultRankType::D: cueID = 1000045; break;
			default: cueID = 1000046; break;
			}

			static SharedPtrTypeless rankSoundHandle;
			Common::PlaySoundStatic(rankSoundHandle, cueID);
		}

		if (!Common::IsPlayerSuper() && m_resultTimer >= 0.6833f && !m_soundState.m_rankVoice)
		{
			m_soundState.m_rankVoice = true;

			uint32_t cueID;
			switch (m_resultData.m_perfectRank)
			{
			case HudResult::ResultRankType::S: cueID = 40000; break;
			case HudResult::ResultRankType::A: cueID = 40001; break;
			case HudResult::ResultRankType::B: cueID = 40002; break;
			case HudResult::ResultRankType::C: cueID = 40003; break;
			case HudResult::ResultRankType::D: cueID = 40004; break;
			default: cueID = 40005; break;
			}

			static SharedPtrTypeless rankVoiceHandle;
			Common::PlaySoundStatic(rankVoiceHandle, cueID);
		}
	}
	
	// Sonic clapping in rank C (Unleashed Model only)
	if (m_modelType == HudResult::ModelType::SWA_Hedgehog && m_resultData.m_perfectRank == HudResult::ResultRankType::C && !Common::IsPlayerSuper())
	{
		auto const* context = Sonic::Player::CPlayerSpeedContext::GetInstance();
		Sonic::Message::MsgGetAnimationInfo message;
		context->m_pPlayer->ProcessMessage(message, true);
		if (std::strstr(message.m_Name.c_str(), "ResultRankC_Link") != 0)
		{
			static float clapFrame[] = { 91.0f, 109.0f, 125.0f, 141.0f };
			for (int i = 0; i < 4; i++)
			{
				if (message.m_Frame >= clapFrame[i] && !m_soundState.m_rankCClaps[i])
				{
					m_soundState.m_rankCClaps[i] = true;
					static SharedPtrTypeless clapSoundHandle;
					Common::PlaySoundStatic(clapSoundHandle, 1000054);
				}
			}
		}
	}
	
	// State transition
	switch (m_resultState)
	{
	case HudResult::ResultState::Idle: break;
	case HudResult::ResultState::MainWait:
	{
		if (m_resultTimer > cResultMainDelay)
		{
			m_resultStateNew = HudResult::ResultState::Main;
		}
		break;
	}
	case HudResult::ResultState::Main:
	{
		if (m_resultTimer > cResultNewRecordDelay)
		{
			m_resultStateNew = HudResult::ResultState::NewRecord;
		}
		break;
	}
	case HudResult::ResultState::NewRecord:
	{
		if (m_resultTimer > cResultRankDelay)
		{
			m_resultStateNew = HudResult::ResultState::Rank;
		}
		break;
	}
	case HudResult::ResultState::Rank:
	{
		if (m_resultTimer > cResultFooterDelay)
		{
			m_resultStateNew = HudResult::ResultState::Footer;
		}
		break;
	}
	case HudResult::ResultState::Footer:
	{
		if (*(uint32_t*)0x10B96E6 == 0xFFD285E8)
		{
			static SharedPtrTypeless soundHandle;
			Sonic::SPadState const* padState = &Sonic::CInputState::GetInstance()->GetPadState();
			if (padState->IsTapped(Sonic::EKeyState::eKeyState_A))
			{
				m_resultStateNew = HudResult::ResultState::Medal;

				rcItemResultWindow->SetHideFlag(false);
				CSDCommon::PlayAnimation(*rcItemResultWindow, motion_so_ev_2, Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);

				rcItemResultContents->SetHideFlag(false);
				CSDCommon::PlayAnimation(*rcItemResultContents, "usual_Anim_2", Chao::CSD::eMotionRepeatType_Loop, 1, 0);

				for (int i = 0; i < HudResult::ResultNumType::COUNT; i++)
				{
					if (rcResultNum[i])
					{
						Chao::CSD::CProject::DestroyScene(rcProjectResult.Get(), rcResultNum[i]);
					}
				}
				Chao::CSD::CProject::DestroyScene(rcProjectResult.Get(), rcResultNewRecordTime);
				Chao::CSD::CProject::DestroyScene(rcProjectResult.Get(), rcResultNewRecordScore);
				Chao::CSD::CProject::DestroyScene(rcProjectResult.Get(), rcResultRankText);
				Chao::CSD::CProject::DestroyScene(rcProjectResult.Get(), rcResultRank);
			}
		}

		break;
	}
	case HudResult::ResultState::Medal:
	{
		if (*(uint32_t*)0x10B96E6 == 0xFFD285E8) {
			Sonic::SPadState const* padState = &Sonic::CInputState::GetInstance()->GetPadState();
			medalTimer += in_rUpdateInfo.DeltaTime;
			if (padState->IsTapped(Sonic::EKeyState::eKeyState_A) && medalTimer >= 0.5f)
			{
				m_resultStateNew = HudResult::ResultState::Status;

				Chao::CSD::CProject::DestroyScene(rcProjectResult.Get(), rcResultTitle);
				Chao::CSD::CProject::DestroyScene(rcProjectResult.Get(), rcResultFooter);

				CSDCommon::PlayAnimation(*rcItemResultWindow, motion_so_ev_2, Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);

				CSDCommon::FreezeMotion(*rcItemResultContents);
				rcItemResultContents->SetHideFlag(true);

				HudStatus::Kill();
				HudStatus::Start();

				medalTimer = 0.0f;
			}
		}
		break;
	}
	case HudResult::ResultState::Status: {
		if (*(uint32_t*)0x10B96E6 == 0xFFD285E8) {
			static SharedPtrTypeless soundHandle;
			Sonic::SPadState const* padState = &Sonic::CInputState::GetInstance()->GetPadState();
			statusTimer += in_rUpdateInfo.DeltaTime;
			if (padState->IsTapped(Sonic::EKeyState::eKeyState_A) && statusTimer >= 2.5f && HudStatus::CanEnd())
			{
				m_resultStateNew = HudResult::ResultState::FadeOut;

				statusTimer = 0.0f;
				HudStatus::End();

				WRITE_JUMP(0x10B96E6, (void*)0x10B974B);
				WRITE_NOP(0x10B9976, 5);
				Common::PlaySoundStatic(soundHandle, 1000005);

				HudLoading::StartFadeOut();
			}
		}
		break;
	}
	case HudResult::ResultState::FadeOut: break;
	}
}

void DebugCombinedScore()
{
	printf("[Unleashed HUD] Score: %.f\n", m_stageData.m_enemyScore + m_stageData.m_trickScore);
}

HOOK(int, __fastcall, HudResult_MsgRestartStage, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	m_stageData = HudResult::StageData();
	return originalHudResult_MsgRestartStage(This, Edx, message);
}

HOOK(void, __fastcall, HudResult_LapTimeHud, 0x1097640, Sonic::CGameObject* This, void* Edx, hh::fnd::Message& in_rMsg)
{
	originalHudResult_LapTimeHud(This, Edx, in_rMsg);

	auto const* context = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (!context) return;

	float score = max(0.0f, context->m_Velocity.norm() - 20.0f) * 150.0f;
	m_stageData.m_speedScore = max(m_stageData.m_speedScore, score);
	printf("[Unleashed HUD] Speed Score = %.f\n", m_stageData.m_speedScore);
}

HOOK(void, __fastcall, HudResult_CSonicUpdate, 0xE6BF20, void* This, void* Edx, float* dt)
{
	originalHudResult_CSonicUpdate(This, Edx, dt);

	auto const* context = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (!context) return;

	if (context->m_Grounded)
	{
		m_isEnemyChain = false;
	}
}

void AddEnemyScore()
{
	m_stageData.m_enemyScore += m_enemyBonus;
	if (m_isEnemyChain)
	{
		m_stageData.m_enemyScore += m_enemyChainBonus;
		printf("[Unleashed HUD] Enemy Score = %.f (+%.f Chain Bonus)\n", m_stageData.m_enemyScore, m_enemyBonus + m_enemyChainBonus);
		DebugCombinedScore();
	}
	else
	{
		printf("[Unleashed HUD] Enemy Score = %.f (+%.f)\n", m_stageData.m_enemyScore, m_enemyBonus);
		DebugCombinedScore();
		auto const* context = Sonic::Player::CPlayerSpeedContext::GetInstance();
		if (context)
		{
			m_isEnemyChain = !context->m_Grounded;
		}
	}
}

void __declspec(naked) HudResult_Enemy()
{
	static uint32_t returnAddress = 0xBDDD56;
	__asm
	{
		push	eax
		call	AddEnemyScore
		pop		eax

		mov     ecx, [eax + 0A4h]
		jmp		[returnAddress]
	}
}

void AddRainbowRingScore()
{
	m_stageData.m_trickScore += m_rainbowRingBonus;
	printf("[Unleashed HUD] Trick Score = %.f (+%.f Rainbow Ring)\n", m_stageData.m_trickScore, m_rainbowRingBonus);
	DebugCombinedScore();
}

void __declspec(naked) HudResult_RainbowRing()
{
	static uint32_t returnAddress = 0x115A947;
	__asm
	{
		push	eax
		call	AddRainbowRingScore
		pop		eax

		mov     eax, [eax + 134h]
		jmp		[returnAddress]
	}
}

HOOK(int, __fastcall, HudResult_CTrickSimpleBegin, 0xE4B050, uint32_t* This)
{
	m_trickChain = 0;
	return originalHudResult_CTrickSimpleBegin(This);
}

HOOK(void, __fastcall, HudResult_CTrickSimpleAdvance, 0xE4B3F0, uint32_t* This)
{
	uint32_t trickCountPrev = This[35];
	uint32_t statePrev = This[24];
	originalHudResult_CTrickSimpleAdvance(This);

	// Performed trick
	if (This[35] > trickCountPrev)
	{
		// Trick level 0
		if (This[34] == 0)
		{
			m_trickChain = 0;
		}

		m_trickChain++;
		float bonus = m_trickBonus;
		for (int i = 0; i < m_trickChain - 1; i++)
		{
			bonus = min(m_trickBonusLimit, bonus * m_trickChainMultiplier);
		}
		m_stageData.m_trickScore += bonus;
		printf("[Unleashed HUD] Trick Score = %.f (+%.f Trick Bonus Chain %d)\n", m_stageData.m_trickScore, bonus, m_trickChain);
		DebugCombinedScore();
	}

	// Trick finish
	if (This[24] == 3 && statePrev != 3)
	{
		m_stageData.m_trickScore += m_trickFinishBonus;
		printf("[Unleashed HUD] Trick Score = %.f (+%.f Trick Finish)\n", m_stageData.m_trickScore, m_trickFinishBonus);
		DebugCombinedScore();
	}
}

//---------------------------------------------------
// Result music
//---------------------------------------------------
const char* SNG19_JNG_SWA = "SNG19_JNG_SWA";
HOOK(int, __fastcall, HudResult_SNG19_JNG_1, 0xCFF440, void* This, void* Edx, int a2)
{
	WRITE_MEMORY(0xCFF44E, char*, SNG19_JNG_SWA);
	return originalHudResult_SNG19_JNG_1(This, Edx, a2);
}

HOOK(void, __fastcall, HudResult_SNG19_JNG_2, 0xD00F70, void* This, void* Edx, int a2)
{
	WRITE_MEMORY(0xD01A06, char*, SNG19_JNG_SWA);
	originalHudResult_SNG19_JNG_2(This, Edx, a2);
}

HOOK(void, __fastcall, HudResult_CStateGoalFadeInBegin, 0xCFD2D0, void* This)
{
	static const char* Result_Boss = "Result_Boss";
	static const char* Result = (char*)0x15B38F0;

	if (Common::IsCurrentStageBossBattle())
	{
		WRITE_MEMORY(0xCFD3C9, char*, Result_Boss);
	}
	else
	{
		WRITE_MEMORY(0xCFD3C9, char*, Result);
	}

	// Music length
	static double length = 6.121;
	WRITE_MEMORY(0xCFD562, double*, &length);

	// Use Result2 if above 4 (rank E is 0xFFFFFFFF)
	WRITE_MEMORY(0xCFD4E5, uint8_t, 0x76);

	originalHudResult_CStateGoalFadeInBegin(This);
}

//---------------------------------------------------
// E-rank animations
//---------------------------------------------------
HOOK(bool, __fastcall, HudResult_MsgChangeResultState, 0xE27BA0, void* This, void* Edx, uint32_t a2)
{
	if (!*(int*)(a2 + 16))
	{
		if (*(int*)(a2 + 20) == -1)
		{
			HudResult::GetModelType();

			WRITE_STRING(0x15EFEC4, "SonicRankE");
			switch (m_modelType)
			{
			case HudResult::ModelType::Gens:
				WRITE_STRING(0x15E8E80, "sn_result04");
				WRITE_STRING(0x15E8E6C, "sn_result04_loop");
				WRITE_STRING(0x15D60F4, "ssn_result04");
				WRITE_STRING(0x15D60E0, "ssn_result04_loop");
				break;
			case HudResult::ModelType::SWA_Hedgehog:
				WRITE_STRING(0x15E8E80, "so_result04");
				WRITE_STRING(0x15E8E6C, "so_result04_loop");
				WRITE_STRING(0x15D60F4, "sso_result04");
				WRITE_STRING(0x15D60E0, "sso_result04_loop");
				break;
			case HudResult::ModelType::SWA_Werehog:
				WRITE_STRING(0x15E8E80, "ev_result04");
				WRITE_STRING(0x15E8E6C, "ev_result04_loop");
				WRITE_STRING(0x15D60F4, "sev_result04");
				WRITE_STRING(0x15D60E0, "sev_result04_loop");
				break;
			}
		}
		else
		{
			WRITE_STRING(0x15EFEC4, "SonicRankD");
			WRITE_STRING(0x15E8E80, "sn_result04");
			WRITE_STRING(0x15E8E6C, "sn_result04_loop");
			WRITE_STRING(0x15D60F4, "ssn_result04");
			WRITE_STRING(0x15D60E0, "ssn_result04_loop");
		}
	}

	return originalHudResult_MsgChangeResultState(This, Edx, a2);
}

//---------------------------------------------------
// Look for tricks
//---------------------------------------------------
std::string m_setdataLayer;

HOOK(bool, __stdcall, HudResult_ParseSetData, 0xEB5050, void* a1, char** pFileName, void* a3, void* a4, uint8_t a5, uint8_t a6)
{
	m_setdataLayer = std::string(*(char**)pFileName);
	bool result = originalHudResult_ParseSetData(a1, pFileName, a3, a4, a5, a6);
	m_setdataLayer.clear();

	return result;
}

HOOK(uint32_t*, __fastcall, HudResult_ReadXmlData, 0xCE5FC0, uint32_t size, char* pData, void* a3, uint32_t* a4)
{
	if (!m_setdataLayer.empty())
	{
		char* trickRainbow = strstr(pData, "<RainbowRing>");
		char* trickRamp = strstr(pData, "<AdlibTrickJump>");
		if (trickRamp || trickRainbow) {
			stageHasTricks = true;
		}
	}

	return originalHudResult_ReadXmlData(size, pData, a3, a4);
}
void HudResult::Install()
{
	// Get score multiplier so we can track them
	INIReader reader("ScoreGenerations.ini");
	m_enemyBonus = reader.GetFloat("Score", "Enemy", 300.0f);
	m_enemyChainBonus = reader.GetFloat("GameplayBonus", "homingChainBonus", 200.0f);
	m_trickBonus = reader.GetFloat("Score", "Trick", 100.0f);
	m_trickBonusLimit = reader.GetFloat("GameplayBonus", "trickBonusLimit", 400.0f);
	m_trickChainMultiplier = reader.GetFloat("Multiplier", "trickMultiplier", 2.0f);
	m_trickFinishBonus = reader.GetFloat("Score", "TrickFinish", 2000.0f);
	m_rainbowRingBonus = reader.GetFloat("Score", "RainbowRing", 1000.0f);

	// Use Unleashed goal camera (default) param
	// CameraSp -> CameraSu (this doesn't read CameraSu, just fall back to default values)
	WRITE_MEMORY(0x15AF4C3, uint8_t, 0x75);
	WRITE_MEMORY(0x15D1EBB, uint8_t, 0x75);
	WRITE_MEMORY(0x15D293B, uint8_t, 0x75);
	WRITE_MEMORY(0x1A48C7C, float, -0.6053f); // -0.08 from default OffsetRight

	// Prevent using Gen's result
	INSTALL_HOOK(HudResult_MsgStartGoalResult);

	// Tricks
	INSTALL_HOOK(HudResult_ParseSetData);
	INSTALL_HOOK(HudResult_ReadXmlData);

	// Data collection
	INSTALL_HOOK(HudResult_CStateGoalFadeBeforeBegin);
	INSTALL_HOOK(HudResult_CStateGoalFadeInEnd);

	// Result HUD handling
	INSTALL_HOOK(HudResult_CHudResultAddCallback);
	WRITE_MEMORY(0x16A1C38, void*, HudResult_CHudResultRemoveCallback);
	INSTALL_HOOK(HudResult_CHudResultAdvance);

	// Record score stats (only for cosmetics if Score Generations isn't enabled)
	INSTALL_HOOK(HudResult_MsgRestartStage);
	INSTALL_HOOK(HudResult_LapTimeHud);
	INSTALL_HOOK(HudResult_CSonicUpdate);
	WRITE_JUMP(0xBDDD50, HudResult_Enemy);
	WRITE_JUMP(0x115A941, HudResult_RainbowRing);
	INSTALL_HOOK(HudResult_CTrickSimpleBegin);
	INSTALL_HOOK(HudResult_CTrickSimpleAdvance);

	// Patch result music
	if (Configuration::unleashedResultMusic)
	{
		INSTALL_HOOK(HudResult_SNG19_JNG_1);
		INSTALL_HOOK(HudResult_SNG19_JNG_2);
		INSTALL_HOOK(HudResult_CStateGoalFadeInBegin);
	}
	else
	{
		// Changed to signed comparison to include E-rank (-1)
		WRITE_MEMORY(0xCFD4E5, uint8_t, 0x7D);
	}

	// E-rank animations
	INSTALL_HOOK(HudResult_MsgChangeResultState);
}

void HudResult::GetModelType()
{
	m_modelType = HudResult::ModelType::Gens;
	auto const* context = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (context)
	{
		if (!context->m_pPlayer->m_spCharacterModel)
		{
			return;
		}
		else if (context->m_pPlayer->m_spCharacterModel->GetNode("SonicRoot") != nullptr)
		{
			m_modelType = HudResult::ModelType::SWA_Hedgehog;
		}
		else if (context->m_pPlayer->m_spCharacterModel->GetNode("EvilRoot") != nullptr)
		{
			m_modelType = HudResult::ModelType::SWA_Werehog;
		}
	}
}
