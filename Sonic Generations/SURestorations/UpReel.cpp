bool upReelEnd = false;

HOOK(void, __fastcall, SonicUpReelUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicUpReelUpdate(This, _, updateInfo);
	if (BlueBlurCommon::IsModern()) {
		Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
		Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
		Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();

		if (upReelEnd) {
			Eigen::Vector3f m_Position = sonic->m_spMatrixNode->m_Transform.m_Position;
			Eigen::Vector3f m_EndPosition = m_Position + (sonic->m_HorizontalRotation * Eigen::Vector3f::UnitZ() * 2.0f);
			Eigen::Vector4f const rayStartPos(m_Position.x(), m_Position.y(), m_Position.z(), 1.0f);
			Eigen::Vector4f const rayEndPos(m_EndPosition.x(), m_EndPosition.y(), m_EndPosition.z(), 1.0f);
			Eigen::Vector4f outPos;
			Eigen::Vector4f outNormal;
			if (!Common::fRaycast(rayStartPos, rayEndPos, outPos, outNormal, *(uint32_t*)0x1E0AFB4)) { // Raycast from briannu, check if can push forward
				upReelEnd = false;
				Hedgehog::Math::CVector ForwardVel = sonic->m_HorizontalRotation * Hedgehog::Math::CVector::UnitZ() * sonic->m_spParameter->Get<float>(Sonic::Player::ePlayerSpeedParameter_HangOnJumpHorzVel) * UpReel::upReelForceMultiplier;
				sonic->m_Velocity += ForwardVel;
			}
		}
	}
}

HOOK(void, __fastcall, HangEnd, 0xE46B20, int This) {
	originalHangEnd(This);
	upReelEnd = true;
}

void UpReel::Install() {
	//timer = new Timer(4.0f, [] {MessageBoxA(0, format("hooray! took %d ticks!", runTicks), "timer finished", MB_OK); runTicks = 0; }, [](float dt) {runTicks++; });
	UpReel::upReelForceMultiplier = 2.5f;
	INSTALL_HOOK(HangEnd);
	INSTALL_HOOK(SonicUpReelUpdate);
}