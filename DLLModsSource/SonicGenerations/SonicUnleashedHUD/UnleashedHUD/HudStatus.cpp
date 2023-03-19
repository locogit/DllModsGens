boost::shared_ptr<Sonic::CGameObject> status;

bool isWerehog = false;

class Stat {
public:
    const char* name;
    int txt_num;
    int level;
    int maxLevel;
    int count;

    Chao::CSD::RCPtr<Chao::CSD::CScene> m_tag_bg_2;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_tag_txt_2;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_bg_2;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_bar_2;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_num_2;

    Stat() {
        txt_num = 2;
        level = 0;
        count = 0;
        maxLevel = 0;
        name = "null";
    }
    Stat(const char* _name, int _level, int _maxLevel, int _txt_num, int _count) {
        name = _name;
        level = _level;
        maxLevel = _maxLevel;
        txt_num = _txt_num;
        count = _count;
    }
};

class CHudStatus : public Sonic::CGameObject
{
    Chao::CSD::RCPtr<Chao::CSD::CProject> m_rcStatus;
    boost::shared_ptr<Sonic::CGameObjectCSD> m_spStatus;

    Chao::CSD::RCPtr<Chao::CSD::CScene> m_status_title;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_logo;

    Chao::CSD::RCPtr<Chao::CSD::CScene> m_medal_info;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_medal_s_gauge;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_medal_m_gauge;

    Chao::CSD::RCPtr<Chao::CSD::CScene> m_tag_bg_1;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_tag_txt_1;

    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_bg_1;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_bar_1;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_num_1;


    Chao::CSD::RCPtr<Chao::CSD::CScene> m_decide_bg;

    Chao::CSD::RCPtr<Chao::CSD::CScene> m_status_footer;

    float timeFooter = 0.0f;
    bool footerVisible = false;

    float statDelay = 0.17f;
    int currentStatIndex = 0;
    float y = 0.0f;

    float statDelayStart = 0.0f;

    float switchCooldown = 0.0f;

    bool exit = false;

    const char* intro() {
        return isWerehog ? "Intro_ev_Anim" : "Intro_so_Anim";
    }

    const char* select() {
        return isWerehog ? "select_ev_Anim" : "select_so_Anim";
    }

    const char* outro() {
        return isWerehog ? "Outro_ev_Anim" : "Outro_so_Anim";
    }

    const char* intro2() {
        return isWerehog ? "Intro_ev_Anim_2" : "Intro_so_Anim_2";
    }

    int statusIndex;

    std::vector<Stat> statsDay = {};
    std::vector<Stat> statsNight = {};

    std::vector<Chao::CSD::RCPtr<Chao::CSD::CScene>> m_tag_bg_2 = {};
    std::vector<Chao::CSD::RCPtr<Chao::CSD::CScene>> m_tag_txt_2 = {};
    std::vector<Chao::CSD::RCPtr<Chao::CSD::CScene>> m_prgs_bg_2 = {};
    std::vector<Chao::CSD::RCPtr<Chao::CSD::CScene>> m_prgs_num_2 = {};
    std::vector<Chao::CSD::RCPtr<Chao::CSD::CScene>> m_prgs_bar_2 = {};

    float selectDelay = -1;

    float quitSoundTimer = 0.0f;

