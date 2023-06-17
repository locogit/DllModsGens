const float disableBoostChaosEnergy = 1000.0f;

const float disableAirBoostChaosEnergy = 1000.0f;

void ToggleBoost(bool enabled) {

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	if (!enabled) {
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_BoostEnableChaosEnergy] = disableBoostChaosEnergy;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AirBoostEnableChaosEnergy] = disableAirBoostChaosEnergy;
	}
	else {
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_BoostEnableChaosEnergy);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_AirBoostEnableChaosEnergy);
	}
}

bool bobsleighBoostCancel = false;

float paraloopTime;
float soundTime;
bool sound = false;
bool paraloop = false;

SharedPtrTypeless paraloopHandle;

struct ParaloopInfo {
	std::vector<float> startPos = { 0, 0, 0 };
	float distanceThreshold = 3.0f;
	float duration = 2.0f;
	float soundDelay = 2.0f;
	bool condition;
	void Update(Sonic::Player::CPlayerSpeedContext* sonic) {
		float dist = (sonic->m_spMatrixNode->m_Transform.m_Position - Hedgehog::Math::CVector(startPos[0], startPos[1], startPos[2])).norm();
		dist = abs(dist);
		if (dist <= distanceThreshold && condition && abs(sonic->m_Velocity.norm()) >= sonic->m_spParameter->Get<float>(Sonic::Player::ePlayerSpeedParameter_ParaloopMinSpeed)) {
			void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
			Common::fCGlitterCreate(sonic, paraloopHandle, middlematrixNode, (BlueBlurCommon::IsSuper() ? "ef_ch_sps_lms_paraloop02" : "ef_ch_sng_lms_paraloop02"), 0);
			paraloop = true;
			sound = true;
			paraloopTime = duration;
			soundTime = soundDelay;
		}
	}
};

void Paraloop(Sonic::Player::CPlayerSpeedContext* sonic) {

	if (paraloop) return;

	Hedgehog::Math::CVector pos = sonic->m_spMatrixNode->m_Transform.m_Position;

	// Mazuri
	if (Common::CheckCurrentStage("cpz200")) {
		{
			ParaloopInfo info;
			info.startPos = { 666.0f, -129.898f, 1364.824f };
			info.distanceThreshold = 1.5f;
			info.condition = true;
			info.duration = 2.0f;
			info.soundDelay = 1.0f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 697.287f, -92.5f, 1174.389f };
			info.distanceThreshold = 1.5f;
			info.condition = true;
			info.duration = 1.35f;
			info.soundDelay = 0.8f;
			info.Update(sonic);
		}

		// Paraloop (Rail)
		{
			ParaloopInfo info;
			info.startPos = { -5.615f, -53.5f, 449.415f };
			info.distanceThreshold = 1.5f;
			info.condition = true;
			info.duration = 1.5f;
			info.soundDelay = 1.0f;
			info.Update(sonic);
		}
	}
	else if (Common::CheckCurrentStage("euc200")) { // Abadat
		{
			ParaloopInfo info;
			info.startPos = { -1.464, 18.020f, -1502.584f };
			info.distanceThreshold = 1.5f;
			info.condition = true;
			info.duration = 1.9f;
			info.soundDelay = 0.0f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { -1.468f, 7.859f, -1517.009f };
			info.distanceThreshold = 1.5f;
			info.condition = true;
			info.duration = 1.6f;
			info.soundDelay = 0.0f;
			info.Update(sonic);
		}
	}
	else if (Common::CheckCurrentStage("ssh200")) { // Shamar
		{
			ParaloopInfo info;
			info.startPos = { -698.696f, 90.0f, 50.010f };
			info.distanceThreshold = 1.5f;
			info.condition = true;
			info.duration = 1.3f;
			info.soundDelay = 1.3f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 868.926f, 26.941f, 171.875f };
			info.distanceThreshold = 1.5f;
			info.condition = true;
			info.duration = 1.0f;
			info.soundDelay = 1.0f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { -22.0f, 63.0f, 5.37f };
			info.distanceThreshold = 2.0f;
			info.condition = true;
			info.duration = 1.65f;
			info.soundDelay = 1.65f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 1097.56f, -61.0f, 162.0f };
			info.distanceThreshold = 2.0f;
			info.condition = true;
			info.duration = 0.9f;
			info.soundDelay = 0.9f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 2162.285f, -85.735f, -326.38f };
			info.distanceThreshold = 2.5f;
			info.condition = true;
			info.duration = 1.35f;
			info.soundDelay = 1.35f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 2153.9f, -96.707f, -461.3f };
			info.distanceThreshold = 1.5f;
			info.condition = true;
			info.duration = 1.0f;
			info.soundDelay = 1.0f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 2222.497f, -172.588f, -1110.001f };
			info.distanceThreshold = 1.5f;
			info.condition = true;
			info.duration = 1.35f;
			info.soundDelay = 1.35f;
			info.Update(sonic);
		}
	}
	else if (Common::CheckCurrentStage("sph200")) { // Chun-Nan
		{
			ParaloopInfo info;
			info.startPos = { -508.074f, 46.021f, 206.315f };
			info.distanceThreshold = 1.5f;
			info.condition = true;
			info.duration = 1.2f;
			info.soundDelay = 1.2f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 561.526f, 20.047f, -1449.837f };
			info.distanceThreshold = 1.5f;
			info.condition = true;
			info.duration = 1.4f;
			info.soundDelay = 1.4f;
			info.Update(sonic);
		}

		{
			ParaloopInfo info;
			info.startPos = { 743.021f, -10.195f, -1490.299f };
			info.distanceThreshold = 1.5f;
			info.condition = true;
			info.duration = 1.5f;
			info.soundDelay = 1.5f;
			info.Update(sonic);
		}
	}
}

