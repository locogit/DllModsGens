bool stumbleAir = false;

// Original Code by HyperBE32
HOOK(int, __fastcall, CSonicStateFallStart, 0x1118FB0, hh::fnd::CStateMachineBase::CStateBase* _this)
{
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
HOOK(void, __fastcall, SonicStumbleUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicStumbleUpdate(This, _, updateInfo);

	if (!BlueBlurCommon::IsModern()) { return; }

	Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
	if (state == "StumbleAir" && !stumbleAir) {
		stumbleAir = true;
	}
	else if (state != "StumbleAir" && stumbleAir) {
		stumbleAir = false;
	}
}
void Stumble::Install() {
	INSTALL_HOOK(CSonicStateFallStart);
	INSTALL_HOOK(SonicStumbleUpdate);
}