HOOK(void, __fastcall, SonicUpdateDebug, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicUpdateDebug(This, _, updateInfo);
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Hedgehog::Base::CSharedString state = sonic->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();
	Hedgehog::Math::CVector playerPos = sonic->m_spMatrixNode->m_Transform.m_Position;

	uint8_t stageID = Common::GetCurrentStageID() & 0xFF;
	static char* stageName = *(char**)(4 * stageID + 0x1E66B48);
	stageName[5] = '0' + ((Common::GetCurrentStageID() & 0xFF00) >> 8);

	DebugDrawText::log(format("Left Thumbstick: X: %.2f Y: %.2f\nRight Thumbstick: X: %.2f Y: %.2f", input.LeftStickHorizontal, input.LeftStickVertical, input.RightStickHorizontal, input.RightStickVertical));
	DebugDrawText::log(format("State : %s\nAnimation : %s", state, anim));
	DebugDrawText::log(format("OutOfControl : %d", sonic->StateFlag(eStateFlag_OutOfControl)));
	DebugDrawText::log(format("Stage : %s", stageName));
	DebugDrawText::log(format("Max Speed : %f", *Common::GetPlayerMaxSpeed()));
	DebugDrawText::log(format("Velocity Norm : %f", sonic->m_Velocity.norm()));

	if (BlueBlurCommon::IsModern()) {
		DebugDrawText::log(format("Ring Energy : %.2f%%", sonic->m_ChaosEnergy / sonic->GetMaxChaosEnergy() * 100.0f));
	}
}

void Update::Install() {
	INSTALL_HOOK(SonicUpdateDebug);
}