float rotationFloatBobsleigh = 0.0f;

int XPressed = 0;
bool XResetTimerEnable = false;
float XResetTimer;
bool bobsleighTrick = false;
float bobsleighTrickTimer = 0.0f;
bool bobsleighRamp = false;

HOOK(int, __fastcall, ProcMsgRestartStageUPC, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	ToggleBoost(true);

	WRITE_MEMORY(0x16D61BC, size_t, 0x111D950);
	WRITE_MEMORY(0x16D29A4, size_t, 0xE49B10);
	WRITE_MEMORY(0x16D1C38, size_t, 0xE49B10);
	WRITE_MEMORY(0x16D1C64, size_t, 0xE49B10);

	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_KeepRunningMinVelocityOnBoard);
	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_KeepRunningMaxVelocityOnBoard);

	return originalProcMsgRestartStageUPC(This, Edx, message);
}

HOOK(void, __fastcall, CitadelDashRing, 0x11DE240, int This) {
	originalCitadelDashRing(This);
	if (!BlueBlurCommon::IsModern()) { return; }
	if (!Common::CheckCurrentStage("cpz200")) { return; }

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	Hedgehog::Base::CSharedString state = sonic->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();

	float dist = (sonic->m_spMatrixNode->m_Transform.m_Position - Hedgehog::Math::CVector(71.332f, -108.133f, 606.234f)).norm();
	dist = abs(dist);
	if (dist <= 2.0f && anim != "Fall") {
		sonic->ChangeAnimation("Fall");
	}
}

float desiredRotationFloatBobsleigh = 0.0f;

bool bobsleighExplode = false;
float bobsleighExplodeTime = -1;
float bobsleighExplodeAfterTime = -1;

void BobsleighExplode() {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	bobsleighExplodeTime = -1;
	bobsleighExplode = true;
	bobsleighExplodeAfterTime = 1.0f;
	Eigen::Vector3f impulse;
	Eigen::Vector3f position = Eigen::Vector3f(-2163.594f, -340.120f, -1702.429f);
	float launchSpeed = 40.0f;
	float outOfControl = 0.0f;
	for (int i = 0; i < 10; i++)
	{
		if (Common::SolveBallisticArc
		(
			sonic->m_spMatrixNode->m_Transform.m_Position,
			position,
			launchSpeed,
			sonic->m_spParameter->Get<float>(Sonic::Player::ePlayerSpeedParameter_Gravity),
			false,
			impulse,
			outOfControl
		)
			)
		{
			// this makes a 0.5s not to accept MsgApplyImpulse if launched in air...? sub_E2BA00
			sonic->StateFlag(eStateFlag_NoLandOutOfControl) = 0;

			printf("[QTE] Launch velocity = {%.2f, %.2f, %.2f}, Speed = %.2f, OutOfControl = %.2fs\n", DEBUG_VECTOR3(impulse), launchSpeed, outOfControl);
			alignas(16) MsgApplyImpulse message {};
			message.m_position = sonic->m_spMatrixNode->m_Transform.m_Position;
			message.m_impulse = impulse;
			message.m_impulseType = ImpulseType::JumpBoard;
			message.m_outOfControl = outOfControl;
			message.m_notRelative = true;
			message.m_snapPosition = false;
			message.m_pathInterpolate = false;
			message.m_alwaysMinusOne = -1.0f;
			Common::ApplyPlayerApplyImpulse(message);
			break;
		}
		else
		{
			printf("[QTE] No solution for launch speed %.2f\n", launchSpeed);
			launchSpeed += 10.0f;
		}
	}
}

