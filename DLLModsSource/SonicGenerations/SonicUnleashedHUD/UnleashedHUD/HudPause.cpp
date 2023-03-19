#include "HudPause.h"

Chao::CSD::RCPtr<Chao::CSD::CProject> m_projectPause;
boost::shared_ptr<Sonic::CGameObjectCSD> m_spPause;
Chao::CSD::RCPtr<Chao::CSD::CScene> m_scenePauseBG;
Chao::CSD::RCPtr<Chao::CSD::CScene> m_scenePauseHeader;
Chao::CSD::RCPtr<Chao::CSD::CScene> m_scenePauseWindow;
Chao::CSD::RCPtr<Chao::CSD::CScene> m_scenePauseSelect;
Chao::CSD::RCPtr<Chao::CSD::CScene> m_scenePauseFooter;

uint32_t m_cursorPos = 0;

void HudPause_PlayMotion(Chao::CSD::RCPtr<Chao::CSD::CScene>& scene, char const* motion, bool loop = false, float start = 0.0f, float end = -1.0f)
{
    if (!scene) return;
    scene->SetHideFlag(false);
    scene->SetMotion(motion);
    scene->SetMotionFrame(start);
    if (end > start)
    {
        scene->m_MotionEndFrame = end;
    }
    scene->m_MotionDisableFlag = false;
    scene->m_MotionSpeed = 1.0f;
    scene->m_MotionRepeatType = loop ? Chao::CSD::eMotionRepeatType_Loop : Chao::CSD::eMotionRepeatType_PlayOnce;
    scene->Update();
}

void HudPause_StopMotion(Chao::CSD::RCPtr<Chao::CSD::CScene>& scene, char const* motion, float frame)
{
    if (!scene) return;
    scene->SetHideFlag(false);
    scene->SetMotion(motion);
    scene->SetMotionFrame(frame);
    scene->m_MotionDisableFlag = true;
    scene->m_MotionSpeed = 0.0f;
    scene->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
    scene->Update();
}

void __fastcall HudPause_CPauseRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
    if (m_spPause)
    {
        m_spPause->SendMessage(m_spPause->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
        m_spPause = nullptr;
    }

    Chao::CSD::CProject::DestroyScene(m_projectPause.Get(), m_scenePauseBG);
    Chao::CSD::CProject::DestroyScene(m_projectPause.Get(), m_scenePauseHeader);
    Chao::CSD::CProject::DestroyScene(m_projectPause.Get(), m_scenePauseWindow);
    Chao::CSD::CProject::DestroyScene(m_projectPause.Get(), m_scenePauseSelect);
    Chao::CSD::CProject::DestroyScene(m_projectPause.Get(), m_scenePauseFooter);
    m_projectPause = nullptr;
}

void HudPause_CreatePauseScreen(uint32_t* This)
{
    Sonic::CGameObject* gameObject = (Sonic::CGameObject*)This[1];
    HudPause_CPauseRemoveCallback(gameObject, nullptr, nullptr);

    HudSonicStage::CalculateAspectOffsets();

    Sonic::CCsdDatabaseWrapper wrapper(gameObject->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());
    auto spCsdProject = wrapper.GetCsdProject("ui_pause_swa");
    m_projectPause = spCsdProject->m_rcProject;

    m_scenePauseBG = m_projectPause->CreateScene("bg");
    m_scenePauseBG->SetHideFlag(true);
    m_scenePauseHeader = m_projectPause->CreateScene("status_title");
    m_scenePauseHeader->SetHideFlag(true);
    m_scenePauseWindow = m_projectPause->CreateScene("bg_1");
    m_scenePauseWindow->SetHideFlag(true);
    m_scenePauseSelect = m_projectPause->CreateScene("bg_1_select");
    m_scenePauseSelect->SetHideFlag(true);

    m_scenePauseFooter = m_projectPause->CreateScene("footer_A");
    m_scenePauseFooter->SetHideFlag(true);

    if (m_projectPause && !m_spPause)
    {
        m_spPause = boost::make_shared<Sonic::CGameObjectCSD>(m_projectPause, 0.5f, "HUD_A2", true);
        Sonic::CGameDocument::GetInstance()->AddGameObject(m_spPause, "main", gameObject);
    }
}

