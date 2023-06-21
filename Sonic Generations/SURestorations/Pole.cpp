bool poleTrail = false;
bool poleSwingTrail = false;
SharedPtrTypeless swingHandle;

void PlayPoleParticle(Sonic::Player::CPlayerSpeedContext* sonic) {
	std::string name;

	if (Common::IsPlayerIn2D() && BlueBlurCommon::IsSuper())
		name = "ef_ch_sps_lms_bar_trace02";
	else if (!Common::IsPlayerIn2D() && BlueBlurCommon::IsSuper())
		name = "ef_ch_sps_lms_bar_trace01";

	if (Common::IsPlayerIn2D() && !BlueBlurCommon::IsSuper())
		name = "ef_ch_sng_lms_bar_trace02";
	else if (!Common::IsPlayerIn2D() && !BlueBlurCommon::IsSuper())
		name = "ef_ch_sng_lms_bar_trace01";

	boost::shared_ptr<Hedgehog::Mirage::CMatrixNodeSingleElementNode> FootNode = sonic->m_pPlayer->m_spCharacterModel->GetNode("Foot_L");
	Common::fCGlitterCreate(sonic, swingHandle, &FootNode, name.c_str(), 1);
	poleSwingTrail = true;
}

void Pole::OnUpdate(const hh::fnd::SUpdateInfo& updateInfo) {
	if (!BlueBlurCommon::IsModern()) { return; }

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();

	if (strstr(anim.c_str(), "PoleSpinJump") && !poleTrail) {
		poleTrail = true;
		Common::SonicContextRequestLocusEffect();
	}
	else if (!strstr(anim.c_str(), "PoleSpinJump") && poleTrail)
		poleTrail = false;

	if (anim == "PoleSpinLoop" && !poleSwingTrail && Common::CheckPlayerNodeExist("Foot_L"))
		PlayPoleParticle(sonic);
	else if (anim != "PoleSpinLoop" && poleSwingTrail && Common::CheckPlayerNodeExist("Foot_L")) {
		Common::fCGlitterEnd(sonic, swingHandle, true);
		poleSwingTrail = false;
	}
	
}
void Pole::Install() {

}