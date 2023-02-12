int lastHurdleIndex = 0;
int currentHurdleIndex = 1;

bool ShortJumpMove() {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	float velXZ = sonic->m_Velocity.x() + sonic->m_Velocity.z();
	return  abs(velXZ) >= 5;
}

void playAnimBegin() {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (ShortJumpMove()) {
		currentHurdleIndex = (lastHurdleIndex == 0) ? 1 : 0;
		lastHurdleIndex = currentHurdleIndex;

		Hedgehog::Base::CSharedString hurdleAnimName = (currentHurdleIndex == 0) ? "JumpHurdleL" : "JumpHurdleR";
		sonic->ChangeAnimation(hurdleAnimName);
	}
	else {
		sonic->ChangeAnimation("JumpShortBegin");
	}
}

void playAnimTop() {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (!ShortJumpMove()) {
		sonic->ChangeAnimation("JumpShortTop");
	}
}

void __declspec(naked) ShortTop()
{
	static uint32_t shortTop = 0x011BF036;
	__asm
	{
		call playAnimTop
		jmp[shortTop]
	}
}

void __declspec(naked) ShortBegin()
{
	static uint32_t shortTop = 0x011BF26E;
	__asm
	{
		call playAnimBegin
		jmp[shortTop]	
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
	WRITE_JUMP(0x011BF008, ShortTop);
	WRITE_JUMP(0x011BF247, ShortBegin);
}