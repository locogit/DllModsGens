#include "HudLoading.h"
#include "Configuration.h"

int m_tailsTextStart = 0;
float m_tailsTextScrollTimer = 0.1f;
int const m_tailsTextSize = 34;
char const* m_tailsText[] =
{
	"<?tml version=\\\"1.0\\\" encode=\\\"TFF-S\\\" standalone=\\\"yes\\\"?>",
	"<assembly tmlns=\\\"urn:tails.com:asm.v1\\\" ApplicationVersion=\\\"1.0\\\">",
	"<assemblyIdentity",
	"version=\\\"1.0.0.0\\\"",
	"processor=\\\"T88\\\"",
	"name=\\\"TL1.exe\\\"",
	"type=\\\"Tal32\\\"",
	"/>",
	"<description>Description</description>",
	"<dependency>",
	"<dependent>",
	"<assemblyIdentity",
	"type=\\\"tails99\\\"",
	"name=\\\"Tales.Systems.Common-Controls\\\"",
	"version=\\\"1.2.3.4\\\"",
	"processor=\\\"T66\\\"",
	"KeyToken=\\\"stlwi19902828\\\"",
	"language=\\\"*\\\"",
	"/>",
	"</dependent>",
	"</dependency>",
	"<dependency>",
	"<dependent>",
	"<assemblyIdentity",
	"type=\\\"sonic64\\\"",
	"name=\\\"Sonic.Systems.Common-Controls\\\"",
	"version=\\\"1.2.3.4\\\"",
	"processor=\\\"S99\\\"",
	"KeyToken=\\\"98lgh12916\\\"",
	"language=\\\"*\\\"",
	"/>",
	"</dependent>",
	"</dependency>",
	"</assembly>",
};

HudLoading::LoadingArchiveType m_loadingArchiveType = HudLoading::LoadingArchiveType::LAT_Gens;
char const* m_loadingArchiveNames[] =
{
	"LoadingGens.ar",
	"LoadingUP.ar",
	"LoadingCustom.ar",
};

char const* m_loadingButtonArchiveNames[] =
{
	"LoadingXBOX.ar",
	"LoadingXBOX.ar",
	"LoadingPS.ar",
	"LoadingXBOX.ar",
};

void __fastcall HudLoading_ExtraLoadingARLImpl(int a1)
{
	uint32_t* v5 = *(uint32_t**)(*(uint32_t*)(a1 + 4) + 200);
	SharedPtrTypeless a2;
	int v37[53] = {};

	FUNCTION_PTR(int*, __thiscall, sub_446F90, 0x446F90, int* This, int, int);
	FUNCTION_PTR(void*, __thiscall, sub_69C270, 0x69C270, uint32_t * This, SharedPtrTypeless & a2, hh::base::CSharedString const& a3, hh::base::CSharedString const& a4, int* a5);
	FUNCTION_PTR(int, __thiscall, sub_446E30, 0x446E30, int* This);

	// Loading stage BG
	sub_446F90(v37, 200, 5);
	sub_69C270(v5, a2, "", (std::string(m_loadingArchiveNames[(int)m_loadingArchiveType]) + "l").c_str(), v37);
	sub_446E30(v37);
	
	// Loading buttons
	sub_446F90(v37, 200, 5);
	sub_69C270(v5, a2, "", (std::string(m_loadingButtonArchiveNames[(int)Configuration::buttonType]) + "l").c_str(), v37);
	sub_446E30(v37);
}

void __declspec(naked) HudLoading_ExtraLoadingARL()
{
	static uint32_t sub_446E30 = 0x446E30;
	static uint32_t returnAddress = 0xD6A362;
	__asm
	{
		call	[sub_446E30]
		mov		ecx, ebx
		call	HudLoading_ExtraLoadingARLImpl
		jmp		[returnAddress]
	}
}