void CoolEdgeUpdate(const hh::fnd::SUpdateInfo& updateInfo) {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	Hedgehog::Base::CSharedString state = sonic->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_KeepRunningMinVelocityOnBoard] = 20.0f;
	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_KeepRunningMaxVelocityOnBoard] = 20.0f;

	float dist = (sonic->m_spMatrixNode->m_Transform.m_Position - Hedgehog::Math::CVector(-2095.450f, -352.846f, -1705.640f)).norm();
	dist = abs(dist);

	if (dist <= 2.0f && bobsleighExplodeTime == -1) {
		bobsleighExplodeTime = 0.45f;
		bobsleighExplodeAfterTime = -1;
	}

	if (bobsleighExplodeTime != -1) {
		bobsleighExplodeTime -= updateInfo.DeltaTime;
	}

	if (bobsleighExplodeTime <= 0 && bobsleighExplodeTime != -1) {
		BobsleighExplode();
	}

	if (state == "CPlayerSpeedStateBoardGetOff") {
		sonic->m_pPlayer->m_StateMachine.GetCurrentState()->LeaveState();
		sonic->ChangeState("Fall");
		sonic->ChangeAnimation("JumpSpring");
	}

	if (bobsleighExplodeAfterTime != -1) {
		bobsleighExplodeAfterTime -= updateInfo.DeltaTime;
	}

	if (bobsleighExplodeAfterTime <= 0 && bobsleighExplodeAfterTime != -1) {
		bobsleighExplode = false;
	}

	if (input.IsTapped(Sonic::eKeyState_X)) {
		if (!XResetTimerEnable) {
			XResetTimer = 0.2f;
			XResetTimerEnable = true;
		}
		XPressed++;
		if (XPressed == 2 && bobsleighTrick) {
			XPressed = 0;
			if (XResetTimer > 0) {
				sonic->ChangeAnimation("BoardSpecialJump");
				bobsleighTrick = false;
			}
		}
	}

	if (XResetTimerEnable) {
		if (XResetTimer > 0) XResetTimer -= updateInfo.DeltaTime;
		if (XResetTimer <= 0) {
			XPressed = 0;
			XResetTimerEnable = false;
		}
	}

	if (bobsleighTrick) {
		bobsleighTrickTimer -= updateInfo.DeltaTime;
		if (bobsleighTrickTimer <= 0) {
			bobsleighTrickTimer = 0;
			bobsleighTrick = false;
		}
	}

	if (Common::IsPlayerOnBoard()) {
		if (state != "SpecialJump") {
			bobsleighRamp = false;
		}

		if (!bobsleighBoostCancel) { bobsleighBoostCancel = true; }

		Common::SonicContextSetCollision(TypeSonicBoost, true);

		sonic->StateFlag(eStateFlag_EndBoost) = true;
		ToggleBoost(false);

		WRITE_MEMORY(0x16D61BC, size_t, 0x11BCAB0);
		WRITE_MEMORY(0x16D29A4, size_t, 0x11BCAB0);
		WRITE_MEMORY(0x16D1C38, size_t, 0x11BCAB0);
		WRITE_MEMORY(0x16D1C64, size_t, 0x11BCAB0);

		if (sonic->m_Grounded) {
			desiredRotationFloatBobsleigh = 0.0f;
		}
		else {
			desiredRotationFloatBobsleigh = -Common::Lerp(-0.35f, 0.7f, Common::Clamp01(sonic->m_Velocity.y() * updateInfo.DeltaTime * 3.0f));
		}

		rotationFloatBobsleigh = Common::Lerp(rotationFloatBobsleigh, desiredRotationFloatBobsleigh, updateInfo.DeltaTime * 5.0f);
		Hedgehog::Math::CQuaternion m_Rotation = Hedgehog::Math::CQuaternion::Identity() * Eigen::AngleAxis<float>(rotationFloatBobsleigh, Eigen::Vector3f::UnitX());
		sonic->m_spModelMatrixNode->m_LocalMatrix.matrix() = (Eigen::Translation3f(Eigen::Vector3f::Zero()) * m_Rotation).matrix();
	}
	else {

		WRITE_MEMORY(0x16D61BC, size_t, 0x111D950);
		WRITE_MEMORY(0x16D29A4, size_t, 0xE49B10);
		WRITE_MEMORY(0x16D1C38, size_t, 0xE49B10);
		WRITE_MEMORY(0x16D1C64, size_t, 0xE49B10);

		if (bobsleighBoostCancel) {
			Common::SonicContextSetCollision(TypeSonicBoost, false);
			ToggleBoost(true);
			bobsleighBoostCancel = false;
		}

		if (bobsleighExplode) {
			desiredRotationFloatBobsleigh = Common::Lerp(desiredRotationFloatBobsleigh, 1.5f, updateInfo.DeltaTime * 5.0f);
		}
		else {
			desiredRotationFloatBobsleigh = 0.0f;
		}

		rotationFloatBobsleigh = Common::Lerp(rotationFloatBobsleigh, desiredRotationFloatBobsleigh, updateInfo.DeltaTime * 5.0f);
		Hedgehog::Math::CQuaternion m_Rotation = Hedgehog::Math::CQuaternion::Identity() * Eigen::AngleAxis<float>(rotationFloatBobsleigh, Eigen::Vector3f::UnitX());
		sonic->m_spModelMatrixNode->m_LocalMatrix.matrix() = (Eigen::Translation3f(Eigen::Vector3f::Zero()) * m_Rotation).matrix();
	}
}

