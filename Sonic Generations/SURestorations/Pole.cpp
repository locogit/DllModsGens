bool poleTrail = false;
bool poleSwingTrail = false;
SharedPtrTypeless swingHandle;

void PlayPoleParticle(Sonic::Player::CPlayerSpeed* This, Sonic::Player::CPlayerSpeedContext* sonic) {
	std::string name;

	if (Common::IsPlayerIn2D() && BlueBlurCommon::IsSuper()) {
		name = "ef_ch_sps_lms_bar_trace02";
	}
	else if (!Common::IsPlayerIn2D() && BlueBlurCommon::IsSuper()) {
		name = "ef_ch_sps_lms_bar_trace01";
	}

	if (Common::IsPlayerIn2D() && !BlueBlurCommon::IsSuper()) {
		name = "ef_ch_sng_lms_bar_trace02";
	}
	else if (!Common::IsPlayerIn2D() && !BlueBlurCommon::IsSuper()) {
		name = "ef_ch_sng_lms_bar_trace01";
	}

	boost::shared_ptr<Hedgehog::Mirage::CMatrixNodeSingleElementNode> FootNode = This->m_spCharacterModel->GetNode("Foot_L");
	Common::fCGlitterCreate(sonic, swingHandle, &FootNode, name.c_str(), 1);
	poleSwingTrail = true;
}

HOOK(void, __fastcall, SonicPoleUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicPoleUpdate(This, _, updateInfo);
	if (!BlueBlurCommon::IsModern()) { return; }

	Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();

	if (strstr(anim.c_str(), "PoleSpinJump") && !poleTrail) {
		poleTrail = true;
		Common::SonicContextRequestLocusEffect();
	}
	else if (!strstr(anim.c_str(), "PoleSpinJump") && poleTrail) {
		poleTrail = false;
	}

	if (anim == "PoleSpinLoop" && !poleSwingTrail && Common::CheckPlayerNodeExist("Foot_L")) {
		PlayPoleParticle(This, sonic);
	}
	else if (anim != "PoleSpinLoop" && poleSwingTrail && Common::CheckPlayerNodeExist("Foot_L")) {
		Common::fCGlitterEnd(sonic, swingHandle, true);
		poleSwingTrail = false;
	}
	
}
void Pole::Install() {
	INSTALL_HOOK(SonicPoleUpdate);
}