void __declspec(naked) HudLoading_ExtraLoadingAR()
{
	static uint32_t sub_4FFD50 = 0x4FFD50;
	static uint32_t sub_4FF000 = 0x4FF000;
	static uint32_t sub_4FF0C0 = 0x4FF0C0;
	static uint32_t sub_4FF5B0 = 0x4FF5B0;
	static uint32_t sub_69AB10 = 0x69AB10;
	static uint32_t sub_446E30 = 0x446E30;
	static uint32_t returnAddress = 0xD6A45A;
	__asm
	{
		call	[sub_446E30]

		// Loading stage BG
		push    5
		lea     ecx, [esp + 104h - 0xD8]
		call    [sub_4FFD50]
		mov     eax, [ebx + 4]
		mov     edx, [eax + 90h]
		sub     esp, 0Ch
		add     eax, 90h
		mov     edi, esp
		sub     esp, 8
		mov     ecx, esp
		mov		[ecx], edx
		mov     eax, [eax + 4]
		mov		[ecx + 4], eax
		test    eax, eax
		jz      loc_D6A39D
		add     eax, 4
		mov     ecx, 1
		lock xadd[eax], ecx

		loc_D6A39D:
		push    0xD69590
		call    [sub_4FF000]
		add     esp, 0Ch
		lea     esi, [esp + 10Ch - 0xC0]
		call    [sub_4FF0C0]
		mov     eax, [esp + 100h - 0xA0]
		mov     esi, 0xD69430
		test    eax, eax
		jz      loc_D6A3E4
		test    al, 1
		jnz     loc_D6A3DC
		and		eax, 0FFFFFFFEh
		mov     eax, [eax]
		test    eax, eax
		jz      loc_D6A3DC
		lea     edx, [esp + 100h - 0x98]
		push    2
		push    edx
		mov     ecx, edx
		push    ecx
		call    eax
		add     esp, 0Ch

		loc_D6A3DC:
		mov		[esp + 100h - 0xA0], 0

		loc_D6A3E4:
		mov     edx, [esp + 100h - 0xDC]
		push    edx
		push    esi
		lea     esi, [esp + 108h - 0xA0]
		call    [sub_4FF5B0]
		mov		eax, m_loadingArchiveType
		mov		ecx, [m_loadingArchiveNames + eax * 4]
		push    ecx
		lea     ecx, [esp + 104h - 0xF0]
		call    [CStringConstructor]
		push    1
		push    0
		lea     eax, [esp + 108h - 0xD8]
		push    eax
		mov     eax, [ebx + 4]
		mov     edx, [eax + 90h]
		lea     ecx, [esp + 10Ch - 0xF0]
		push    ecx
		add     eax, 90h
		sub     esp, 8
		mov     ecx, esp
		mov		[ecx], edx
		mov     eax, [eax + 4]
		mov		[ecx + 4], eax
		test    eax, eax
		jz      loc_D6A43A
		add     eax, 4
		mov     ecx, 1
		lock xadd[eax], ecx
		
		loc_D6A43A:
		mov     edx, [ebx + 4]
		mov     ecx, [edx + 0C8h]
		call    [sub_69AB10]
		lea     ecx, [esp + 100h - 0xF0]
		call    [CStringDestructor]
		lea     ecx, [esp + 100h - 0xD8]
		call    [sub_446E30]

		// Loading buttons
		push    5
		lea     ecx, [esp + 104h - 0xD8]
		call    [sub_4FFD50]
		mov     eax, [ebx + 4]
		mov     edx, [eax + 90h]
		sub     esp, 0Ch
		add     eax, 90h
		mov     edi, esp
		sub     esp, 8
		mov     ecx, esp
		mov		[ecx], edx
		mov     eax, [eax + 4]
		mov		[ecx + 4], eax
		test    eax, eax
		jz      loc_D6A39D_2
		add     eax, 4
		mov     ecx, 1
		lock xadd[eax], ecx

		loc_D6A39D_2:
		push    0xD69590
		call    [sub_4FF000]
		add     esp, 0Ch
		lea     esi, [esp + 10Ch - 0xC0]
		call    [sub_4FF0C0]
		mov     eax, [esp + 100h - 0xA0]
		mov     esi, 0xD69430
		test    eax, eax
		jz      loc_D6A3E4_2
		test    al, 1
		jnz     loc_D6A3DC_2
		and		eax, 0FFFFFFFEh
		mov     eax, [eax]
		test    eax, eax
		jz      loc_D6A3DC_2
		lea     edx, [esp + 100h - 0x98]
		push    2
		push    edx
		mov     ecx, edx
		push    ecx
		call    eax
		add     esp, 0Ch

		loc_D6A3DC_2:
		mov		[esp + 100h - 0xA0], 0

		loc_D6A3E4_2:
		mov     edx, [esp + 100h - 0xDC]
		push    edx
		push    esi
		lea     esi, [esp + 108h - 0xA0]
		call    [sub_4FF5B0]
		mov		eax, Configuration::buttonType
		mov		ecx, [m_loadingButtonArchiveNames + eax * 4]
		push    ecx
		lea     ecx, [esp + 104h - 0xF0]
		call    [CStringConstructor]
		push    1
		push    0
		lea     eax, [esp + 108h - 0xD8]
		push    eax
		mov     eax, [ebx + 4]
		mov     edx, [eax + 90h]
		lea     ecx, [esp + 10Ch - 0xF0]
		push    ecx
		add     eax, 90h
		sub     esp, 8
		mov     ecx, esp
		mov		[ecx], edx
		mov     eax, [eax + 4]
		mov		[ecx + 4], eax
		test    eax, eax
		jz      loc_D6A43A_2
		add     eax, 4
		mov     ecx, 1
		lock xadd[eax], ecx
		
		loc_D6A43A_2:
		mov     edx, [ebx + 4]
		mov     ecx, [edx + 0C8h]
		call    [sub_69AB10]
		lea     ecx, [esp + 100h - 0xF0]
		call    [CStringDestructor]
		lea     ecx, [esp + 100h - 0xD8]
		call    [sub_446E30]

		jmp		[returnAddress]
	}
}

