/// <summary>
/// The main entry point for the injection.
/// </summary>
extern "C" _declspec(dllexport) void Init()
{
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicNew", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicCrawl", { "Sonic" }));
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SonicPulley", { "Sonic" }));
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