void HudPause_OpenPauseWindow(bool isPam)
{
    m_cursorPos = 0;

    if (!m_projectPause) return;
    m_scenePauseWindow->GetNode("center")->SetScale(62.0f, isPam ? 0.5f : 0.9f);
    m_scenePauseWindow->GetNode("text_area")->SetScale(62.0f, isPam ? 0.5f : 0.9f);
    HudPause_PlayMotion(m_scenePauseWindow, "Intro_Anim");

    m_scenePauseFooter->SetHideFlag(false);
    HudPause_PlayMotion(m_scenePauseFooter, "Usual_Anim");
}

void HudPause_OpenPauseScreen(bool isPam)
{
    if (!m_projectPause) return;
    HudPause_PlayMotion(m_scenePauseBG, "Intro_Anim");
    HudPause_PlayMotion(m_scenePauseHeader, "Intro_Anim");

    m_scenePauseSelect->SetPosition(HudSonicStage::xAspectOffset / 2, HudSonicStage::yAspectOffset / 2 + (isPam ? -23.0f : -74.0f));
    m_scenePauseSelect->GetNode("img")->SetScale(38.0f, 1.0f);
    HudPause_PlayMotion(m_scenePauseSelect, "Usual_Anim", true);
    HudPause_StopMotion(m_scenePauseSelect, "Scroll_Anim", 0.0f);
    m_scenePauseSelect->SetHideFlag(true);

    m_scenePauseFooter->SetHideFlag(false);
    HudPause_PlayMotion(m_scenePauseFooter, "Usual_Anim");

    HudPause_OpenPauseWindow(isPam);
}

void HudPause_ClosePauseScreen()
{
    if (!m_projectPause) return;
    m_scenePauseBG->SetHideFlag(true);
    m_scenePauseHeader->SetHideFlag(true);
    m_scenePauseWindow->SetHideFlag(true);
    m_scenePauseSelect->SetHideFlag(true);
}

void HudPause_PauseCase(uint32_t* This, int Case, bool isPam)
{
    if (!m_projectPause) return;

    uint32_t cursorPos = This[3];
    auto& originalScene = *(Chao::CSD::RCPtr<Chao::CSD::CScene>*)(&This[9]);
    switch (Case)
    {
    case 0: // Start
    {
        originalScene->SetHideFlag(true);
        HudPause_OpenPauseScreen(isPam);
        break;
    }
    case 1: // End
    {
        m_scenePauseFooter->SetHideFlag(true);
        originalScene->SetHideFlag(true);
        break;
    }
    case 2: // Start animation complete
    {
        originalScene->SetHideFlag(false);
        if (isPam)
        {
            originalScene->SetMotionFrame(originalScene->m_MotionEndFrame);
        }
        m_scenePauseSelect->SetHideFlag(false);
        break;
    }
    case 3: // New cursor pos
    {
        originalScene->SetMotionFrame(originalScene->m_MotionEndFrame);
        HudPause_StopMotion(m_scenePauseSelect, "Usual_Anim", 0.0f);
        if (cursorPos == m_cursorPos + 1)
        {
            // Scroll down
            HudPause_PlayMotion(m_scenePauseSelect, "Scroll_Anim", false, m_cursorPos * 10.0f, cursorPos * 10.0f);
        }
        else if (cursorPos == m_cursorPos - 1)
        {
            // Scroll up
            HudPause_PlayMotion(m_scenePauseSelect, "Scroll_Anim", false, 120.0f - m_cursorPos * 10.0f, 120.0f - cursorPos * 10.0f);
        }
        else
        {
            HudPause_StopMotion(m_scenePauseSelect, "Scroll_Anim", cursorPos * 10.0f);
        }
        break;
    }
    case 4: // Confirm
    {
        break;
    }
    case 5: // Back from confirm dialog
    {
        originalScene->SetHideFlag(true);
        HudPause_OpenPauseWindow(isPam);
        break;
    }
    default: break;
    }

    m_cursorPos = cursorPos;
}

