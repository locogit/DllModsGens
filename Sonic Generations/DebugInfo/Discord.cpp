DiscordRichPresence discordPresence;

bool m_isEvent = false;
HOOK(void, __fastcall, HudLoading_CHudLoadingCStateIntroBegin, 0x10938F0, hh::fnd::CStateMachineBase::CStateBase* This)
{
    char const* eventName = *(char**)Common::GetMultiLevelAddress(0x1E66B34, { 0x4, 0x1B4, 0x80, 0x2C });
    m_isEvent = !std::string(eventName).empty();

    if (!m_isEvent) {
        Discord::ChangeInformationFromStageInfo();
    }

    originalHudLoading_CHudLoadingCStateIntroBegin(This);
}

void Discord::Initialize() {
    INSTALL_HOOK(HudLoading_CHudLoadingCStateIntroBegin);

	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(Handle));
	Discord_Initialize(CLIENT_ID, NULL, 1, NULL);

    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.startTimestamp = time(0);
}

void Discord::Update() {
    discordPresence.details = TopText.c_str();
    discordPresence.state = BottomText.c_str();

    discordPresence.largeImageKey = Thumbnail.c_str();
    //discordPresence.largeImageText = Country;

    discordPresence.partySize = partySize;
    discordPresence.partyMax = partyMax;

    discordPresence.smallImageKey = ThumbnailSmall.c_str();
    discordPresence.smallImageText = StageTypeText.c_str();

    Discord_UpdatePresence(&discordPresence);
}