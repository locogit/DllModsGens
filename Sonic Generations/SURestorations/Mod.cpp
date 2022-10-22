/// <summary>
/// The main entry point for the injection.
/// </summary>
extern "C" _declspec(dllexport) void Init()
{
	if (Common::reader.GetBoolean("Restorations", "Explosion", true)) {
		ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("Explosion", { "EnemyCommon" }));
	}
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicNew", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicCrawl", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicSweepKick", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicUI", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicRamp", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicEXP", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicSound", { "Sonic" }));
	ArchiveTreePatcher::applyPatches();
	AnimationSetPatcher::applyPatches();
	if(Common::SUHud && Common::UP) HubUI::Install();
	if(Common::SUHud && Common::reader.GetBoolean("EXP", "Use", true)) EXP::Install();
	CPlayerSpeedUpdate::Install();
	//Missile::Install();
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