    float medalSubImage = 0.0f;

public:
    void HudRemove() {
        if (!m_rcStatus || !m_spStatus) { return; }

        RemoveStats();

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_status_title);
        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_logo);

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_medal_info);
        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_medal_s_gauge);
        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_medal_m_gauge);

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_tag_bg_1);
        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_tag_txt_1);
        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_prgs_bg_1);
        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_prgs_bar_1);
        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_prgs_num_1);

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_status_footer);
        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_decide_bg);

        if (m_spStatus) {
            m_spStatus->SendMessage(m_spStatus->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
            m_spStatus = nullptr;
        }

        m_rcStatus = nullptr;
    }

    void RemoveStats() {
        for (size_t i = 0; i < m_tag_bg_2.size(); i++)
        {
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_tag_bg_2[i]);
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_tag_txt_2[i]);
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_prgs_bg_2[i]);
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_prgs_num_2[i]);
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_prgs_bar_2[i]);
        }

        m_tag_bg_2.clear();
        m_tag_txt_2.clear();
        m_prgs_bg_2.clear();
        m_prgs_num_2.clear();
        m_prgs_bar_2.clear();
    }

    void ToggleStats() {
        isWerehog = !isWerehog;

        //Exp Bar
        m_tag_bg_1->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_tag_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_tag_txt_1->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_tag_txt_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_prgs_bg_1->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_prgs_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_prgs_bar_1->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_prgs_bar_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_prgs_num_1->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_prgs_num_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        // Quit Button
        m_decide_bg->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_decide_bg, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 0.0, 100.0, 100.0);

        // Header
        m_status_title->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_status_title, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 0.0, m_status_title->m_MotionEndFrame);

        // Character Portrait
        m_logo->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_logo, "Switch_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0, 20.0, false, !isWerehog);
        RemoveStats();
        statDelayStart = 0.75f;

        currentStatIndex = 0;
        statusIndex = 0;
        y = 0.0f;
    }

    void Select(int index, bool up, bool reverse, bool quit = false) {

        if (quit) {
            CSDCommon::PlayAnimation(*m_decide_bg, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
            CSDCommon::PlayAnimation(*m_tag_bg_2[index - 1], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*m_prgs_bg_2[index - 1], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*m_prgs_num_2[index - 1], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*m_prgs_bar_2[index - 1], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
        }
        else {
            if (reverse) {
                if (up && index == (isWerehog ? 4 : 1)) {
                    CSDCommon::PlayAnimation(*m_decide_bg, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                }

                if (up) {
                    CSDCommon::PlayAnimation(*m_tag_bg_2[std::clamp(index + 1, 0, (isWerehog ? 4 : 1))], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                    CSDCommon::PlayAnimation(*m_prgs_bg_2[std::clamp(index + 1, 0, (isWerehog ? 4 : 1))], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                    CSDCommon::PlayAnimation(*m_prgs_num_2[std::clamp(index + 1, 0, (isWerehog ? 4 : 1))], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                    CSDCommon::PlayAnimation(*m_prgs_bar_2[std::clamp(index + 1, 0, (isWerehog ? 4 : 1))], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                }
                else {
                    CSDCommon::PlayAnimation(*m_tag_bg_2[std::clamp(index - 1, 0, (isWerehog ? 4 : 1))], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                    CSDCommon::PlayAnimation(*m_prgs_bg_2[std::clamp(index - 1, 0, (isWerehog ? 4 : 1))], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                    CSDCommon::PlayAnimation(*m_prgs_num_2[std::clamp(index - 1, 0, (isWerehog ? 4 : 1))], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                    CSDCommon::PlayAnimation(*m_prgs_bar_2[std::clamp(index - 1, 0, (isWerehog ? 4 : 1))], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                }
            }

            CSDCommon::PlayAnimation(*m_tag_bg_2[index], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);

            CSDCommon::PlayAnimation(*m_prgs_bg_2[index], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);

            CSDCommon::PlayAnimation(*m_prgs_num_2[index], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);

            CSDCommon::PlayAnimation(*m_prgs_bar_2[index], select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
        }
    }

    void AddStat() {
        if ((isWerehog && currentStatIndex >= 5) || (!isWerehog && currentStatIndex >= 2)) { 
            return; 
        }

        Stat curStat = isWerehog ? statsNight[currentStatIndex] : statsDay[currentStatIndex];

        curStat.m_tag_bg_2 = m_rcStatus->CreateScene("tag_bg_2");
        curStat.m_tag_bg_2->SetHideFlag(false);
        curStat.m_tag_bg_2->SetPosition(0.0f, y);
        CSDCommon::PlayAnimation(*curStat.m_tag_bg_2, intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        m_tag_bg_2.push_back(curStat.m_tag_bg_2);

        const char* txtScene;

        switch (curStat.txt_num)
        {
        case 1:
            txtScene = "tag_txt_2";
            break;
        case 2:
            txtScene = "tag_txt_3";
            break;
        case 3:
            txtScene = "tag_txt_4";
            break;
        case 4:
            txtScene = "tag_txt_5";
            break;
        case 5:
            txtScene = "tag_txt_6";
            break;
        default:
            txtScene = "tag_txt_2";
            break;
        }

        curStat.m_tag_txt_2 = m_rcStatus->CreateScene(txtScene);
        curStat.m_tag_txt_2->SetHideFlag(false);
        CSDCommon::PlayAnimation(*curStat.m_tag_txt_2, (curStat.txt_num < 3) ? intro2() : "Intro_ev_Anim_2", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        m_tag_txt_2.push_back(curStat.m_tag_txt_2);

        curStat.m_prgs_bg_2 = m_rcStatus->CreateScene("prgs_bg_2");
        curStat.m_prgs_bg_2->SetHideFlag(false);
        curStat.m_prgs_bg_2->SetPosition(0.0f, y);
        CSDCommon::PlayAnimation(*curStat.m_prgs_bg_2, intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        m_prgs_bg_2.push_back(curStat.m_prgs_bg_2);

        curStat.m_prgs_num_2 = m_rcStatus->CreateScene("prgs_num_2");
        curStat.m_prgs_num_2->SetHideFlag(false);
        curStat.m_prgs_num_2->SetPosition(0.0f, y);

        if (curStat.level >= curStat.maxLevel) {
            curStat.m_prgs_num_2->GetNode("num")->SetHideFlag(1);
            curStat.m_prgs_num_2->GetNode("img")->SetHideFlag(1);
            curStat.m_prgs_num_2->GetNode("txt")->SetHideFlag(0);
        }
        else {
            curStat.m_prgs_num_2->GetNode("num")->SetText(std::to_string(curStat.level).c_str());
        }

        if (curStat.count == 0) {
            curStat.m_prgs_num_2->GetNode("num_2")->SetHideFlag(1);
            curStat.m_prgs_num_2->GetNode("img_2")->SetHideFlag(1);
        }
        else {
            curStat.m_prgs_num_2->GetNode("num_2")->SetText(std::to_string(curStat.count).c_str());
        }

        CSDCommon::PlayAnimation(*curStat.m_prgs_num_2, intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        m_prgs_num_2.push_back(curStat.m_prgs_num_2);

        curStat.m_prgs_bar_2 = m_rcStatus->CreateScene("prgs_bar_2");
        curStat.m_prgs_bar_2->SetHideFlag(false);
        curStat.m_prgs_bar_2->SetPosition(0.0f, y);
        CSDCommon::PlayAnimation(*curStat.m_prgs_bar_2, intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        m_prgs_bar_2.push_back(curStat.m_prgs_bar_2);

        y += (isWerehog ? 64.0f : 73.0f);

        statDelay = 0.17f;

        static SharedPtrTypeless soundHandle;
        Common::PlaySoundStatic(soundHandle, 1000020);
    }

    void Outro() {
        exit = true;
        return; // rest crashes 

        // Quit Button
        CSDCommon::PlayAnimation(*m_decide_bg, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);

        // Header
        CSDCommon::PlayAnimation(*m_status_title, outro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);

        // Character Portrait
        CSDCommon::PlayAnimation(*m_logo, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);

        // Bottom Buttons
        m_status_footer->SetHideFlag(true);

        //Medals (Top Right)
        CSDCommon::PlayAnimation(*m_medal_info, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);

        CSDCommon::PlayAnimation(*m_medal_s_gauge, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);

        CSDCommon::PlayAnimation(*m_medal_m_gauge, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);

        Stat curStat = isWerehog ? statsNight[currentStatIndex] : statsDay[currentStatIndex];

        for (size_t i = 0; i < m_tag_bg_2.size(); i++)
        {
            CSDCommon::PlayAnimation(*m_tag_bg_2[i], intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*m_tag_txt_2[i], (curStat.txt_num < 3) ? intro2() : "Intro_ev_Anim_2", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*m_prgs_bg_2[i], intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*m_prgs_num_2[i], intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*m_prgs_bar_2[i], intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
        }

        // Exp Bar
        CSDCommon::PlayAnimation(*m_tag_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);

        CSDCommon::PlayAnimation(*m_tag_txt_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);

        CSDCommon::PlayAnimation(*m_prgs_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);

        CSDCommon::PlayAnimation(*m_prgs_bar_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);

        CSDCommon::PlayAnimation(*m_prgs_num_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
    }

    void AddCallback
    (
        const Hedgehog::Base::THolder<Sonic::CWorld>& worldHolder,
        Sonic::CGameDocument* pGameDocument,
        const boost::shared_ptr<Hedgehog::Database::CDatabase>& spDatabase
    ) override
    {
        // const char* _name, int _level, int _maxLevel, int _txt_num, int _count
        statsDay = { Stat("speed", 9, 11, 1, 10), Stat("ring_energy", 6, 6, 2, 0) };

        statsNight = { Stat("combat", 0, 31, 1, 0), Stat("strength", 0, 11, 2, 0), Stat("life", 0, 11, 3, 0), Stat("unleash", 0, 11, 4, 0), Stat("shield", 0, 11, 5, 0) };

        isWerehog = false;

        Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
        pGameDocument->AddUpdateUnit("1", this);

        // initialize ui
        Sonic::CCsdDatabaseWrapper wrapper(m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

        auto spCsdProject = wrapper.GetCsdProject("ui_status");
        m_rcStatus = spCsdProject->m_rcProject;

        // Exp Bar
        m_tag_bg_1 = m_rcStatus->CreateScene("tag_bg_1");

        m_tag_txt_1 = m_rcStatus->CreateScene("tag_txt_1");

        m_prgs_bg_1 = m_rcStatus->CreateScene("prgs_bg_1");

        m_prgs_bar_1 = m_rcStatus->CreateScene("prgs_bar_1");

        m_prgs_num_1 = m_rcStatus->CreateScene("prgs_num_1");

        // Quit Button
        m_decide_bg = m_rcStatus->CreateScene("decide_bg");

        // Bottom Buttons
        m_status_footer = m_rcStatus->CreateScene("status_footer");

        // Header
        m_status_title = m_rcStatus->CreateScene("status_title");

        // Character Portrait
        m_logo = m_rcStatus->CreateScene("logo");

        //Medals (Top Right)
        m_medal_info = m_rcStatus->CreateScene("medal_info");

        m_medal_s_gauge = m_rcStatus->CreateScene("medal_s_gauge");

        m_medal_m_gauge = m_rcStatus->CreateScene("medal_m_gauge");

        Start();

        m_spStatus = boost::make_shared<Sonic::CGameObjectCSD>(m_rcStatus, 0.5f, "HUD_B2", false);
        Sonic::CGameDocument::GetInstance()->AddGameObject(m_spStatus, "main", this);
    }

    void Start() {
        m_tag_bg_1->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_tag_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_tag_txt_1->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_tag_txt_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_prgs_bg_1->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_prgs_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_prgs_bar_1->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_prgs_bar_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_prgs_num_1->SetHideFlag(false);
        m_prgs_num_1->GetNode("num")->SetText("69");
        CSDCommon::PlayAnimation(*m_prgs_num_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_decide_bg->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_decide_bg, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_status_footer->SetHideFlag(true);
        CSDCommon::PlayAnimation(*m_status_footer, "Usual_Anim_2", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_status_title->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_status_title, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_logo->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_logo, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_medal_info->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_medal_info, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_medal_s_gauge->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_medal_s_gauge, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_medal_m_gauge->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_medal_m_gauge, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        medalSubImage = 0.0f;

        timeFooter = 0.0f;
        footerVisible = false;

        statDelayStart = 0.75f;

        currentStatIndex = 0;
        y = 0.0f;

        quitSoundTimer = 0.0f;

        exit = false;
    }

    void UpdateParallel
    (
        const Hedgehog::Universe::SUpdateInfo& updateInfo
    ) override
    {
        Sonic::SPadState const* padState = &Sonic::CInputState::GetInstance()->GetPadState();

        if (!exit) {
            if (statDelayStart != -1) {
                statDelayStart -= updateInfo.DeltaTime;
                if (statDelayStart <= 0) {
                    AddStat();
                    selectDelay = 0.65f;
                    statDelayStart = -1;
                }
            }
            else {
                if (selectDelay != -1) {
                    if (selectDelay <= 0) {
                        Select(statusIndex, false, false);
                        selectDelay = -1;
                    }
                    else {
                        selectDelay -= updateInfo.DeltaTime;
                    }
                }
                statDelay -= updateInfo.DeltaTime;
                if (statDelay <= 0) {
                    currentStatIndex++;
                    AddStat();
                }
            }

            if (quitSoundTimer >= 1.06f) {
                static SharedPtrTypeless soundHandle;
                Common::PlaySoundStatic(soundHandle, 1000020);
                quitSoundTimer = -1;
            }
            else if(quitSoundTimer != -1) {
                quitSoundTimer += updateInfo.DeltaTime;
            }

            medalSubImage += updateInfo.DeltaTime * 25.0f;
            if (medalSubImage >= 29) {
                medalSubImage = 0.0f;
            }

            m_medal_info->GetNode("sun")->SetPatternIndex(medalSubImage);
            m_medal_info->GetNode("sun_shade")->SetPatternIndex(medalSubImage);

            m_medal_info->GetNode("moon")->SetPatternIndex(medalSubImage);
            m_medal_info->GetNode("moon_shade")->SetPatternIndex(medalSubImage);

            if (switchCooldown <= 0) {
                if (padState->IsTapped(Sonic::eKeyState_LeftBumper) || padState->IsTapped(Sonic::eKeyState_RightBumper)) {
                    ToggleStats();
                    switchCooldown = 0.5f;
                }
            }
            else {
                switchCooldown -= updateInfo.DeltaTime;
            }

            if (selectDelay == -1 && statDelayStart == -1) {
                if (padState->IsTapped(Sonic::eKeyState_LeftStickUp) || padState->IsTapped(Sonic::eKeyState_DpadUp)) {
                    if (statusIndex == (isWerehog ? 5 : 2)) {
                        statusIndex = (isWerehog ? 4 : 1);
                        Select(statusIndex, true, true);
                    }
                    else if(statusIndex != 0) {
                        if (statusIndex - 1 >= 0) {
                            statusIndex--;
                        }
                        Select(statusIndex, true, true);
                    }
                }
                if (padState->IsTapped(Sonic::eKeyState_LeftStickDown) || padState->IsTapped(Sonic::eKeyState_DpadDown)) {
                    if (statusIndex + 1 <= (isWerehog ? 4 : 1)) {
                        statusIndex++;
                        Select(statusIndex, false, true);
                    }
                    else if (statusIndex + 1 == (isWerehog ? 5 : 2)) {
                        statusIndex = (isWerehog ? 5 : 2);
                        Select(statusIndex, false, true, true);
                    }
                }
            }

            if (!footerVisible) {
                if (timeFooter >= 1.06f) {
                    m_status_footer->SetHideFlag(false);
                    footerVisible = true;
                }
                else { m_status_footer->SetHideFlag(true); timeFooter += updateInfo.DeltaTime; }
            }
        }
    }

    void Kill() {
        HudRemove();
        SendMessage(m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
    }

    bool IsQuit() {
        return statusIndex == (isWerehog ? 5 : 2);
    }
};

void HudStatus::Start() {
    status = boost::make_shared<CHudStatus>();
    Sonic::CGameDocument::GetInstance()->AddGameObject(status);
}

void HudStatus::End() {
    ((CHudStatus*)status.get())->Outro(); // Crashes, not sure why.
}

bool HudStatus::CanEnd() {
    return ((CHudStatus*)status.get())->IsQuit();
}

void HudStatus::Kill() {
    if (status) {
        ((CHudStatus*)status.get())->Kill();
    }
}

void HudStatus::Install() {

}