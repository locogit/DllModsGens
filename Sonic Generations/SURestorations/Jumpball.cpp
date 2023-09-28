SharedPtrTypeless jumpBallHandle;

const char* GetParticleName() {
	if (BlueBlurCommon::IsSuper()) {
		switch (Jumpball::ballType)
		{
		case JumpBallType::Default:
			return "ef_ch_sps_yh1_spinattack";
		case JumpBallType::Preview:
			return "ef_ch_sps_yh1_spinattack_preview";
		}
	}
	else {
		switch (Jumpball::ballType)
		{
		case JumpBallType::Default:
			return "ef_ch_sng_yh1_spinattack";
		case JumpBallType::Preview:
			return "ef_ch_sng_yh1_spinattack_preview";
		}
	}
	return "ef_ch_sng_yh1_spinattack";
}

HOOK(char*, __fastcall, JumpBallStart, 0x11BCBE0, rsize_t* This) {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
	Common::fCGlitterCreate(sonic, jumpBallHandle, middlematrixNode, GetParticleName(), 1);
	return originalJumpBallStart(This);
}

HOOK(int, __fastcall, JumpBallEnd, 0x11BCB60, DWORD* This) {
	Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	Common::fCGlitterEnd(sonic, jumpBallHandle, true);
	return originalJumpBallEnd(This);
}

void Jumpball::Install() {
	Jumpball::ballType = (JumpBallType)Common::reader.GetInteger("Changes", "JumpEnum", 0);
	INSTALL_HOOK(JumpBallStart);
	INSTALL_HOOK(JumpBallEnd);

	// Disable Jumpball Particles
	WRITE_STRING(0x15E902C, "");
	WRITE_STRING(0x161C160, "");
	WRITE_STRING(0x15F99DC, "");
}