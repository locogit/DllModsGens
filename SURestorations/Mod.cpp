/// <summary>
/// The main entry point for the injection.
/// </summary>
extern "C" _declspec(dllexport) void Init()
{
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