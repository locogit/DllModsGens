float speed = 70.0f;

float GetCorrectedSpeed() {
	float correctedSpeed = speed;

	if (Common::GetSonicStateFlags()->Boost) {
		correctedSpeed *= 1.3f;
	}

	if (Common::GetSonicStateFlags()->Squat) {
		correctedSpeed *= 0.1f;
	}

	if (Common::IsPlayerIn2D()) {
		correctedSpeed *= 0.5f;
	}

	if (Common::GetSonicStateFlags()->KeepRunning) {
		correctedSpeed *= 0.75f;
	}

	return correctedSpeed;
}

HOOK(void, __fastcall, SonicUpdateRing, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicUpdateRing(This, _, updateInfo);

	if (!BlueBlurCommon::IsModern()) { return; }

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	sonic->m_MaxVelocity = GetCorrectedSpeed();
}

void OnRing(hh::fnd::Message& msg) {
	if (!BlueBlurCommon::IsModern()) { return; }

	auto const* context = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (context->m_pPlayer->m_ActorID != msg.m_SenderActorID) { return; }

	int rings = context->m_RingCount;
	if (rings % 25 != 0 || rings <= 0) { return; }

	if (rings <= 100) {
		speed += 2.5f;
	}
	else if (rings < 200 && rings > 100) {
		speed += 1.0f;
	}
	else if (rings >= 200) {
		speed += 0.5f;
	}

	float* maxSpeed = Common::GetPlayerMaxSpeed();
	*maxSpeed = GetCorrectedSpeed();
}

HOOK(void, __fastcall, CObjRingProcMsgHitEventCollision, 0x10534B0, Sonic::CGameObject3D* This, void* Edx, hh::fnd::Message& in_rMsg)
{
	OnRing(in_rMsg);
	originalCObjRingProcMsgHitEventCollision(This, Edx, in_rMsg);
}

HOOK(void, __fastcall, CObjSuperRingProcMsgHitEventCollision, 0x11F2F10, Sonic::CGameObject3D* This, void* Edx, hh::fnd::Message& in_rMsg)
{
	OnRing(in_rMsg);
	originalCObjSuperRingProcMsgHitEventCollision(This, Edx, in_rMsg);
}

HOOK(void, __fastcall, Ring_SetMaxSpeedBasis, 0xDFBCA0, int* This)
{
	originalRing_SetMaxSpeedBasis(This);

	if (!BlueBlurCommon::IsModern()) { return; }

	if (*pModernSonicContext) {
		float* maxSpeed = Common::GetPlayerMaxSpeed();
		*maxSpeed = GetCorrectedSpeed();
	}
}

HOOK(int, __fastcall, Ring_MsgRestartStage, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	int result = originalRing_MsgRestartStage(This, Edx, message);

	if (BlueBlurCommon::IsModern()) {
		speed = 70.0f;
		float* maxSpeed = Common::GetPlayerMaxSpeed();
		*maxSpeed = GetCorrectedSpeed();
	}

	return result;
}

void Ring::Install() {
	Ring::ringLess100Add = 2.5f;
	Ring::ringGreat100Less200Add = 1.0f;
	Ring::ringGreat200Add = 0.5f;

	INSTALL_HOOK(Ring_SetMaxSpeedBasis); // even more stuff from brian love u bbg
	WRITE_NOP(0xDFE117, 1);
	INSTALL_HOOK(CObjRingProcMsgHitEventCollision);
	INSTALL_HOOK(CObjSuperRingProcMsgHitEventCollision);
	INSTALL_HOOK(Ring_MsgRestartStage);
}