Chao::CSD::RCPtr<Chao::CSD::CProject> rcProjectLoading;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcLoadingBG1;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcLoadingPDA;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcLoadingPDATxt;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcLoadingInfo;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcLoadingEvent;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcLoadingBG2;
bool m_isBG1Intro = false;

void HudLoading_CreateScene(hh::fnd::CStateMachineBase::CStateBase* This)
{
	if (rcProjectLoading) return;

	void* contextBase = This->GetContextBase();
	rcProjectLoading = *(Chao::CSD::RCPtr<Chao::CSD::CProject>*)((uint32_t)contextBase + 188);
	if (!rcProjectLoading) return;

	rcLoadingBG1 = rcProjectLoading->CreateScene("bg_1");
	rcLoadingBG1->SetHideFlag(true);
	rcLoadingPDA = rcProjectLoading->CreateScene("pda");
	rcLoadingPDA->SetHideFlag(true);
	rcLoadingPDATxt = rcProjectLoading->CreateScene("pda_txt");
	rcLoadingPDATxt->SetHideFlag(true);
	rcLoadingInfo = rcProjectLoading->CreateScene("loadinfo");
	rcLoadingInfo->SetHideFlag(true);
	rcLoadingEvent = rcProjectLoading->CreateScene("event_viewer");
	rcLoadingEvent->SetHideFlag(true);
	rcLoadingBG2 = rcProjectLoading->CreateScene("bg_2");
	rcLoadingBG2->SetHideFlag(true);
}

void HudLoading_PlayMotion(Chao::CSD::RCPtr<Chao::CSD::CScene>& scene, char const* motion, float startFrame = 0.0f, bool loop = false)
{
	if (!scene) return;
	scene->SetHideFlag(false);
	scene->SetMotion(motion);
	scene->SetMotionFrame(startFrame);
	scene->m_MotionDisableFlag = false;
	scene->m_MotionSpeed = 1.0f;
	scene->m_MotionRepeatType = loop ? Chao::CSD::eMotionRepeatType_Loop : Chao::CSD::eMotionRepeatType_PlayOnce;
	scene->Update();
}