HOOK(void, __fastcall, SonicAddonUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicAddonUpdate(This, _, updateInfo);
	if (!BlueBlurCommon::IsModern()) { return; }
	Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
	Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();

	//Paraloop
	Paraloop(sonic);

	if (paraloopTime > 0 && paraloop) paraloopTime -= updateInfo.DeltaTime;

	if (paraloopTime <= 0 && paraloop) {
		Common::fCGlitterEnd(sonic, paraloopHandle, true);
		paraloop = false;
	}

	if (abs(sonic->m_Velocity.norm()) < sonic->m_spParameter->Get<float>(Sonic::Player::ePlayerSpeedParameter_ParaloopMinSpeed) && paraloop) {
		Common::fCGlitterEnd(sonic, paraloopHandle, true);
		paraloop = false;
		soundTime = 0.1f;
		sound = false;
	}

	if (soundTime > 0 && sound) soundTime -= updateInfo.DeltaTime;

	if (soundTime <= 0 && sound) {
		sonic->PlaySound(2002501, true);
		sound = false;
	}

	// Bobsleigh
	if (Common::CheckCurrentStage("cte200")) { CoolEdgeUpdate(updateInfo); }

	
}
HOOK(void, __fastcall, RampUPC, 0x11DE240, int This) {
	originalRampUPC(This);
	if (!BlueBlurCommon::IsModern() || !Common::CheckCurrentStage("cte200")) { return; }

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (Common::IsPlayerOnBoard() && !bobsleighRamp) {
		sonic->m_pPlayer->m_StateMachine.GetCurrentState()->LeaveState();
		sonic->ChangeState("BoardFall");
		sonic->ChangeAnimation("BoardFall");
		bobsleighRamp = true;
		bobsleighTrick = true;
		bobsleighTrickTimer = 0.5f;
		if (XPressed == 0) {
			XResetTimer = 0.0f;
			XResetTimerEnable = false;
		}
	}
}

HOOK(char, __fastcall, bobsleighBounce, 0x11BCA70, void* This, int a2) {
	if (Common::IsPlayerOnBoard() && BlueBlurCommon::IsModern() && Common::CheckCurrentStage("cte200")) {
		Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		sonic->m_Velocity.y() *= 0.3f;
		sonic->m_Velocity.x() *= 1.7f;
		sonic->m_Velocity.z() *= 1.7f;
	}
	return originalbobsleighBounce(This, a2);
}

void UPC::Install() {
	INSTALL_HOOK(RampUPC);
	INSTALL_HOOK(bobsleighBounce);
	// Credit to Skyth
	WRITE_MEMORY(0xDFF622, ::byte, 0xEB); // Disables Drifting when using bobsleigh

	// Speed Highway Rocket Explosion
	WRITE_MEMORY(0x164D90C, char, "ef_obj_chn_lms_firework");

	//brianuuu 06 Experience
	// Use idle animation on stage gates
	WRITE_MEMORY(0x127ADF1, uint32_t, 0x15E7670);
	WRITE_MEMORY(0x127AE58, uint32_t, 0x15E7670);
	WRITE_MEMORY(0x127AEAC, uint32_t, 0x15E7670);
	WRITE_MEMORY(0x127AF2A, uint32_t, 0x15E7670);
	WRITE_MEMORY(0x127AF91, uint32_t, 0x15E7670);
	WRITE_MEMORY(0x127AFF8, uint32_t, 0x15E7670);

	INSTALL_HOOK(SonicAddonUpdate);
	INSTALL_HOOK(CitadelDashRing);
	INSTALL_HOOK(ProcMsgRestartStageUPC);
}