
bool isScoreGenLowerPriority = false;
extern "C" __declspec(dllexport) void Init(const char* path)
{
	if (GetModuleHandle(TEXT("ChipReturns.dll")))
	{
		MessageBox(nullptr, TEXT("'Chip Returns' mod must be higher priority than 'Sonic Unleashed HUD'!"), TEXT("Sonic Unleashed HUD"), MB_ICONERROR);
		exit(-1);
	}

	isScoreGenLowerPriority = (GetModuleHandle(TEXT("ScoreGenerations.dll")) != nullptr || GetModuleHandle(TEXT("STH2006ProjectExtra.dll")) != nullptr);

	Configuration::Read();

	ArchiveTreePatcher::Install();
	HudSonicStage::Install();
	HudResult::Install(); // Must be hooked after HudSonicStage to get checkpoint speed data
	HudStatus::Install();
	HudLoading::Install();
	HudPause::Install();
	Patches::Install();
}

extern "C" __declspec(dllexport) void PostInit()
{
	if (!isScoreGenLowerPriority && (GetModuleHandle(TEXT("ScoreGenerations.dll")) != nullptr || GetModuleHandle(TEXT("STH2006ProjectExtra.dll")) != nullptr))
	{
		MessageBox(nullptr, TEXT("'Score Generations' mod must be lower priority than 'Sonic Unleashed HUD'!"), TEXT("Sonic Unleashed HUD"), MB_ICONERROR);
		exit(-1);
	}

	if (GetModuleHandle(TEXT("ERankGenerations.dll")))
	{
		MessageBox(nullptr, TEXT("'E-Rank Generations' mod is not compatible with 'Sonic Unleashed HUD'! Please disable it."), TEXT("Sonic Unleashed HUD"), MB_ICONERROR);
		exit(-1);
	}

	if (Configuration::unleashedResultMusic && GetModuleHandle(TEXT("CustomizableResultsMusic.dll")))
	{
		MessageBox(nullptr, TEXT("'Customizable Results Music' mod is not compatible with 'Sonic Unleashed HUD' when Unleashed Music is enabled! Either disable Unleashed Music in configuration or the mod"), TEXT("Sonic Unleashed HUD"), MB_ICONERROR);
		exit(-1);
	}
}

extern "C" void __declspec(dllexport) OnFrame()
{
	CSDCommon::update();
}