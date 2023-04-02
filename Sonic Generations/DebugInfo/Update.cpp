class CObjTest : public Sonic::CGameObject
{
	boost::shared_ptr<hh::mr::CSingleElement> m_spModel;
	Sonic::Player::CPlayerSpeedContext* context;
public:
	CObjTest()
	{
	}
	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& worldHolder, Sonic::CGameDocument* pGameDocument,
		const boost::shared_ptr<Hedgehog::Database::CDatabase>& spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		pGameDocument->AddUpdateUnit("f", this);

		m_spModel = boost::make_shared<hh::mr::CSingleElement>(hh::mr::CMirageDatabaseWrapper(spDatabase.get()).GetModelData("chr_Sonic_HD"));
		AddRenderable("Object", m_spModel, false);

		context = Sonic::Player::CPlayerSpeedContext::GetInstance();
	}

	void UpdateParallel(const Hedgehog::Universe::SUpdateInfo& updateInfo) override
	{
		UpdateTransform(updateInfo);
	}

	bool ProcessMessage(Hedgehog::Universe::Message& message, bool flag) override
	{
		if (flag)
		{
			if (std::strstr(message.GetType(), "MsgRestartStage") != nullptr
				|| std::strstr(message.GetType(), "MsgStageClear") != nullptr)
			{
				Kill();
				return true;
			}
		}

		return Sonic::CGameObject::ProcessMessage(message, flag);
	}

	void Kill()
	{
		SendMessage(m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
	}

	void UpdateTransform(const Hedgehog::Universe::SUpdateInfo& updateInfo) {
		Eigen::Vector3f pos = Eigen::Vector3f(0, 0, 0);

		float dist = (context->m_spMatrixNode->m_Transform.m_Position - pos).norm();

		if (dist <= 0.5f) {
			context->ChangeState(context->m_Grounded ? "NormalDamage" : "NormalDamageAir");
		}

		auto matrix = (Eigen::Translation3f(pos) * Eigen::Quaternionf::Identity()).matrix();

		Hedgehog::Base::CSharedString state = context->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();

		m_spModel->m_spInstanceInfo->m_Transform = matrix;
	}
};

boost::shared_ptr<Sonic::CGameObject> m_testSingleton;

float speed = 70.0f;
const char* _state;
bool isBoost;
bool is2D;
bool isSquat;

HOOK(void, __fastcall, SonicUpdateDebug, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalSonicUpdateDebug(This, _, updateInfo);
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Hedgehog::Base::CSharedString state = sonic->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
	Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
	Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();
	Hedgehog::Math::CVector playerPos = sonic->m_spMatrixNode->m_Transform.m_Position;
	DebugDrawText::log(format("Left Thumbstick: X: %.2f Y: %.2f\nRight Thumbstick: X: %.2f Y: %.2f", input.LeftStickHorizontal, input.LeftStickVertical, input.RightStickHorizontal, input.RightStickVertical));
	DebugDrawText::log(format("State : %s\nAnimation : %s", state, anim));
	DebugDrawText::log(format("OutOfControl : %d", sonic->StateFlag(eStateFlag_OutOfControl)));
	DebugDrawText::log(format("Stage : %s", (char*)0x01E774D4));
	DebugDrawText::log(format("Ring Energy : %.2f%%", sonic->m_ChaosEnergy/sonic->GetMaxChaosEnergy() * 100.0f));
	DebugDrawText::log(format("Max Speed : %f", *Common::GetPlayerMaxSpeed()));
	DebugDrawText::log(format("Velocity Norm : %f", sonic->m_Velocity.norm()));

	_state = state.c_str();
	isBoost = sonic->StateFlag(eStateFlag_Boost) || _state == "AirBoost";
	isSquat = _state == "Squat";
	is2D = sonic->m_Is2DMode;

	if (input.IsTapped(Sonic::eKeyState_RightTrigger)) {
		if (m_testSingleton) {
			((CObjTest*)m_testSingleton.get())->Kill();
		}
		m_testSingleton = boost::make_shared<CObjTest>();
		sonic->m_pPlayer->m_pMember->m_pGameDocument->AddGameObject(m_testSingleton);
	}
}

float GetCorrectedSpeed() {
	if (*pModernSonicContext)
	{
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
			correctedSpeed *= 0.7f;
		}

		return correctedSpeed;
	}

	return 0.0f;
}

void OnRing(hh::fnd::Message& msg) {
	if (BlueBlurCommon::IsModern()) {
		auto const* context = Sonic::Player::CPlayerSpeedContext::GetInstance();
		if (context->m_pPlayer->m_ActorID == msg.m_SenderActorID)
		{
			int rings = context->m_RingCount;
			if (rings % 25 == 0 && rings > 0) {
				if (rings < 100) {
					speed += 2.5f;
				}
				else if (rings < 200 && rings > 100) {
					speed += 1.0f;
				}
				else if (rings > 200) {
					speed += 0.5f;
				}

				float* maxSpeed = Common::GetPlayerMaxSpeed();
				*maxSpeed = GetCorrectedSpeed();
			}
		}
	}
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

HOOK(void, __fastcall, CSonicSetMaxSpeedBasis, 0xDFBCA0, int* This)
{
	originalCSonicSetMaxSpeedBasis(This);

	if (*pModernSonicContext)
	{
		float* maxSpeed = Common::GetPlayerMaxSpeed();

		*maxSpeed = GetCorrectedSpeed();
	}
}

HOOK(int, __fastcall, Sonic_MsgRestartStage, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	int result = originalSonic_MsgRestartStage(This, Edx, message);

	speed = 70.0f;

	float* maxSpeed = Common::GetPlayerMaxSpeed();
	*maxSpeed = GetCorrectedSpeed();

	return result;
}

void Update::Install() {
	INSTALL_HOOK(CSonicSetMaxSpeedBasis);
	WRITE_NOP(0xDFE117, 1);
	INSTALL_HOOK(CObjRingProcMsgHitEventCollision);
	INSTALL_HOOK(CObjSuperRingProcMsgHitEventCollision);
	INSTALL_HOOK(Sonic_MsgRestartStage);

	INSTALL_HOOK(SonicUpdateDebug);
}