bool shortJumpMove = false;
int lastHurdleIndex = 0;
int currentHurdleIndex = 1;

HOOK(int, __fastcall, ShortJumpMove, 0x11BF200, int This) {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	float velXZ = sonic->m_Velocity.x() + sonic->m_Velocity.z();
	shortJumpMove = abs(velXZ) >= 5;
	if (shortJumpMove) {
		WRITE_JUMP(0x11BF008, (void*)0x11BF03A);
		WRITE_JUMP(0x11BF247, (void*)0x11BF275);

		currentHurdleIndex = (lastHurdleIndex == 0) ? 1 : 0;
		lastHurdleIndex = currentHurdleIndex;

		Hedgehog::Base::CSharedString hurdleAnimName = (currentHurdleIndex == 0) ? "JumpHurdleL" : "JumpHurdleR";
		sonic->ChangeAnimation(hurdleAnimName);
	}
	else {
		// reverse the jumps
	}

	return originalShortJumpMove(This);
}

void ShortJump::Install() {
	INSTALL_HOOK(ShortJumpMove);
}