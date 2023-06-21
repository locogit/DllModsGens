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
	else
		sonic->ChangeAnimation("JumpShortBegin");
}

void playAnimTop() {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (!shortJumpMove)
		sonic->ChangeAnimation("JumpShortTop");
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

HOOK(int, __fastcall, HurdleJumpBegin, 0xDF1760, int a1, void* Edx) {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
	lastHurdleIndex = (anim == "JumpHurdleL") ? 1 : 0;
	return originalHurdleJumpBegin(a1, Edx);
}

void ShortJump::Install() {
	INSTALL_HOOK(HurdleJumpBegin);
	WRITE_JUMP(0x011BF008, ShortTop);
	WRITE_JUMP(0x011BF247, ShortBegin);
}