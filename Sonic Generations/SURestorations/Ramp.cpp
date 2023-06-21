SharedPtrTypeless RampHandle;
bool usedRamp;
bool rampParticle = false;
bool RampLoop = Common::reader.GetBoolean("Restorations", "Ramp", true);
bool RampBoost = Common::reader.GetBoolean("Restorations", "BoostRamp", true);

void Ramp::OnUpdate(const hh::fnd::SUpdateInfo& updateInfo) {
	if (!BlueBlurCommon::IsModern()) 
		return;

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Hedgehog::Base::CSharedString state = sonic->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();

	if (anim == "JumpBoard" && state == "SpecialJump" && !sonic->StateFlag(eStateFlag_Boost) && RampLoop)
		sonic->ChangeAnimation("JumpBoardLoop");

	if (strstr(sonic->GetCurrentAnimationName().c_str(), "JumpBoardSpecial") && state == "SpecialJump" && !usedRamp && RampBoost && !rampParticle) {
		usedRamp = true;
		rampParticle = false;
	}

	if (state != "SpecialJump" && RampBoost) {
		usedRamp = false;
		rampParticle = false;
	}

	if (sonic->StateFlag(eStateFlag_Boost) && usedRamp && !rampParticle) {
		usedRamp = false;
		rampParticle = true;
		void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
		Common::fCGlitterCreate(sonic, RampHandle, middlematrixNode, "ef_ch_sng_lms_jump_delux", 1);
		printf("[SU Restorations] Play Ramp Particle\n");
	}
}

void Ramp::Install() {
	
}