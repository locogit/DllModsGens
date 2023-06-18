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

HOOK(void, __cdecl, InitializeApplicationParams_Restorations, 0x00D65180, Sonic::CParameterFile* This)
{
	// Airboost
	{
		boost::shared_ptr<Sonic::CParameterGroup> parameterGroupAirBoost;
		This->CreateParameterGroup(parameterGroupAirBoost, "SU Restorations", "Parameters for SU Restorations");
		Sonic::CEditParam* airboost_param = parameterGroupAirBoost->CreateParameterCategory("Airboost", "Parameters for the airboost");

		airboost_param->CreateParamBool(&Misc::fadeOutAirBoost, "Fade Out Airboost");
		airboost_param->CreateParamFloat(&Misc::airBoostActiveTime, "Airboost Active Time");
		airboost_param->CreateParamFloat(&Misc::airBoostEndTime, "Airboost End Time");

		parameterGroupAirBoost->Flush();
	}

	// Sweepkick
	{
		boost::shared_ptr<Sonic::CParameterGroup> parameterGroupSweep;
		This->CreateParameterGroup(parameterGroupSweep, "SU Restorations", "Parameters for SU Restorations");
		Sonic::CEditParam* sweep_param = parameterGroupSweep->CreateParameterCategory("Sweepkick", "Parameters for the sweepkick");

		sweep_param->CreateParamFloat(&Sweepkick::sweepInputTime, "Sweepkick Input Time");
		sweep_param->CreateParamBool(&Sweepkick::useSupportShockwave, "Use Support Shockwave");
		sweep_param->CreateParamFloat(&Sweepkick::supportShockwaveDelay, "Support Shockwave Delay");
		sweep_param->CreateParamBool(&Sweepkick::transitionToStand, "Transition To Stand");

		parameterGroupSweep->Flush();
	}

	// Sweepkick Light
	{
		boost::shared_ptr<Sonic::CParameterGroup> parameterGroupSweepLight;
		This->CreateParameterGroup(parameterGroupSweepLight, "SU Restorations", "Parameters for SU Restorations");
		Sonic::CEditParam* sweepLight_param = parameterGroupSweepLight->CreateParameterCategory("Sweepkick Light", "Parameters for the sweepkick light");

		sweepLight_param->CreateParamBool(&Sweepkick::useLight, "Use Sweepkick Light");

		sweepLight_param->CreateParamFloat(&Sweepkick::offsetX, "Sweepkick Light Offset X");
		sweepLight_param->CreateParamFloat(&Sweepkick::offsetY, "Sweepkick Light Offset Y");
		sweepLight_param->CreateParamFloat(&Sweepkick::offsetZ, "Sweepkick Light Offset Z");

		sweepLight_param->CreateParamFloat(&Sweepkick::colorR, "Sweepkick Light Color R");
		sweepLight_param->CreateParamFloat(&Sweepkick::colorG, "Sweepkick Light Color G");
		sweepLight_param->CreateParamFloat(&Sweepkick::colorB, "Sweepkick Light Color B");
		sweepLight_param->CreateParamFloat(&Sweepkick::colorScalar, "Sweepkick Light Color Scalar");

		sweepLight_param->CreateParamFloat(&Sweepkick::colorRSS, "Sweepkick Light Color R Super");
		sweepLight_param->CreateParamFloat(&Sweepkick::colorGSS, "Sweepkick Light Color G Super");
		sweepLight_param->CreateParamFloat(&Sweepkick::colorBSS, "Sweepkick Light Color B Super");
		sweepLight_param->CreateParamFloat(&Sweepkick::colorScalarSS, "Sweepkick Light Color Scalar Super");

		sweepLight_param->CreateParamFloat(&Sweepkick::lightRange, "Sweepkick Light Range");
		sweepLight_param->CreateParamFloat(&Sweepkick::lightLifeTime, "Sweepkick Light Lifetime");
		sweepLight_param->CreateParamFloat(&Sweepkick::lightInDelay, "Sweepkick Light In Delay");
		sweepLight_param->CreateParamFloat(&Sweepkick::lerpSpeedIn, "Sweepkick Light Speed In");
		sweepLight_param->CreateParamFloat(&Sweepkick::lerpSpeedOut, "Sweepkick Light Speed Out");

		parameterGroupSweepLight->Flush();
	}

	// EXP
	{
		boost::shared_ptr<Sonic::CParameterGroup> parameterGroupEXP;
		This->CreateParameterGroup(parameterGroupEXP, "SU Restorations", "Parameters for SU Restorations");
		Sonic::CEditParam* exp_param = parameterGroupEXP->CreateParameterCategory("EXP", "Parameters for EXP");

		exp_param->CreateParamBool(&EXP::useStats, "Use EXP");
		exp_param->CreateParamBool(&EXP::maxStats, "Max EXP");

		parameterGroupEXP->Flush();
	}

	// UpReel
	{
		boost::shared_ptr<Sonic::CParameterGroup> parameterGroupUpReel;
		This->CreateParameterGroup(parameterGroupUpReel, "SU Restorations", "Parameters for SU Restorations");
		Sonic::CEditParam* pulley_param = parameterGroupUpReel->CreateParameterCategory("Pulley", "Parameters for pulleys");

		pulley_param->CreateParamFloat(&UpReel::upReelForceMultiplier, "Pulley Forward Force Multiplier");

		parameterGroupUpReel->Flush();
	}

	// Crawl
	{
		boost::shared_ptr<Sonic::CParameterGroup> parameterGroupCrawl;
		This->CreateParameterGroup(parameterGroupCrawl, "SU Restorations", "Parameters for SU Restorations");
		Sonic::CEditParam* crawl_param = parameterGroupCrawl->CreateParameterCategory("Crawl", "Parameters for crawling");

		crawl_param->CreateParamFloat(&Crawl::crawlSpeed, "Crawl Speed");
		crawl_param->CreateParamFloat(&Crawl::crawlTurnSpeed, "Crawl Turn Speed");
		crawl_param->CreateParamBool(&Crawl::crawlToSlide, "Crawl To Slide");
		crawl_param->CreateParamFloat(&Crawl::crawlSlipMult, "Crawl Slip Multiplier");

		parameterGroupCrawl->Flush();
	}

	// Ring Speed
	{
		boost::shared_ptr<Sonic::CParameterGroup> parameterGroupRing;
		This->CreateParameterGroup(parameterGroupRing, "SU Restorations", "Parameters for SU Restorations");
		Sonic::CEditParam* ring_param = parameterGroupRing->CreateParameterCategory("Ring Speed", "Parameters for ring speed");

		ring_param->CreateParamFloat(&Ring::ringLess100Add, "Ring <= 100");
		ring_param->CreateParamFloat(&Ring::ringGreat100Less200Add, " 200 > Ring > 100");
		ring_param->CreateParamFloat(&Ring::ringGreat200Add, "Ring >= 200");

		parameterGroupRing->Flush();
	}

	originalInitializeApplicationParams_Restorations(This);
}

extern "C" _declspec(dllexport) void Init()
{
	Common::LoadData();
	Common::SaveDataINI();

	//CreateConsole();
	//TestBuild();

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
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SWAShop", { "pam000" }));

	WRITE_STRING(0x15E90DC, "oneUpChanged");

	ArchiveTreePatcher::applyPatches();
	AnimationSetPatcher::applyPatches();

	LetterboxHelper::Initialize(1280, 720);

	Misc::Install();

	//if (Common::SUTitle || Common::UPC) Shop::Install();

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

	Save::Install();

	Ring::Install();

	FallCam::Install();

	INSTALL_HOOK(InitializeApplicationParams_Restorations);

	CommonFunctions::Install();
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

extern "C" void __declspec(dllexport) OnFrame()
{
	CSDCommon::update();
}