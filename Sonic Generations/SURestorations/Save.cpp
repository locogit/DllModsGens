HOOK(void, __fastcall, StartGoalSave, 0x10B58A0, uint32_t* This, void* Edx, void* message)
{
	EXP::Save(); 
	HubUI::Save();
	Save::SaveDataINI();
	printf("[SU Restorations] Data Saved!\n");
	originalStartGoalSave(This, Edx, message);
}

void Save::Install() {
	INSTALL_HOOK(StartGoalSave);
}