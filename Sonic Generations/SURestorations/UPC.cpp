float regularBoostEnableChaosEnergy = 1.0f;
const float disableBoostChaosEnergy = 9999.9999f;

float regularAirBoostEnableChaosEnergy = 1.0f;
const float disableAirBoostChaosEnergy = 9999.9999f;

bool toggleBoostEnabled = true;
void ToggleBoost(bool enabled) {
	toggleBoostEnabled = enabled;

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	float boostEnable = enabled ? regularBoostEnableChaosEnergy : disableBoostChaosEnergy;
	float airBoostEnable = enabled ? regularBoostEnableChaosEnergy : disableBoostChaosEnergy;

	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_BoostEnableChaosEnergy] = boostEnable;
	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AirBoostEnableChaosEnergy] = airBoostEnable;
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
			Common::fCGlitterCreate(sonic, paraloopHandle, middlematrixNode, (BlueBlurCommon::IsSuper() ? "ss_paraloop" : "paraloop"), 0);
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

float rotationFloatBobsleigh;

int XPressed = 0;
bool XResetTimerEnable = false;
float XResetTimer;
bool bobsleighTrick = false;
float bobsleighTrickTimer = 0.0f;
bool bobsleighRamp = false;
HOOK(int, __fastcall, ProcMsgRestartStageUPC, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	ToggleBoost(true);
	return originalProcMsgRestartStageUPC(This, Edx, message);
}
HOOK(void, __fastcall, SpecialJumpUPC, 0x11DE240, int This) {
	originalSpecialJumpUPC(This);
	if (BlueBlurCommon::IsModern()) {
		Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

		Hedgehog::Base::CSharedString state = sonic->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
		Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();

		if (Common::CheckCurrentStage("cpz200")) {
			float dist = (sonic->m_spMatrixNode->m_Transform.m_Position - Hedgehog::Math::CVector(71.332f, -108.133f, 606.234f)).norm();
			dist = abs(dist);
			if (dist <= 2.0f && anim != "Fall") {
				sonic->ChangeAnimation("Fall");
			}
		}
	}
}
HOOK(void, __fastcall, SonicAddonUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicAddonUpdate(This, _, updateInfo);
	if (BlueBlurCommon::IsModern()) {
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
		if (Common::CheckCurrentStage("cte200")) {
			if (toggleBoostEnabled) {
				regularAirBoostEnableChaosEnergy = sonic->m_spParameter->Get<float>(Sonic::Player::ePlayerSpeedParameter_AirBoostEnableChaosEnergy);
				regularBoostEnableChaosEnergy = sonic->m_spParameter->Get<float>(Sonic::Player::ePlayerSpeedParameter_BoostEnableChaosEnergy);
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
			if (strstr(state.c_str(), "Board")) {
				bobsleighRamp = false;
				if (!bobsleighBoostCancel) { bobsleighBoostCancel = true; }
				Common::SonicContextSetCollision(TypeSonicBoost, true);
				sonic->StateFlag(eStateFlag_EndBoost) = true;
				ToggleBoost(false);

				float desiredRotationFloatBobsleigh;

				if (sonic->m_Grounded) {
					desiredRotationFloatBobsleigh = 0.0f;
				}
				else {
					desiredRotationFloatBobsleigh = -Common::Lerp(-0.35f, 1.0f, Common::Clamp01(sonic->m_Velocity.y() * updateInfo.DeltaTime * 4.0f));
				}

				rotationFloatBobsleigh = Common::Lerp(rotationFloatBobsleigh, desiredRotationFloatBobsleigh, updateInfo.DeltaTime * 5.0f);
				Hedgehog::Math::CQuaternion m_Rotation = Hedgehog::Math::CQuaternion::Identity() * Eigen::AngleAxis<float>(rotationFloatBobsleigh, Eigen::Vector3f::UnitX());
				sonic->m_spModelMatrixNode->m_LocalMatrix.matrix() = (Eigen::Translation3f(Eigen::Vector3f::Zero()) * m_Rotation).matrix();
			}
			else if (bobsleighBoostCancel) {
				Common::SonicContextSetCollision(TypeSonicBoost, false);
				ToggleBoost(true);
				bobsleighBoostCancel = false;

				Hedgehog::Math::CQuaternion m_Rotation = Hedgehog::Math::CQuaternion::Identity() * Eigen::AngleAxis<float>(0.0f, Eigen::Vector3f::UnitX());
				sonic->m_spModelMatrixNode->m_LocalMatrix.matrix() = (Eigen::Translation3f(Eigen::Vector3f::Zero()) * m_Rotation).matrix();
			}
		}
	}
}
HOOK(void, __fastcall, RampUPC, 0x11DE240, int This) {
	if (BlueBlurCommon::IsModern() && Common::CheckCurrentStage("cte200")) {
		Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		if (Common::IsPlayerOnBoard()) {
			if (!bobsleighRamp) {
				sonic->ChangeAnimation("BoardJumpSpring");
				bobsleighRamp = true;
				bobsleighTrick = true;
				bobsleighTrickTimer = 0.5f;
				if (XPressed == 0) {
					XResetTimer = 0.0f;
					XResetTimerEnable = false;
				}
			}
		}
	}
	originalRampUPC(This);
}
void UPC::Install() {
	INSTALL_HOOK(RampUPC);

	// Credit to Skyth
	WRITE_MEMORY(0xDFF622, ::byte, 0xEB); // Disables Drifting when using bobsleigh

	// Speed Highway Rocket Explosion
	WRITE_MEMORY(0x164D90C, char, "fireworks");

	//brianuuu 06 Experience
	// Use idle animation on stage gates
	WRITE_MEMORY(0x127ADF1, uint32_t, 0x15E7670);
	WRITE_MEMORY(0x127AE58, uint32_t, 0x15E7670);
	WRITE_MEMORY(0x127AEAC, uint32_t, 0x15E7670);
	WRITE_MEMORY(0x127AF2A, uint32_t, 0x15E7670);
	WRITE_MEMORY(0x127AF91, uint32_t, 0x15E7670);
	WRITE_MEMORY(0x127AFF8, uint32_t, 0x15E7670);

	INSTALL_HOOK(SonicAddonUpdate);
	INSTALL_HOOK(SpecialJumpUPC);
	INSTALL_HOOK(ProcMsgRestartStageUPC);
}