void HudLoading_SetTailsText()
{
	rcLoadingPDA->GetNode("source_1")->SetText(m_tailsText[(m_tailsTextStart + 0) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_2")->SetText(m_tailsText[(m_tailsTextStart + 1) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_3")->SetText(m_tailsText[(m_tailsTextStart + 2) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_4")->SetText(m_tailsText[(m_tailsTextStart + 3) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_5")->SetText(m_tailsText[(m_tailsTextStart + 4) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_6")->SetText(m_tailsText[(m_tailsTextStart + 5) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_7")->SetText(m_tailsText[(m_tailsTextStart + 6) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_8")->SetText(m_tailsText[(m_tailsTextStart + 7) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_9")->SetText(m_tailsText[(m_tailsTextStart + 8) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_10")->SetText(m_tailsText[(m_tailsTextStart + 9) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_11")->SetText(m_tailsText[(m_tailsTextStart + 10) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_12")->SetText(m_tailsText[(m_tailsTextStart + 11) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_13")->SetText(m_tailsText[(m_tailsTextStart + 12) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_14")->SetText(m_tailsText[(m_tailsTextStart + 13) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_15")->SetText(m_tailsText[(m_tailsTextStart + 14) % m_tailsTextSize]);
}

HOOK(int*, __fastcall, HudLoading_CHudLoadingCStateIdleBegin, 0x1092710, hh::fnd::CStateMachineBase::CStateBase* This)
{
	HudLoading_CreateScene(This);

	return originalHudLoading_CHudLoadingCStateIdleBegin(This);
}

bool m_isEvent = false;
bool m_loadingDisplayHint = false;
HOOK(void, __fastcall, HudLoading_CHudLoadingCStateIntroBegin, 0x10938F0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	char const* eventName = *(char**)Common::GetMultiLevelAddress(0x1E66B34, { 0x4, 0x1B4, 0x80, 0x2C });
	m_isEvent = !std::string(eventName).empty();

	HudLoading_CreateScene(This);

	m_loadingDisplayHint = false;
	HudLoading_SetTailsText();

	if (m_isEvent)
	{
		m_loadingDisplayHint = true;
		HudLoading_PlayMotion(rcLoadingEvent, "loop", 0.0f, true);
		HudLoading_PlayMotion(rcLoadingPDATxt, "Usual_Anim_2", 0.0f, true);
	}
	else
	{
		m_isBG1Intro = true;
		HudLoading_PlayMotion(rcLoadingBG1, "Intro_Anim", 100.0f);

		uint8_t stageID = Common::GetCurrentStageID() & 0xFF;
		rcLoadingPDA->GetNode("imgbox")->SetPatternIndex(stageID);
		rcLoadingPDA->GetNode("imgbox")->SetHideFlag(stageID == SMT_pam000 || Common::IsCurrentStageMission());
		rcLoadingPDA->GetNode("img2")->SetHideFlag(stageID != SMT_pam000);

		if (stageID == SMT_pam000)
		{
			HudLoading_PlayMotion(rcLoadingPDA, "Intro_Anim_1", 29.0f);
			HudLoading_PlayMotion(rcLoadingPDATxt, "Intro_Anim_1", 29.0f);
		}
		else if (Common::IsCurrentStageMission())
		{
			HudLoading_PlayMotion(rcLoadingPDA, "Intro_Anim_3", 29.0f);
			HudLoading_PlayMotion(rcLoadingPDATxt, "Intro_Anim_3", 29.0f);
			rcLoadingPDA->GetNode("hint")->SetHideFlag(true);
		}
		else
		{
			HudLoading_PlayMotion(rcLoadingPDA,"Intro_Anim_2", 29.0f);
			HudLoading_PlayMotion(rcLoadingPDATxt, "Usual_Anim_2", 0.0f, true);
			rcLoadingPDATxt->SetHideFlag(true);

			std::srand(static_cast<unsigned int>(std::time(nullptr)));
			int hintPattern = std::rand() % 7;
			if (stageID == SMT_blb)
			{
				hintPattern = 7;
				HudLoading_PlayMotion(rcLoadingInfo, Configuration::buttonType == Configuration::ButtonType::PS3 ? "ps3_super" : "360_super");
			}
			else
			{
				HudLoading_PlayMotion(rcLoadingInfo, Configuration::buttonType == Configuration::ButtonType::PS3 ? "ps3_sonic3" : "360_sonic3");
			}

			rcLoadingInfo->GetNode("controller")->SetHideFlag(true);
			rcLoadingInfo->GetNode("pos_text_sonic")->SetHideFlag(true);
			rcLoadingInfo->GetNode("pos_text_evil")->SetHideFlag(true);
			rcLoadingInfo->GetNode("imgbox_character")->SetHideFlag(false);
			rcLoadingInfo->GetNode("imgbox_character")->SetPatternIndex(hintPattern);
		}
	}

	originalHudLoading_CHudLoadingCStateIntroBegin(This);
}

HOOK(int32_t*, __fastcall, HudLoading_CHudLoadingCStateIntroAdvance, 0x10936B0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	uint8_t stageID = Common::GetCurrentStageID() & 0xFF;
	if (!m_loadingDisplayHint && rcLoadingPDA->m_MotionFrame > 45.0f)
	{
		if (stageID != SMT_pam000)
		{
			rcLoadingInfo->GetNode("controller")->SetHideFlag(false);
			rcLoadingInfo->GetNode("pos_text_sonic")->SetHideFlag(false);
			rcLoadingInfo->GetNode("pos_text_evil")->SetHideFlag(false);

			if (!Common::IsCurrentStageMission())
			{
				HudLoading_PlayMotion(rcLoadingPDATxt, "Usual_Anim_2", 0.0f, true);
			}
		}

		m_loadingDisplayHint = true;
	}

	if (m_isEvent)
	{
		This->m_pStateMachine->ChangeState("Usual");
	}
	else if (rcLoadingPDA->m_MotionDisableFlag)
	{
		HudLoading_PlayMotion(rcLoadingPDA, stageID == SMT_pam000 ? "Usual_Anim_1" : (Common::IsCurrentStageMission() ? "Usual_Anim_3" : "Usual_Anim_2"));
		This->m_pStateMachine->ChangeState("Usual");
	}

	if (rcLoadingPDATxt->m_MotionDisableFlag)
	{
		HudLoading_PlayMotion(rcLoadingPDATxt, Common::IsCurrentStageMission() ? "Usual_Anim_3" : "Usual_Anim_2", 0.0f, true);
	}

	return nullptr;
}

float m_applicationDeltaTime = 0.0f;
HOOK(void*, __fastcall, HudLoading_UpdateApplication, 0xE7BED0, void* This, void* Edx, float elapsedTime, uint8_t a3)
{
	m_applicationDeltaTime = elapsedTime;
	return originalHudLoading_UpdateApplication(This, Edx, elapsedTime, a3);
}

HOOK(int, __fastcall, HudLoading_CHudLoadingCStateUsualAdvance, 0x10926E0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	HudLoading_SetTailsText();

	m_tailsTextScrollTimer -= m_applicationDeltaTime;
	if (m_tailsTextScrollTimer < 0.0f)
	{
		m_tailsTextScrollTimer = 0.1f;
		m_tailsTextStart = (m_tailsTextStart + 1) % m_tailsTextSize;
	}

	return originalHudLoading_CHudLoadingCStateUsualAdvance(This);
}

HOOK(int32_t*, __fastcall, HudLoading_CHudLoadingCStateOutroBegin, 0x1093410, hh::fnd::CStateMachineBase::CStateBase* This)
{
	if (!m_isEvent)
	{
		m_isBG1Intro = false;
		HudLoading_PlayMotion(rcLoadingBG1, "Outro_Anim");
		HudLoading_PlayMotion(rcLoadingPDA, "Outro_Anim");
		HudLoading_PlayMotion(rcLoadingPDATxt, "Outro_Anim");
		HudLoading_PlayMotion(rcLoadingBG2, "Outro_Anim");
		rcLoadingInfo->SetHideFlag(true);

		uint8_t stageID = Common::GetCurrentStageID() & 0xFF;
		rcLoadingPDA->GetNode("img2")->SetHideFlag(stageID != SMT_pam000 && !Common::IsCurrentStageMission());
	}

	return originalHudLoading_CHudLoadingCStateOutroBegin(This);
}

HOOK(int32_t*, __fastcall, HudLoading_CHudLoadingCStateOutroAdvance, 0x1092EE0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	if (m_isEvent && This->m_Time > 0.24f)
	{
		rcLoadingEvent->SetHideFlag(true);
		rcLoadingPDATxt->SetHideFlag(true);
	}

	return originalHudLoading_CHudLoadingCStateOutroAdvance(This);
}

void __declspec(naked) HudLoading_StartResidentLoading()
{
	static uint32_t sub_75FA60 = 0x75FA60;
	static uint32_t returnAddress = 0x44A2F8;
	__asm
	{
		push	eax
		push	ecx
		call	HudLoading::StartResidentLoading
		pop		ecx
		pop		eax

		call	[sub_75FA60]
		jmp		[returnAddress]
	}
}

void __declspec(naked) HudLoading_EndResidentLoading()
{
	static uint32_t sub_75FA60 = 0x75FA60;
	static uint32_t returnAddress = 0x44A505;
	__asm
	{
		push	eax
		push	ecx
		call	HudLoading::EndResidentLoading
		pop		ecx
		pop		eax

		call	[sub_75FA60]
		jmp		[returnAddress]
	}
}

HOOK(void, __fastcall, HudLoading_CHudGateMenuMainCStateOutroBegin, 0x107B770, hh::fnd::CStateMachineBase::CStateBase* This)
{
	uint32_t contextBase = (uint32_t)This->GetContextBase();

	if (!*(bool*)(contextBase + 512)) // Not customize skill
	{
		uint32_t v8 = *(uint32_t*)(contextBase + 440);
		if (v8 != 1)
		{
			if (v8 == 2)
			{
				HudLoading::StartFadeOut();
			}
		}
		else
		{
			// Based on sub_107AC50
			uint32_t v9 = *(uint32_t*)(contextBase + 520);
			if (v9 != 7 && v9 != 11 && v9 != 13)
			{
				HudLoading::StartFadeOut();
			}
		}
	}
	originalHudLoading_CHudGateMenuMainCStateOutroBegin(This);
}

bool m_exitingStage = false;
void __declspec(naked) HudLoading_ExitStage()
{
	static uint32_t sub_10A0710 = 0x10A0710;
	static uint32_t skipAddress = 0x42AFBC;
	static uint32_t returnAddress = 0x42AF32;
	__asm
	{
		mov     eax, esi
		call	[sub_10A0710]
		sub		eax, 1
		jz		jump

		// exit stage
		mov		m_exitingStage, 1
		jmp		[skipAddress]

		// restart stage
		jump:
		jmp		[returnAddress]
	}
}

bool m_exitFadeStarted = false;
HOOK(int, __fastcall, HudLoading_CPauseCStateWindow, 0x42AEE0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	if (!m_exitingStage)
	{
		m_exitFadeStarted = false;
		WRITE_JUMP(0x42AF2D, HudLoading_ExitStage);
	}
	else
	{
		if (!m_exitFadeStarted)
		{
			HudLoading::StartFadeOut();
			m_exitFadeStarted = true;
		}
		else if (HudLoading::IsFadeOutCompleted())
		{
			m_exitingStage = false;
			m_exitFadeStarted = false;
			WRITE_MEMORY(0x42AF2D, uint8_t, 0xBA, 0x02, 0x00, 0x00, 0x00);
		}
	}

	return originalHudLoading_CPauseCStateWindow(This);
}

HOOK(void, __fastcall, HudLoading_CGameplayFlowStage_CStateTitle, 0xCF8F40, void* This)
{
	if (rcLoadingBG1 && m_isBG1Intro)
	{
		rcLoadingBG1->SetHideFlag(true);
	}
	originalHudLoading_CGameplayFlowStage_CStateTitle(This);
}

HOOK(bool, __stdcall, HudLoading_CEventScene, 0xB238C0, void* a1)
{
	// m_isBG1Intro is needed to avoid hiding it too early for stages with cutscene
	// Hiding is only meant for after stage cutscenes!
	if (rcLoadingBG1 && m_isBG1Intro)
	{
		rcLoadingBG1->SetHideFlag(true);
	}
	return originalHudLoading_CEventScene(a1);
}

bool m_isMissionLoadingText = false;
HOOK(void, __fastcall, HudLoading_ReadMissionConverseCommon, 0x6ADE20, uint32_t** This, void* Edx, int nameID, int serifuID, void* a4)
{
	char const* fontHeader = (char*)(This[1][2]);
	if (serifuID <= 1 && std::strstr(fontHeader, "Mission_Cnv") != 0)
	{
		if ((This[1][11] - This[1][10]) >> 3 != 0)
		{
			m_isMissionLoadingText = true;

			//uint32_t* v64 = *(uint32_t**)(This[1][10] + 8 * nameID);
			//uint32_t* caption = *(uint32_t**)(v64[2] + 8 * serifuID);

			//uint32_t const length = (caption[2] - caption[1]) / 4;
			//uint32_t* captionList = (uint32_t*)caption[1];
		}
	}

	originalHudLoading_ReadMissionConverseCommon(This, Edx, nameID, serifuID, a4);
	m_isMissionLoadingText = false;
}

HOOK(void*, __fastcall, HudLoading_SetConverseCommonInfo, 0x6AFBA0, void* This, void* Edx, uint32_t* info)
{
	if (m_isMissionLoadingText)
	{
		// Force mission text to be white
		info[3] = 0xE2FFFFFF;
	}
	else if (info[3] == 0xFF000000)
	{
		// Force black text to be white
		//info[3] = 0xFFFFFFFF;
	}
	return originalHudLoading_SetConverseCommonInfo(This, Edx, info);
}

void HudLoading::Install()
{
	// Load extra archives for loading screens
	if (Common::IsModNameEnabled("Unleashed Project"))
	{
		m_loadingArchiveType = HudLoading::LoadingArchiveType::LAT_UP;
	}
	else if (Common::DoesArchiveExist("LoadingCustom.ar.00"))
	{
		m_loadingArchiveType = HudLoading::LoadingArchiveType::LAT_Custom;
	}
	WRITE_JUMP(0xD6A35D, HudLoading_ExtraLoadingARL);
	WRITE_JUMP(0xD6A455, HudLoading_ExtraLoadingAR);

	// Always use stage loading even on PAM
	WRITE_MEMORY(0x1093EB8, uint8_t, 0xEB);
	WRITE_NOP(0x109273D, 9);

	// Disable loading screen sfx
	WRITE_MEMORY(0x44A2E8, int, -1);
	WRITE_MEMORY(0x44A4F5, int, -1);
	WRITE_MEMORY(0x10933C1, int, -1);
	WRITE_MEMORY(0x1093651, int, -1);
	WRITE_MEMORY(0x1093562, int, -1);
	WRITE_MEMORY(0x10936EC, int, -1);

	// State hooks
	INSTALL_HOOK(HudLoading_CHudLoadingCStateIdleBegin);
	INSTALL_HOOK(HudLoading_CHudLoadingCStateIntroBegin);
	INSTALL_HOOK(HudLoading_CHudLoadingCStateIntroAdvance);
	INSTALL_HOOK(HudLoading_CHudLoadingCStateUsualAdvance);
	INSTALL_HOOK(HudLoading_CHudLoadingCStateOutroBegin);
	INSTALL_HOOK(HudLoading_CHudLoadingCStateOutroAdvance);

	// Resident Loading
	WRITE_JUMP(0x44A2F3, HudLoading_StartResidentLoading);
	WRITE_JUMP(0x44A500, HudLoading_EndResidentLoading);

	// Transition out to loading screen
	INSTALL_HOOK(HudLoading_UpdateApplication);
	INSTALL_HOOK(HudLoading_CHudGateMenuMainCStateOutroBegin);
	INSTALL_HOOK(HudLoading_CPauseCStateWindow);
	INSTALL_HOOK(HudLoading_CGameplayFlowStage_CStateTitle);
	INSTALL_HOOK(HudLoading_CEventScene);

	// Set mission text anchored to top left, fix color
	WRITE_MEMORY(0x1A490C0, uint32_t, 1);
	WRITE_MEMORY(0x1A490C8, uint32_t, 1);
	static float s_missionTextHeight = 33.0f;
	WRITE_MEMORY(0x449E66, float*, &s_missionTextHeight);
	INSTALL_HOOK(HudLoading_ReadMissionConverseCommon);
	INSTALL_HOOK(HudLoading_SetConverseCommonInfo);
}

void HudLoading::StartFadeOut()
{
	if (!rcLoadingBG1) return;
	m_isBG1Intro = true;
	HudLoading_PlayMotion(rcLoadingBG1, "Intro_Anim");
}

bool HudLoading::IsFadeOutCompleted()
{
	if (!rcLoadingBG1) return true;
	return rcLoadingBG1->m_MotionDisableFlag;
}

void HudLoading::StartResidentLoading()
{
	if (!rcLoadingPDATxt) return;
	m_isBG1Intro = true;
	HudLoading_PlayMotion(rcLoadingBG1, "Intro_Anim", 100.0f);
	HudLoading_PlayMotion(rcLoadingPDATxt, "Usual_Anim_2", 0.0f, true);
}

void HudLoading::EndResidentLoading()
{
	if (!rcLoadingPDATxt) return;
	rcLoadingBG1->SetHideFlag(true);
	HudLoading_PlayMotion(rcLoadingPDATxt, "Outro_Anim");
}
