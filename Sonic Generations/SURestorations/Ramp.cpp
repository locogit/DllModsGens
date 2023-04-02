SharedPtrTypeless RampHandle;
bool usedRamp;

bool RampLoop = Common::reader.GetBoolean("Restorations", "Ramp", true);
bool RampBoost = Common::reader.GetBoolean("Restorations", "BoostRamp", true);

HOOK(void, __fastcall, SonicRampUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicRampUpdate(This, _, updateInfo);
	if (BlueBlurCommon::IsModern()) {
		Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
		Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
		Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();

		if (RampLoop) {
			if (anim == "JumpBoard" && state == "SpecialJump" && !sonic->StateFlag(eStateFlag_Boost)) {
				sonic->ChangeAnimation("JumpBoardLoop");
			}
		}
		if (RampBoost) {
			if (strstr(sonic->GetCurrentAnimationName().c_str(), "JumpBoardSpecial") && state == "SpecialJump" && !usedRamp) {
				usedRamp = true;
				if (sonic->StateFlag(eStateFlag_Boost)) {
					usedRamp = true;
					void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
					Common::fCGlitterCreate(sonic, RampHandle, middlematrixNode, "jump_delux", 1);
					printf("[SU Restorations] Play Ramp Particle\n");
				}
			}
			if (state != "SpecialJump")
				usedRamp = false;
		}
	}
}

void Ramp::Install() {
	INSTALL_HOOK(SonicRampUpdate);
}