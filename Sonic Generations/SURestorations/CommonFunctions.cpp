HOOK(void, __fastcall, SonicUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicUpdate(This, _, updateInfo);
	Missile::OnUpdate(updateInfo);
	Misc::OnUpdate(updateInfo);
	if(Common::reader.GetBoolean("Restorations", "Stumble", true)) Stumble::OnUpdate(updateInfo);
	if(Common::reader.GetBoolean("Restorations", "SweepKick", true)) Sweepkick::OnUpdate(updateInfo);
	if(Common::reader.GetBoolean("Restorations", "UpReel", true)) UpReel::OnUpdate(updateInfo);
	if(Common::reader.GetBoolean("Restorations", "Crawl", true)) Crawl::OnUpdate(updateInfo);
	if(Common::reader.GetBoolean("Restorations", "PoleTrail", true)) Pole::OnUpdate(updateInfo);
	Ring::OnUpdate(updateInfo);
	if(Common::UPC) UPC::OnUpdate(updateInfo);
}

HOOK(void, __fastcall, HudSonicStageUpdate, 0x1098A50, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo) {
	if(Common::SUHud && Common::reader.GetBoolean("EXP", "Use", true)) EXP::OnHUDUpdate(in_rUpdateInfo);
	Misc::OnHUDUpdate(in_rUpdateInfo);
	Missile::OnHUDUpdate(in_rUpdateInfo);
	originalHudSonicStageUpdate(This, Edx, in_rUpdateInfo);
}

void CommonFunctions::Install() {
	INSTALL_HOOK(SonicUpdate);
	INSTALL_HOOK(HudSonicStageUpdate);
}