bool shortJumpMove = false;
int lastHurdleIndex = 0;
int currentHurdleIndex = 1;
HOOK(int, __fastcall, ShortJumpMove, 0x11BF200, int This) {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	float velXZ = sonic->m_Velocity.x() + sonic->m_Velocity.z();
	shortJumpMove = abs(velXZ) >= 5;
	return originalShortJumpMove(This);
}
HOOK(void, __fastcall, HurdleAnim, 0x11BEEC0, float* This) {
	originalHurdleAnim(This);
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
	if (shortJumpMove) {
		if (anim == "JumpShortBegin") {
			currentHurdleIndex = (lastHurdleIndex == 0) ? 1 : 0;
			lastHurdleIndex = currentHurdleIndex;
		}
		if (anim != "JumpHurdleL" && anim != "JumpHurdleR") {
			if (anim != "SpinAttack") {
				Hedgehog::Base::CSharedString hurdleAnimName = (currentHurdleIndex == 0) ? "JumpHurdleL" : "JumpHurdleR";
				sonic->ChangeAnimation(hurdleAnimName);
			}
		}
	}
}
HOOK(void, __fastcall, SonicShortJumpUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicShortJumpUpdate(This, _, updateInfo);
	if (BlueBlurCommon::IsModern()) {
		Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
		Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
		Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
		if (state == "JumpHurdle")
			lastHurdleIndex = (anim == "JumpHurdleL") ? 1 : 0;
	}
}
void ShortJump::Install() {
	INSTALL_HOOK(SonicShortJumpUpdate);
	INSTALL_HOOK(ShortJumpMove);
	INSTALL_HOOK(HurdleAnim);
}