int BPressed = 0;
bool BResetTimerEnable = false;
float BResetTimer;
float sweepkickColTime = 0.0f;
SharedPtrTypeless squatKickParticleHandle;
Hedgehog::Math::CQuaternion squatKickRotation;
HOOK(void, __fastcall, CSonicStateSquatKickBegin, 0x12526D0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	Sonic::Player::CPlayerSpeedContext* context = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;

	void* middlematrixNode = (void*)((uint32_t)context + 0x10);
	Common::fCGlitterCreate(context, squatKickParticleHandle, middlematrixNode, "Sweepkick", 1);

	context->PlaySound(2002497, true);

	sweepkickColTime = 0.2f;

	originalCSonicStateSquatKickBegin(This);
}
HOOK(void, __fastcall, CSonicStateSquatKickAdvance, 0x1252810, hh::fnd::CStateMachineBase::CStateBase* This) {
	auto* context = (Sonic::Player::CPlayerSpeedContext*)This->GetContextBase();
	Hedgehog::Math::CVector pos = context->m_spMatrixNode->m_Transform.m_Position;
	if (sweepkickColTime <= 0) Common::CreatePlayerSupportShockWave(pos, 0.5f, 2.5f, 0.1f);
	originalCSonicStateSquatKickAdvance(This);
}
HOOK(void, __fastcall, CSonicStateSquatKickEnd, 0x12527B0, void* This)
{
	Common::fCGlitterEnd(BlueBlurCommon::GetContext(), squatKickParticleHandle, true);
	originalCSonicStateSquatKickEnd(This);
}
// Original Code by Briannu
bool __fastcall CSonicStateSquatKickAdvanceTransitionOutImpl(char const* name)
{
	if (strcmp(name, "Stand") == 0)
	{
		Eigen::Vector3f inputDirection;
		if (Common::GetWorldInputDirection(inputDirection) && inputDirection.isZero())
		{
			BlueBlurCommon::GetContext()->ChangeState("Squat");
			BlueBlurCommon::GetContext()->ChangeAnimation("SquatToStand");
			return true;
		}
	}

	return false;
}

void __declspec(naked) CSonicStateSquatKickAdvanceTransitionOut()
{
	static uint32_t returnAddress = 0x1252924;
	static uint32_t sub_E4FF30 = 0xE4FF30;
	__asm
	{
		push    eax
		push    ecx

		mov     ecx, [eax]
		call    CSonicStateSquatKickAdvanceTransitionOutImpl
		mov     bl, al

		pop     ecx
		pop     eax

		test    bl, bl
		jnz     jump
		call[sub_E4FF30]

		jump:
		jmp[returnAddress]
	}
}
HOOK(void, __fastcall, SonicUpdateSweep, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicUpdateSweep(This, _, updateInfo);
	if (BlueBlurCommon::IsModern()) {
		if (sweepkickColTime > 0) sweepkickColTime -= updateInfo.DeltaTime;
		Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
		Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
		Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();
		if (input.IsTapped(Sonic::eKeyState_B)) {
			if (!BResetTimerEnable) {
				BResetTimer = 0.3f;
				BResetTimerEnable = true;
			}
			BPressed++;
			if (BPressed == 2) {
				BPressed = 0;
				if (BResetTimer > 0) {
					bool canSquatKick = (state == "Squat" || state == "Sliding") && !Common::IsPlayerControlLocked() && !BlueBlurCommon::IsSuper();
					if (canSquatKick) {
						squatKickRotation = sonic->m_spMatrixNode->m_Transform.m_Rotation;
						sonic->ChangeState("SquatKick");
					}
				}
			}
		}

		if (state == "SquatKick" && sonic->m_Velocity.norm() == 0.0f) {
			sonic->m_spMatrixNode->m_Transform.SetRotation(squatKickRotation);
		}

		if (BResetTimerEnable) {
			if (BResetTimer > 0) BResetTimer -= updateInfo.DeltaTime;
			if (BResetTimer <= 0) {
				BPressed = 0;
				BResetTimerEnable = false;
			}
		}
	}
}
void Sweepkick::Install() {
	//Original code by Brianuu
	static double const c_sweepKickActivateTime = 0.0f;
	WRITE_MEMORY(0x125299E, double*, &c_sweepKickActivateTime);

	// Change SquatKick's collision the same as sliding
	WRITE_MEMORY(0xDFCD6D, uint8_t, 0x5); // switch 6 cases
	static uint32_t const collisionSwitchTable[6] =
	{
		0xDFCD7B, // normal
		0xDFCDC0, // slide
		0xDFCD7B, // boost
		0xDFCD7B,
		0xDFCDFA, // unused
		0xDFCDC0  // squat kick
	};
	WRITE_MEMORY(0xDFCD77, uint32_t*, collisionSwitchTable);

	WRITE_JUMP(0x125291F, CSonicStateSquatKickAdvanceTransitionOut);

	INSTALL_HOOK(SonicUpdateSweep);
	INSTALL_HOOK(CSonicStateSquatKickBegin);
	INSTALL_HOOK(CSonicStateSquatKickAdvance);
	INSTALL_HOOK(CSonicStateSquatKickEnd);
}