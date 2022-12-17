bool poleTrail = false;
bool poleSwingTrail = false;
SharedPtrTypeless swingHandle;
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

		if (Common::CheckPlayerNodeExist("Foot_L")) {
			if (anim == "PoleSpinLoop" && !poleSwingTrail) {
				std::string particleName = Common::IsPlayerIn2D() ? "pole_swing_trail_2d" : "pole_swing_trail";
				std::string particleSuperPrefix = "ss_";
				particleName = BlueBlurCommon::IsSuper() ? particleSuperPrefix.append(particleName) : particleName;
				boost::shared_ptr<Hedgehog::Mirage::CMatrixNodeSingleElementNode> FootNode = This->m_spCharacterModel->GetNode("Foot_L");
				Common::fCGlitterCreate(sonic, swingHandle, &FootNode, particleName.c_str(), 1);
				poleSwingTrail = true;
			}
			else if (anim != "PoleSpinLoop" && poleSwingTrail) {
				Common::fCGlitterEnd(sonic, swingHandle, true);
				poleSwingTrail = false;
			}
		}
	}
}
void Pole::Install() {
	INSTALL_HOOK(SonicPoleUpdate);
}