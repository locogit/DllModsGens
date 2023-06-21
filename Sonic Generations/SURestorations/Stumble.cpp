bool stumbleAir = false;

// Original Code by HyperBE32
HOOK(int, __fastcall, CSonicStateFallStart, 0x1118FB0, hh::fnd::CStateMachineBase::CStateBase* _this)
{
	if (!BlueBlurCommon::IsModern()) { 
		WRITE_MEMORY(0x1118DE5, uint8_t, 0x75);
		WRITE_MEMORY(0x1118E94, uint8_t, 0x76);
		WRITE_MEMORY(0x111910F, uint8_t, 0x75);
		return originalCSonicStateFallStart(_this); 
	}
	auto context = (Sonic::Player::CPlayerSpeedContext*)_this->GetContextBase();

	if (stumbleAir)
	{
		// Don't transition animation.
		WRITE_MEMORY(0x1118DE5, uint8_t, 0xEB);
		WRITE_MEMORY(0x1118E94, uint8_t, 0xEB);
		WRITE_MEMORY(0x111910F, uint8_t, 0xEB);
	}
	else
	{
		WRITE_MEMORY(0x1118DE5, uint8_t, 0x75);
		WRITE_MEMORY(0x1118E94, uint8_t, 0x76);
		WRITE_MEMORY(0x111910F, uint8_t, 0x75);
	}

	return originalCSonicStateFallStart(_this);
}

void Stumble::OnUpdate(const hh::fnd::SUpdateInfo& updateInfo) {
	if (!BlueBlurCommon::IsModern())
		return;

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Hedgehog::Base::CSharedString state = sonic->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();

	if (state == "StumbleAir" && !stumbleAir)
		stumbleAir = true;
	else if (state != "StumbleAir" && stumbleAir)
		stumbleAir = false;
}
void Stumble::Install() {
	INSTALL_HOOK(CSonicStateFallStart);
}