HOOK(bool, __fastcall, HudPause_CPauseVisualActInit, 0x109FAD0, uint32_t* This)
{
    HudPause_CreatePauseScreen(This);
    return originalHudPause_CPauseVisualActInit(This);
}

HOOK(bool, __fastcall, HudPause_CPauseVisualActOpened, 0x109F8F0, uint32_t* This)
{
    return m_scenePauseWindow->m_MotionDisableFlag == 1;
}

HOOK(int, __fastcall, HudPause_CPauseVisualActCase, 0x109F910, uint32_t* This, void* Edx, int Case)
{
    int result = originalHudPause_CPauseVisualActCase(This, Edx, Case);
    HudPause_PauseCase(This, Case, false);
    return result;
}

HOOK(void, __fastcall, HudPause_CPauseCStateSelectAdvance, 0x42A520, hh::fnd::CStateMachineBase::CStateBase* This)
{
    if (m_scenePauseSelect)
    {
        if (m_scenePauseSelect->m_MotionDisableFlag)
        {
            HudPause_PlayMotion(m_scenePauseSelect, "Usual_Anim", true, 0.0f);
        }
    }

    originalHudPause_CPauseCStateSelectAdvance(This);
}

HOOK(void, __fastcall, HudPause_CPauseCStateCloseBegin, 0x42A710, hh::fnd::CStateMachineBase::CStateBase* This)
{
    int* context = (int*)This->m_pContext;
    if (context[58] != -1)
    {
        // Unpause
        HudPause_ClosePauseScreen();
    }
    else
    {
        // HOWTO/WINDOW
        if (m_scenePauseWindow)
        {
            m_scenePauseWindow->SetHideFlag(true);
            m_scenePauseSelect->SetHideFlag(true);
            m_scenePauseFooter->SetHideFlag(true);
        }
    }

    originalHudPause_CPauseCStateCloseBegin(This);
}

HOOK(int, __fastcall, HudPause_CPauseCStateIdleBegin, 0x42A340, hh::fnd::CStateMachineBase::CStateBase* This)
{
    HudPause_ClosePauseScreen();
    return originalHudPause_CPauseCStateIdleBegin(This);
}

HOOK(bool, __fastcall, HudPause_CPauseVisualPamInit, 0x109E9D0, uint32_t* This)
{
    HudPause_CreatePauseScreen(This);
    return originalHudPause_CPauseVisualPamInit(This);;
}

HOOK(bool, __fastcall, HudPause_CPauseVisualPamOpened, 0x109E720, uint32_t* This)
{
    return m_scenePauseWindow->m_MotionDisableFlag == 1;
}

HOOK(int, __fastcall, HudPause_CPauseVisualPamCase, 0x109E740, uint32_t* This, void* Edx, int Case)
{
    int result = Case == 2 ? 0 : originalHudPause_CPauseVisualPamCase(This, Edx, Case);
    HudPause_PauseCase(This, Case, true);
    return result;
}

Chao::CSD::RCPtr<Chao::CSD::CProject> m_projectWindow;
boost::shared_ptr<Sonic::CGameObjectCSD> m_spWindow;
Chao::CSD::RCPtr<Chao::CSD::CScene> m_sceneWindow;
Chao::CSD::RCPtr<Chao::CSD::CScene> m_sceneWindowSelect;

void __fastcall HudPause_CWindowImplRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
    if (m_spWindow)
    {
        m_spWindow->SendMessage(m_spWindow->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
        m_spWindow = nullptr;
    }

    Chao::CSD::CProject::DestroyScene(m_projectWindow.Get(), m_sceneWindow);
    Chao::CSD::CProject::DestroyScene(m_projectWindow.Get(), m_sceneWindowSelect);

    m_projectWindow = nullptr;
}

