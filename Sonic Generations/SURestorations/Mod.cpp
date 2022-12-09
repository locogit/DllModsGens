/// <summary>
/// The main entry point for the injection.
/// </summary>
extern "C" _declspec(dllexport) void Init()
{
	if(Common::reader.GetBoolean("Restorations", "Explosion", true)) ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("Explosion", { "EnemyCommon" }));

	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("EXPOrb", { "EnemyCommon" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("Pole", { "ActionCommon" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicCrawl", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicSweepKick", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicUI", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicRamp", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicEXP", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicSound", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SunMedal", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("MoonMedal", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicFirework", { "Sonic" }));

	ArchiveTreePatcher::applyPatches();
	AnimationSetPatcher::applyPatches();

	Misc::Install();

	if(Common::SUHud && Common::UPC) HubUI::Install();

	if(Common::SUHud && Common::reader.GetBoolean("EXP", "Use", true)) EXP::Install();

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