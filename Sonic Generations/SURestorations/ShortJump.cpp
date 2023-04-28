int lastHurdleIndex = 0;
int currentHurdleIndex = 1;

bool shortJumpMove = false;

void playAnimBegin() {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	shortJumpMove = sonic->m_Velocity.norm() >= 7.0f;
	if (shortJumpMove) {
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
	if (!shortJumpMove) {
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
	static uint32_t shortBegin = 0x011BF26E;
	__asm
	{
		call playAnimBegin
		jmp[shortBegin]	
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