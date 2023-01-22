bool UpReelEnd = false;
bool UpReelForce = false;
float UpReelTimer = 0.0f;

HOOK(void, __fastcall, SonicUpReelUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicUpReelUpdate(This, _, updateInfo);
	if (BlueBlurCommon::IsModern()) {
		Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
		Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
		Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
		if (anim == "UpReelEnd" && !UpReelEnd && !UpReelForce) {
			UpReelEnd = true;
			UpReelForce = true;
			UpReelTimer = 0.125f;
		}
		if (state != "SpecialJump" && anim != "UpReelEnd") {
			UpReelEnd = false;
		}
		if (anim != "UpReelEnd" && UpReelForce) {
			UpReelForce = false;
		}
		if (UpReelTimer > 0 && UpReelForce)
		{
			UpReelTimer -= updateInfo.DeltaTime;
		}
		if (UpReelTimer <= 0 && UpReelForce && UpReelEnd && state == "SpecialJump") {
			UpReelForce = false;
			Eigen::Vector3f playerPosition;
			Eigen::Quaternionf playerRotation;
			if (Common::GetPlayerTransform(playerPosition, playerRotation))
			{
				Hedgehog::Math::CVector ForwardVel = sonic->m_HorizontalRotation * Hedgehog::Math::CVector::UnitZ() * 5.5f;
				sonic->m_Velocity.x() = ForwardVel.x();
				sonic->m_Velocity.z() = ForwardVel.z();
			}
		}
	}
}
void UpReel::Install() {
	INSTALL_HOOK(SonicUpReelUpdate);
}