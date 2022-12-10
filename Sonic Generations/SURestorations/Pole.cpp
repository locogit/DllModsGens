bool poleTrail = false;
HOOK(void, __fastcall, SonicPoleUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicPoleUpdate(This, _, updateInfo);
	if (BlueBlurCommon::IsModern()) {
		Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
		Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();

		if (strstr(anim.c_str(), "PoleSpinJump") && !poleTrail) {
			poleTrail = true;
			Common::SonicContextRequestLocusEffect();
		}
		else if (!strstr(anim.c_str(), "PoleSpinJump") && poleTrail) {
			poleTrail = false;
		}
	}
}
void Pole::Install() {
	INSTALL_HOOK(SonicPoleUpdate);
}