void HudPause_CreateWindowScreen(Sonic::CGameObject* gameObject)
{
    HudPause_CWindowImplRemoveCallback(gameObject, nullptr, nullptr);

    HudSonicStage::CalculateAspectOffsets();

    Sonic::CCsdDatabaseWrapper wrapper(gameObject->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

    auto spCsdProject = wrapper.GetCsdProject("ui_pause_swa");
    m_projectWindow = spCsdProject->m_rcProject;

    m_scenePauseFooter = m_projectPause->CreateScene("footer_A");
    m_scenePauseFooter->SetHideFlag(true);

    spCsdProject = wrapper.GetCsdProject("ui_general_swa");
    m_projectWindow = spCsdProject->m_rcProject;

    m_sceneWindow = m_projectWindow->CreateScene("window");
    m_sceneWindow->SetHideFlag(true);
    m_sceneWindowSelect = m_projectWindow->CreateScene("window_select");
    m_sceneWindowSelect->SetHideFlag(true);

    if (m_projectWindow && !m_spWindow)
    {
        m_spWindow = boost::make_shared<Sonic::CGameObjectCSD>(m_projectWindow, 0.5f, "HUD_A2", true);
        Sonic::CGameDocument::GetInstance()->AddGameObject(m_spWindow, "main", gameObject);
    }
}

HOOK(int, __fastcall, HudPause_CWindowImplCStateOpenBegin, 0x439120, hh::fnd::CStateMachineBase::CStateBase* This)
{
    Sonic::CGameObject* parent = (Sonic::CGameObject*)(This->GetContextBase());
    HudPause_CreateWindowScreen(parent);

    float* context = (float*)(This->GetContextBase());
    float widthMotionRatio = context[242] * 0.01f * 0.8f;
    float heightMotionRatio = context[243] * 0.01f * 0.8f;
    m_sceneWindow->GetNode("center")->SetScale(30.0f + (152.0f - 30.0f) * widthMotionRatio, 0.2f + (2.35f - 0.2f) * heightMotionRatio);
    m_sceneWindow->GetNode("text_area")->SetScale(30.0f + (152.0f - 30.0f) * widthMotionRatio, 0.2f + (2.35f - 0.2f) * heightMotionRatio);
    HudPause_PlayMotion(m_sceneWindow, "Intro_Anim");

    m_scenePauseFooter->SetHideFlag(false);
    HudPause_PlayMotion(m_scenePauseFooter, "Usual_Anim_2");
    return originalHudPause_CWindowImplCStateOpenBegin(This);
}

HOOK(int, __fastcall, HudPause_CWindowImplCStateShowBegin, 0x4392A0, hh::fnd::CStateMachineBase::CStateBase* This)
{
    int result = originalHudPause_CWindowImplCStateShowBegin(This);

    float* context = (float*)(This->GetContextBase());
    float widthMotionRatio = context[242] * 0.01f * 0.8f;
    m_sceneWindowSelect->GetNode("img")->SetScale(19.0f + (95.0f - 19.0f) * widthMotionRatio, 1.0f);

    float heightMotion = context[243];
    float yPos = (heightMotion - 48.69f) * 2.2f;
    uint32_t cursorCount = ((uint32_t*)This)[12];
    if (cursorCount == 3)
    {
        // HACK: for 3 buttons move the cursor up by one slot
        yPos -= 50.0f;
    }

    if (cursorCount == 0)
    {
        m_sceneWindowSelect->SetHideFlag(true);
    }
    else
    {
        m_sceneWindowSelect->SetPosition(HudSonicStage::xAspectOffset / 2, HudSonicStage::yAspectOffset / 2 + yPos);
        HudPause_StopMotion(m_sceneWindowSelect, "Scroll_Anim", 0.0f);
        HudPause_PlayMotion(m_sceneWindowSelect, "Usual_Anim", true);
    }

    // Initial cursor pos
    uint32_t cursorPos = ((uint32_t*)context)[96];
    if (cursorPos > 0)
    {
        HudPause_StopMotion(m_sceneWindowSelect, "Scroll_Anim", cursorPos * 10.0f);
    }

    return result;
}

HOOK(void, __fastcall, HudPause_CWindowImplCStateShowAdvance, 0x439350, hh::fnd::CStateMachineBase::CStateBase* This)
{
    uint32_t* context = (uint32_t*)(This->GetContextBase());
    uint32_t cursorPos = context[96];

    originalHudPause_CWindowImplCStateShowAdvance(This);
    
    uint32_t cursorPosNew = context[96];
    if (cursorPosNew != cursorPos)
    {
        HudPause_StopMotion(m_sceneWindowSelect, "Usual_Anim", 0.0f);
        if (cursorPosNew == cursorPos + 1)
        {
            // Scroll down
            HudPause_PlayMotion(m_sceneWindowSelect, "Scroll_Anim", false, cursorPos * 10.0f, cursorPosNew * 10.0f);
        }
        else if (cursorPosNew == cursorPos - 1)
        {
            // Scroll up
            HudPause_PlayMotion(m_sceneWindowSelect, "Scroll_Anim", false, 200.0f - cursorPos * 10.0f, 200.0f - cursorPosNew * 10.0f);
        }
        else
        {
            HudPause_StopMotion(m_sceneWindowSelect, "Scroll_Anim", cursorPosNew * 10.0f);
        }
    }

    if (m_sceneWindowSelect->m_MotionDisableFlag)
    {
        HudPause_PlayMotion(m_sceneWindowSelect, "Usual_Anim", true);
    }
}

HOOK(int, __fastcall, HudPause_CWindowImplCStateCloseBegin, 0x4395A0, hh::fnd::CStateMachineBase::CStateBase* This)
{
    m_sceneWindow->SetHideFlag(true);
    m_sceneWindowSelect->SetHideFlag(true);
    m_scenePauseFooter->SetHideFlag(true);
    return originalHudPause_CWindowImplCStateCloseBegin(This);
}

void HudPause::Install()
{
    // PAM pause sfx
    WRITE_MEMORY(0x11D203A, uint32_t, 1000000);
    WRITE_MEMORY(0x11D1F8A, uint32_t, 1000001);

    //---------------------------------------------------
    // CPause
    //---------------------------------------------------
    WRITE_MEMORY(0x16A41A4, void*, HudPause_CPauseRemoveCallback);

    INSTALL_HOOK(HudPause_CPauseVisualActInit);
    INSTALL_HOOK(HudPause_CPauseVisualActOpened);
    INSTALL_HOOK(HudPause_CPauseVisualActCase);

    INSTALL_HOOK(HudPause_CPauseVisualPamInit);
    INSTALL_HOOK(HudPause_CPauseVisualPamOpened);
    INSTALL_HOOK(HudPause_CPauseVisualPamCase);

    INSTALL_HOOK(HudPause_CPauseCStateSelectAdvance);
    INSTALL_HOOK(HudPause_CPauseCStateCloseBegin);
    INSTALL_HOOK(HudPause_CPauseCStateIdleBegin);

    // Put HowTo layout to HUD_Pause
    WRITE_MEMORY(0x10AFC56, uint32_t, 0x1E66C18);

    // Don't show mission objective at pause
    WRITE_MEMORY(0xD00A46, uint8_t, 0);
    WRITE_MEMORY(0xD07489, uint8_t, 0);

    //---------------------------------------------------
    // CWindowImpl
    //---------------------------------------------------
    WRITE_MEMORY(0x16A6D84, void*, HudPause_CWindowImplRemoveCallback);
    INSTALL_HOOK(HudPause_CWindowImplCStateOpenBegin);
    INSTALL_HOOK(HudPause_CWindowImplCStateShowBegin);
    INSTALL_HOOK(HudPause_CWindowImplCStateShowAdvance);
    INSTALL_HOOK(HudPause_CWindowImplCStateCloseBegin);
}
