/// <summary>
/// The main entry point for the injection.
/// </summary>

void CreateConsole()
{
	if (!AllocConsole()) {
		// Add some error handling here.
		// You can call GetLastError() to get more info about the error.
		return;
	}
	// std::cout, std::clog, std::cerr, std::cin
	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
	std::cin.clear();
	// std::wcout, std::wclog, std::wcerr, std::wcin
	HANDLE hConOut = CreateFile(_T("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hConIn = CreateFile(_T("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
	SetStdHandle(STD_ERROR_HANDLE, hConOut);
	SetStdHandle(STD_INPUT_HANDLE, hConIn);
	std::wcout.clear();
	std::wclog.clear();
	std::wcerr.clear();
	std::wcin.clear();
}

void TestBuild() {
	MessageBox(nullptr, "This is a test build, please give feedback or bug reports if you encounter any. Thanks!", "SU Restorations", MB_ICONINFORMATION);
}

extern "C" _declspec(dllexport) void Init()
{
	//CreateConsole();
	//TestBuild();

	CSDCommon::Initialize();

	if(Common::reader.GetBoolean("Restorations", "Explosion", true)) ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("Explosion", { "EnemyCommon" }));
	
	if (Common::SUHud && Common::reader.GetBoolean("EXP", "Use", true)) { 
		ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("EXPOrb", { "EnemyCommon" }));
		EXP::Install(); 
	}

	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("Pole", { "ActionCommon" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicCrawl", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicSweepKick", { "SonicActionCommon" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicUI", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicRamp", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicEXP", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicSound", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SunMedal", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("MoonMedal", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicFirework", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicParaloop", { "SonicActionCommon" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicLife", { "Sonic" }));

	WRITE_STRING(0x15E90DC, "oneUpChanged");

	ArchiveTreePatcher::applyPatches();
	AnimationSetPatcher::applyPatches();

	Misc::Install();

	if(Common::SUHud && Common::UPC) HubUI::Install();

	if(Common::reader.GetBoolean("Restorations", "SweepKick", true)) Sweepkick::Install();

	if (Common::reader.GetBoolean("Restorations", "UpReel", true)) UpReel::Install();

	if(Common::reader.GetBoolean("Restorations", "Stumble", true)) Stumble::Install();

	if(Common::reader.GetBoolean("Restorations", "RunJump", true)) ShortJump::Install();

	if(Common::reader.GetBoolean("Restorations", "Crawl", true)) Crawl::Install();

	if(Common::reader.GetBoolean("Restorations", "PoleTrail", true)) Pole::Install();

	if(Common::reader.GetBoolean("Restorations", "Ramp", true) || Common::reader.GetBoolean("Restorations", "BoostRamp", true)) Ramp::Install();

	if(Common::UPC) UPC::Install();

	//if (Common::SUHud) Medal::Install();

	Missile::Install();
}

extern "C" __declspec(dllexport) void PostInit() {
	if (Common::IsModEnabled("Unleashed Foot Sweep Restored") && Common::reader.GetBoolean("Restorations", "SweepKick", true)) {
		MessageBox(nullptr, "'Unleashed Foot Sweep Restored' should be disabled while using this mod.", "SU Restorations", MB_ICONERROR);
		exit(-1);
	}
	if (Common::IsModEnabled("Flail Falling") && Common::reader.GetBoolean("Changes", "UnwiishedFall", false)) {
		MessageBox(nullptr, "'Flail Falling' should be disabled while using this mod.", "SU Restorations", MB_ICONERROR);
		exit(-1);
	}
	if (Common::TestModPriority("SU Restorations", "Sonic Unleashed HUD", true))
	{
		MessageBox(nullptr, TEXT("'Sonic Unleashed HUD' mod must be lower priority than 'SU Restorations'!"), TEXT("SU Restorations"), MB_ICONERROR);
		exit(-1);
	}
}