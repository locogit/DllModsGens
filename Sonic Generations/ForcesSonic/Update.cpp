HOOK(void, __fastcall, ProcMsgChangeResultState, 0xE27BA0, void* This, void* Edx, void* pMessage)
{
	*(uint32_t*)((char*)pMessage + 20) = rand() % 5;
	originalProcMsgChangeResultState(This, Edx, pMessage);
}

enum MouthNodeType : uint32_t
{
	MOUTH_NODE_TYPE_RIGHT = 0,
	MOUTH_NODE_TYPE_LEFT = 1
};

struct MouthNode
{
	MouthNodeType type;
	const char* name;
};

const MouthNode nodes[] =
{
	{ MOUTH_NODE_TYPE_LEFT, "MouthL_Reference" },
	{ MOUTH_NODE_TYPE_LEFT, "MouthL_Position" },
	{ MOUTH_NODE_TYPE_LEFT, "MouthL_Lip_C" },
	{ MOUTH_NODE_TYPE_LEFT, "MouthL_Teeth_Up" },
	{ MOUTH_NODE_TYPE_LEFT, "MouthL_Lip_L" },
	{ MOUTH_NODE_TYPE_LEFT, "MouthL_Lip_R" },
	{ MOUTH_NODE_TYPE_LEFT, "MouthL_Jaw" },
	{ MOUTH_NODE_TYPE_LEFT, "MouthL_Teeth_Low" },
	{ MOUTH_NODE_TYPE_LEFT, "MouthL_MouthTop" },
	{ MOUTH_NODE_TYPE_LEFT, "MouthL_Nose" },
	{ MOUTH_NODE_TYPE_LEFT, "MouthL_UnderEye_L" },
	{ MOUTH_NODE_TYPE_LEFT, "MouthL_UnderEye_R" },
	{ MOUTH_NODE_TYPE_LEFT, "MouthL_Cheek_L" },
	{ MOUTH_NODE_TYPE_LEFT, "MouthL_Cheek_R" },

	{ MOUTH_NODE_TYPE_RIGHT, "MouthR_Reference" },
	{ MOUTH_NODE_TYPE_RIGHT, "MouthR_Position" },
	{ MOUTH_NODE_TYPE_RIGHT, "MouthR_Lip_C" },
	{ MOUTH_NODE_TYPE_RIGHT, "MouthR_Teeth_Up" },
	{ MOUTH_NODE_TYPE_RIGHT, "MouthR_Lip_L" },
	{ MOUTH_NODE_TYPE_RIGHT, "MouthR_Lip_R" },
	{ MOUTH_NODE_TYPE_RIGHT, "MouthR_Jaw" },
	{ MOUTH_NODE_TYPE_RIGHT, "MouthR_Teeth_Low" },
	{ MOUTH_NODE_TYPE_RIGHT, "MouthR_MouthTop" },
	{ MOUTH_NODE_TYPE_RIGHT, "MouthR_Nose" },
	{ MOUTH_NODE_TYPE_RIGHT, "MouthR_UnderEye_L" },
	{ MOUTH_NODE_TYPE_RIGHT, "MouthR_UnderEye_R" },
	{ MOUTH_NODE_TYPE_RIGHT, "MouthR_Cheek_L" },
	{ MOUTH_NODE_TYPE_RIGHT, "MouthR_Cheek_R" },
};

//https://github.com/brianuuu/DllMods/blob/master/Source/ColorsVoice/Mod.cpp
//https://github.com/brianuuu/DllMods/blob/master/Source/Sonic06DefinitiveExperience/NextGenPhysics.cpp
HOOK(void, __fastcall, RampApplyImpulse, 0xE6CFA0, void* This, void* Edx, MsgApplyImpulse* message)
{
	originalRampApplyImpulse(This, Edx, message);

	if (!BlueBlurCommon::IsModern())
		return;

	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	switch (message->m_impulseType)
	{
	case ImpulseType::JumpBoardSpecial:
		sonic->ChangeAnimation("JumpBoardSpecialR");
		break;
	case ImpulseType::JumpBoard:
		sonic->ChangeAnimation("JumpBoardSpecialR");
		break;
	}
}

HOOK(int, __fastcall, RailSwitch, 0x1232570, void* This) {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	sonic->ChangeState("Grind");
	return 0;
}

void Update::Install() {
	// Forces Mouth (Skyth)
	// Count
	WRITE_MEMORY(0xE197FB + 2, uint32_t, sizeof(nodes));

	// Offsets
	WRITE_MEMORY(0xE19765 + 2, uint32_t, (uint32_t)&nodes + 4);
	WRITE_MEMORY(0xE19795 + 2, uint32_t, (uint32_t)&nodes);

	// Set animations to loop (Brianuuu)
	WRITE_MEMORY(0x1276D20, uint8_t, 0x1D); // DashRingL
	WRITE_MEMORY(0x1276D87, uint8_t, 0x1D); // DashRingR

	if ( Common::reader.GetBoolean("config", "ramp_fix", true) )
		INSTALL_HOOK(RampApplyImpulse);

	INSTALL_HOOK(RailSwitch);
}