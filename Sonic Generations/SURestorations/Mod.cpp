/// <summary>
/// The main entry point for the injection.
/// </summary>
extern "C" _declspec(dllexport) void Init()
{
	if (Common::reader.GetBoolean("Restorations", "ExplosionParticle", true)) {
		ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("EnemyExplosion", { "EnemyCommon" }));
	}
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicNew", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicCrawl", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicSweepKick", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicUI", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicRamp", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicSound", { "Sonic" }));
	ArchiveTreePatcher::applyPatches();
	AnimationSetPatcher::applyPatches();
	HubUI::Install();
	EXP::Install();
	CPlayerSpeedUpdate::Install();
	//Missile::Install();
}

extern "C" __declspec(dllexport) void PostInit() {
	if (Common::TestModPriority("SU Restorations", "Sonic Unleashed HUD", true))
	{
		MessageBox(nullptr, TEXT("'Sonic Unleashed HUD' mod must be lower priority than 'SU Restorations'!"), TEXT("SU Restorations"), MB_ICONERROR);
		exit(-1);
	}
}