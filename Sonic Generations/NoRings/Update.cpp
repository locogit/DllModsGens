bool isDead = false;

template<typename C, typename T>
bool contains(C&& c, T e) {
	return std::find(std::begin(c), std::end(c), e) != std::end(c);
};

HOOK(void, __fastcall, CPlayerSpeedUpdateParallel, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	originalCPlayerSpeedUpdateParallel(This, _, updateInfo);
	auto sonic = This->GetContext();
	auto state = This->m_StateMachine.GetCurrentState()->GetStateName();
	Hedgehog::Base::CSharedString deadStates[] = { "NormalDamageDead","NormalDamageDeadAir" };
	if (sonic) {
		if (sonic->m_RingCount != 0) {
			if (!contains(deadStates,state)) {
				sonic->m_pPlayer->SendMessage(sonic->m_pPlayer->m_ActorID, boost::make_shared<Sonic::Message::MsgDead>(true));
				if (sonic->m_Grounded) {
					sonic->ChangeState("NormalDamageDead");
				}
				else {
					sonic->ChangeState("NormalDamageDeadAir");
				}
				isDead = true;	
			}
			else {
				isDead = false;
			}
		}
	}
}

void Update::Install()
{
	INSTALL_HOOK(CPlayerSpeedUpdateParallel);
}