// Original code by Brianuuu: https://github.com/brianuuu
using SharedPtrTypeless = boost::shared_ptr<void>;
INIReader reader(INI_FILE);
enum SonicCollision : uint32_t
{
	TypeNoAttack = 0x1E61B5C,
	TypeRagdoll = 0x1E61B60,
	TypeSonicSpinCharge = 0x1E61B64,
	TypeSonicSpin = 0x1E61B68,
	TypeSonicUnbeaten = 0x1E61B6C,
	TypeSuperSonic = 0x1E61B70,
	TypeSonicSliding = 0x1E61B74,
	TypeSonicHoming = 0x1E61B78,
	TypeSonicSelectJump = 0x1E61B7C,
	TypeSonicDrift = 0x1E61B80,
	TypeSonicBoost = 0x1E61B84,
	TypeSonicStomping = 0x1E61B88,
	TypeSonicTrickAttack = 0x1E61B8C,
	TypeSonicSquatKick = 0x1E61B90,
	TypeSonicClassicSpin = 0x1E61B94,
	TypeExplosion = 0x1E61B98,
	TypeBossAttack = 0x1E61B9C,
	TypeGunTruckAttack = 0x1E61BA0,
	TypeRagdollEnemyAttack = 0x1E61BA4,
};
namespace Common
{
	inline uint32_t GetMultiLevelAddress(uint32_t initAddress, std::vector<uint32_t> offsets)
	{
		uint32_t address = *(uint32_t*)initAddress;
		for (uint32_t i = 0; i < offsets.size(); i++)
		{
			uint32_t const& offset = offsets[i];
			address += offset;

			if (i < offsets.size() - 1)
			{
				address = *(uint32_t*)address;
			}
		}
		return address;
	}
	static void* SonicContextSetCollision(SonicCollision collisionType, bool enabled, Sonic::Player::CPlayerSpeedContext* sonic)
	{
		static void* const pEnableFunc = (void*)0xE65610;
		static void* const pDisableFunc = (void*)0xE655C0;
		__asm
		{
			mov		edi, sonic

			mov		ecx, collisionType
			mov		ecx, [ecx]
			push	ecx

			cmp		enabled, 0
			je		jump

			call[pEnableFunc]
			jmp		end

			jump :
			call[pDisableFunc]

				end :
		}
	}
	static void* fCGlitterCreate
	(
		void* pContext,
		SharedPtrTypeless& handle,
		void* pMatrixTransformNode,
		Hedgehog::Base::CSharedString const& name,
		uint32_t flag
	)
	{
		static void* const pCGlitterCreate = (void*)0xE73890;
		__asm
		{
			push    flag
			push    name
			push    pMatrixTransformNode
			mov     eax, pContext
			mov     esi, handle
			call[pCGlitterCreate]
		}
	}
	static void fCGlitterEnd
	(
		void* pContext,
		SharedPtrTypeless& handle,
		bool instantStop
	)
	{
		static void* const pCGlitterEnd = (void*)0xE72650;
		static void* const pCGlitterKill = (void*)0xE72570;
		__asm
		{
			mov     eax, [handle]
			mov     ebx, [eax + 4]
			push    ebx
			test	ebx, ebx
			jz		noIncrement
			mov		edx, 1
			add		ebx, 4
			lock xadd[ebx], edx

			noIncrement :
			mov     ebx, [eax]
				push    ebx
				mov     eax, pContext
				cmp     instantStop, 0
				jnz     jump
				call[pCGlitterEnd]
				jmp     end

				jump :
			call[pCGlitterKill]

				end :
		}
	}
	inline bool IsFileExist(std::string const& file)
	{
		struct stat buffer;
		return stat(file.c_str(), &buffer) == 0;
	}

	inline void GetModIniList(std::vector<std::string>& modIniList)
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		std::string exePath(buffer);
		std::string cpkRedirConfig = exePath.substr(0, exePath.find_last_of("\\")) + "\\cpkredir.ini";

		if (!Common::IsFileExist(cpkRedirConfig))
		{
			printf("%s not exist.\n", cpkRedirConfig.c_str());
			return;
		}

		INIReader reader(cpkRedirConfig);
		std::string modsDatabase = reader.Get("CPKREDIR", "ModsDbIni", "mods\\ModsDB.ini");

		if (!Common::IsFileExist(modsDatabase))
		{
			printf("%s not exist.\n", modsDatabase.c_str());
			return;
		}

		INIReader modsDatabaseReader(modsDatabase);
		int count = modsDatabaseReader.GetInteger("Main", "ActiveModCount", 0);
		for (int i = 0; i < count; i++)
		{
			std::string guid = modsDatabaseReader.Get("Main", "ActiveMod" + std::to_string(i), "");
			std::string config = modsDatabaseReader.Get("Mods", guid, "");
			if (!config.empty() && Common::IsFileExist(config))
			{
				modIniList.push_back(config);
			}
		}
	}

	inline bool IsModEnabled(std::string const& testModName, std::string* o_iniPath = nullptr)
	{
		std::vector<std::string> modIniList;
		GetModIniList(modIniList);
		for (size_t i = 0; i < modIniList.size(); i++)
		{
			std::string const& config = modIniList[i];
			INIReader configReader(config);
			std::string name = configReader.Get("Desc", "Title", "");
			if (name == testModName)
			{
				if (o_iniPath)
				{
					*o_iniPath = config;
				}

				return true;
			}
		}

		return false;
	}
	inline bool IsModEnabledContains(std::string const& testModName, std::string* o_iniPath = nullptr)
	{
		std::vector<std::string> modIniList;
		GetModIniList(modIniList);
		for (size_t i = 0; i < modIniList.size(); i++)
		{
			std::string const& config = modIniList[i];
			INIReader configReader(config);
			std::string name = configReader.Get("Desc", "Title", "");
			if (name.find(testModName) != std::string::npos)
			{
				if (o_iniPath)
				{
					*o_iniPath = config;
				}

				return true;
			}
		}

		return false;
	}
	inline bool GetPlayerTransform(Eigen::Vector3f& position, Eigen::Quaternionf& rotation)
	{
		if (!Sonic::Player::CPlayerSpeedContext::GetInstance()) return false;

		const uint32_t result = *(uint32_t*)((uint32_t) * (void**)((uint32_t)Sonic::Player::CPlayerSpeedContext::GetInstance() + 0x110) + 0xAC);
		if (!result) return false;

		float* pPos = (float*)(*(uint32_t*)(result + 0x10) + 0x70);
		position.x() = pPos[0];
		position.y() = pPos[1];
		position.z() = pPos[2];

		float* pRot = (float*)(*(uint32_t*)(result + 0x10) + 0x60);
		rotation.x() = pRot[0];
		rotation.y() = pRot[1];
		rotation.z() = pRot[2];
		rotation.w() = pRot[3];

		return true;
	}
	inline std::string wideCharToMultiByte(LPCWSTR value)
	{
		char multiByte[0x1000];
		WideCharToMultiByte(CP_UTF8, 0, value, -1, multiByte, _countof(multiByte), 0, 0);
		return std::string(multiByte);
	}
	inline std::wstring multiByteToWideChar(const char* value)
	{
		WCHAR wideChar[0x1000];
		MultiByteToWideChar(CP_UTF8, 0, value, -1, wideChar, _countof(wideChar));
		return std::wstring(wideChar);
	}
	enum LanguageType : uint8_t
	{
		LT_English,
		LT_Japanese,
		LT_French,
		LT_German,
		LT_Spanish,
		LT_Italian
	};
	inline LanguageType GetVoiceLanguageType()
	{
		return *(LanguageType*)Common::GetMultiLevelAddress(0x1E66B34, { 0x4, 0x1B4, 0x7C, 0x10 });
	}

	inline LanguageType GetUILanguageType()
	{
		return *(LanguageType*)Common::GetMultiLevelAddress(0x1E66B34, { 0x8 });
	}
}
struct MsgGetHudPosition
{
	INSERT_PADDING(0x10);
	Eigen::Vector3f m_position;
	INSERT_PADDING(0x4);
	float m_speed; // just a guess?
	uint32_t m_type;
};
void __declspec(naked) groundBoostSuperSonicOnly()
{
	static uint32_t returnAddress = 0xDFF270;
	static uint32_t failAddress = 0xDFF2CB;
	__asm
	{
		// disable air boost for normal Sonic
		mov     eax, [ebx + 534h]
		mov     eax, [eax + 4]
		cmp     byte ptr[eax + 6Fh], 0
		jz      jump

		// original function
		movss   xmm0, dword ptr[ebx + 5BCh]
		jmp[returnAddress]

		jump:
		jmp[failAddress]
	}
}

void __declspec(naked) airBoostSuperSonicOnly()
{
	static uint32_t returnAddress = 0xDFE094;
	static uint32_t failAddress = 0xDFDFE6;
	__asm
	{
		// disable air boost for normal Sonic
		mov     eax, [esi + 534h]
		mov     eax, [eax + 4]
		cmp     byte ptr[eax + 6Fh], 0
		jz      jump

		// original function
		movss   xmm0, dword ptr[esi + 5BCh]
		jmp[returnAddress]

		jump:
		jmp[failAddress]
	}
}
// Chaos Energy
float const c_chaosEnergyReward = 2.0f;

boost::shared_ptr<Sonic::CGameObjectCSD> spLockOn, spExp;
Chao::CSD::RCPtr<Chao::CSD::CProject> rcLockOn, rcExp;
Chao::CSD::RCPtr<Chao::CSD::CScene> cursor_enemy, exp_count;
bool cursorHidden = false;
bool expHidden = false;
int expLevel = 0;
float expAmount = 0;
bool renderGameHud;
float xAspectOffset = 0.0f;
float yAspectOffset = 0.0f;
bool SUMouthFix;
const std::string expFileName = "exp.sav";
std::string readExpFile(int index)
{
	std::string line;
	std::ifstream expFile(expFileName);
	int currentLine = 0;
	if (expFile.is_open())
	{
		while (!expFile.eof()) {
			currentLine++;
			std::getline(expFile, line);
			if (currentLine == index) break;
		}
		return line;
	}
	else {
		return "NULL";
	}
}
void writeToExpFile() {
	std::ofstream expFile(expFileName);
	if (expFile.is_open())
	{
		expFile << std::to_string(expLevel) + "\n";
		expFile << std::to_string(expAmount) + "\n";
		expFile.close();
	}
}
void CreateScreen(Sonic::CGameObject* pParentGameObject)
{
	if (rcLockOn && !spLockOn)
		pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spLockOn = boost::make_shared<Sonic::CGameObjectCSD>(rcLockOn, 0.5f, "HUD", false), "main", pParentGameObject);
	if (rcExp && !spExp)
		pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spExp = boost::make_shared<Sonic::CGameObjectCSD>(rcExp, 0.5f, "HUD", false), "main", pParentGameObject);
}
void KillScreen()
{
	if (spLockOn)
	{
		spLockOn->SendMessage(spLockOn->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spLockOn = nullptr;
	}
	if (spExp) {
		spExp->SendMessage(spExp->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spExp = nullptr;
	}
}
void __fastcall CHudSonicStageRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	KillScreen();
	Chao::CSD::CProject::DestroyScene(rcLockOn.Get(), cursor_enemy);
	Chao::CSD::CProject::DestroyScene(rcExp.Get(), exp_count);
	rcLockOn = nullptr;
	rcExp = nullptr;
	cursor_enemy = nullptr;
	exp_count = nullptr;
}
void CalculateAspectOffsets()
{
	if (*(size_t*)0x6F23C6 != 0x75D8C0D9) // Widescreen Support
	{
		const float aspect = (float)*(size_t*)0x1DFDDDC / (float)*(size_t*)0x1DFDDE0;

		if (aspect * 9.0f > 16.0f)
		{
			xAspectOffset = 720.0f * aspect - 1280.0f;
			yAspectOffset = 0.0f;
		}
		else
		{
			xAspectOffset = 0.0f;
			yAspectOffset = 1280.0f / aspect - 720.0f;
		}
	}
	else
	{
		xAspectOffset = 0.0f;
		yAspectOffset = 0.0f;
	}
}
HOOK(void, __fastcall, CHudSonicStageDelayProcessImp, 0x109A8D0, Sonic::CGameObject* This) {
	originalCHudSonicStageDelayProcessImp(This);
	CHudSonicStageRemoveCallback(This, nullptr, nullptr);

	Sonic::CCsdDatabaseWrapper wrapperLockOn(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	boost::shared_ptr<Sonic::CCsdProject> spCsdProjectLockOn;

	Sonic::CCsdDatabaseWrapper wrapperExp(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	boost::shared_ptr<Sonic::CCsdProject> spCsdProjectExp;

	size_t& flags = ((size_t*)This)[151];


	wrapperLockOn.GetCsdProject(spCsdProjectLockOn, "ui_lcursor_enemy");
	rcLockOn = spCsdProjectLockOn->m_rcProject;
	cursor_enemy = rcLockOn->CreateScene("cursor_enemy");
	cursor_enemy->SetHideFlag(true);
	CSDCommon::FreezeMotion(*cursor_enemy);
	cursorHidden = true;

	wrapperExp.GetCsdProject(spCsdProjectExp, "exp");
	rcExp = spCsdProjectExp->m_rcProject;
	exp_count = rcExp->CreateScene("exp_count");
	char text[256];
	sprintf(text, "%02d", expLevel);
	exp_count->GetNode("exp")->SetText(text);
	exp_count->GetNode("gauge")->SetScale(expAmount, 0.65f);
	exp_count->SetPosition(xAspectOffset, 0);
	CSDCommon::FreezeMotion(*exp_count);
	expHidden = true;

	flags &= ~(0x1 | 0x2 | 0x4 | 0x200 | 0x800); // Mask to prevent crash when game tries accessing the elements we disabled later on

	CreateScreen(This);
}
bool expCountDown = false;
float expTimer = 4;
float expTime = 0;
bool SUHud = false;
float ringEnergyAmount = 2.0f;
size_t prevRingCount;
float previousChaosEnergy;
HOOK(void, __fastcall, CHudSonicStageUpdateParallel, 0x1098A50, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo) {
	originalCHudSonicStageUpdateParallel(This, Edx, in_rUpdateInfo);
	renderGameHud = *(bool*)0x1A430D8;
	auto sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (expCountDown) {
		if (expTime > 0) {
			expTime -= in_rUpdateInfo.DeltaTime;
			//exp_count->SetMotion("size");
			//exp_count->SetMotionFrame(expAmount / 0.63f);
			//exp_count->Update(0.0f);
		}
		else if (expTime <= 0) {
			expHidden = false;
			CSDCommon::PlayAnimation(*exp_count, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, exp_count->m_MotionEndFrame, false, true);
			expCountDown = false;
		}
	}
	else {
		if (exp_count->m_MotionFrame <= 0) {
			expHidden = true;
		}
	}
	if (renderGameHud && SUHud) {
		exp_count->SetHideFlag(expHidden);
	}
	else {
		exp_count->SetHideFlag(true);
	}
	if (prevRingCount < sonic->m_RingCount) {
		if (sonic->m_ChaosEnergy < 100.0f && previousChaosEnergy + ringEnergyAmount < 100.0f)
			sonic->m_ChaosEnergy = max(previousChaosEnergy + ringEnergyAmount, 0);
	}
	else {
		previousChaosEnergy = sonic->m_ChaosEnergy;
	}
	if (!cursorHidden) {
		auto& position = Sonic::Player::CPlayerSpeedContext::GetInstance()->m_spMatrixNode->m_Transform.m_Position;
		const auto camera = Sonic::CGameDocument::GetInstance()->GetWorld()->GetCamera();
		hh::math::CVector4 screenPosition = camera->m_MyCamera.m_View * hh::math::CVector4(position.x(), position.y(), position.z(), 1.0f);
		screenPosition = camera->m_MyCamera.m_Projection * screenPosition;
		screenPosition.head<2>() /= screenPosition.w();
		screenPosition.x() = ((screenPosition.x() * 0.5f + 0.5f) * 1280.0f);
		screenPosition.y() = (screenPosition.y() * -0.5f + 0.5f) * 720.0f - 30;
		cursor_enemy->SetPosition(screenPosition.x(), screenPosition.y());
	}
	prevRingCount = sonic->m_RingCount;
}
HOOK(int, __fastcall, ProcMsgRestartStage, 0xE76810, uint32_t* This, void* Edx, void* message)
{
	if (exp_count) {
		expCountDown = false;
		expTime = 0;
		expHidden = true;
		exp_count->SetHideFlag(true);
	}
	return originalProcMsgRestartStage(This, Edx, message);
}
HOOK(void, __fastcall, HudResult_MsgStartGoalResult, 0x10B58A0, uint32_t* This, void* Edx, void* message)
{
	if(!reader.GetBoolean("EXP", "Max", false))
		writeToExpFile();
	originalHudResult_MsgStartGoalResult(This, Edx, message);
}
HOOK(void, __fastcall, ChaosEnergy_MsgGetHudPosition, 0x1096790, void* This, void* Edx, MsgGetHudPosition* message)
{
	if (message->m_type == 0)
	{
		Eigen::Vector3f sonicPosition;
		Eigen::Quaternionf sonicRotation;
		if (Common::GetPlayerTransform(sonicPosition, sonicRotation))
		{
			sonicPosition += sonicRotation * (Eigen::Vector3f::UnitY() * 0.5f); // half Sonic's height
			message->m_position = sonicPosition;
			return;
		}
	}

	originalChaosEnergy_MsgGetHudPosition(This, Edx, message);
}
HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicNew</Archive>\n";
		stream << "  </DefAppend>\n";

		str = stream.str();
	}

	const size_t newSize = size + str.size();
	const std::unique_ptr<char[]> buffer = std::make_unique<char[]>(newSize);
	memcpy(buffer.get(), data, size);

	char* insertionPos = strstr(buffer.get(), "<Include>");

	memmove(insertionPos + str.size(), insertionPos, size - (size_t)(insertionPos - buffer.get()));
	memcpy(insertionPos, str.c_str(), str.size());

	bool result;
	{
		result = originalParseArchiveTree(A1, buffer.get(), newSize, database);
	}

	return result;
}
typedef void CSonicContext;
CSonicContext** const PLAYER_CONTEXT = (CSonicContext**)0x1E5E2F0;
SharedPtrTypeless ChaosEnergyHandle;
void chaosEnergyParticle() {
	auto sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
	Common::fCGlitterCreate(sonic, ChaosEnergyHandle, middlematrixNode, "jump_delux", 1);
	if (expHidden && expTime <= 0 && !expCountDown) {
		expHidden = false;
		expTime = expTimer;
		expCountDown = true;
		CSDCommon::PlayAnimation(*exp_count, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 0, 1);
	}
	if (!reader.GetBoolean("EXP","Max",false)) {
		expAmount += 0.25f;
		if (expAmount > 63 && expLevel < 99) {
			expAmount -= 63;
			expLevel++;
		}
	}
	char text[256];
	sprintf(text, "%02d", expLevel);
	exp_count->GetNode("exp")->SetText(text);
	exp_count->GetNode("gauge")->SetScale(expAmount, 0.65f);
}
void __declspec(naked) addBoostFromChaosEnergy()
{
	static uint32_t fpAddBoostToSonicContext = 0xE5D990;
	static uint32_t returnAddress = 0x112459A;
	__asm
	{
		// Check Sonic context just in case
		mov		esi, PLAYER_CONTEXT
		cmp		dword ptr[esi], 0
		je		jump

		// Award Sonic 5 boost
		push	ecx
		push	edi
		push	0
		movss	xmm1, c_chaosEnergyReward
		mov		esi, [esi]
		call[fpAddBoostToSonicContext]
		call	chaosEnergyParticle
		pop		edi
		pop		ecx

		// original function
		jump :
		mov     ecx, [ecx + 0FCh]
			jmp[returnAddress]
	}
}
uint32_t __fastcall getEnemyChaosEnergyAmountImpl(uint32_t* pEnemy)
{
	printf("0x%08X\n", pEnemy[0]);
	switch (pEnemy[0])
	{
	case 0x016F593C: return 5; // EFighter
	case 0x016F70BC: return 3; // Spinner
	case 0x016FB1FC: return 5; // EFighterMissile
	case 0x016FB62C: return 4; // AirCannon
	case 0x016F912C: return 8; // Mole
	default: return 5;
	}
}
void __declspec(naked) getEnemyChaosEnergyAmount()
{
	static uint32_t returnAddress = 0xBE05EF;
	__asm
	{
		mov		ecx, esi
		call	getEnemyChaosEnergyAmountImpl
		mov		ecx, eax
		jmp[returnAddress]
	}
}

SharedPtrTypeless RampHandle;
bool usedRamp;

bool bobsleighBoostCancel = false;
bool usingBobsleigh = false;
bool bobsleigh = false;

template<typename C, typename T>
bool contains(C&& c, T e) {
	return std::find(std::begin(c), std::end(c), e) != std::end(c);
};
bool poleSwing = false;
float poleParticleDelay = 0.25f;
float poleParticleTime = 0;
HOOK(void, __fastcall, CPlayerSpeedUpdateParallel, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	originalCPlayerSpeedUpdateParallel(This, _, updateInfo);
	auto sonic = This->GetContext();
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	auto state = This->m_StateMachine.GetCurrentState()->GetStateName();
	if (state == "ExternalControl" && sonic->GetCurrentAnimationName() == "PoleSpinLoop" && !poleSwing) {
		poleSwing = true;
		poleParticleTime = poleParticleDelay;
	}
	else if(state != "ExternalControl" && poleSwing) {
		if (poleParticleTime > 0) {
			poleParticleTime -= updateInfo.DeltaTime;
		}
		else {
			poleSwing = false;
		}
	}

	// If bobsleigh is used (using UP or AP)
	if (usingBobsleigh) {
		// All of the states you do not want to be allowed to boost in.
		Hedgehog::Base::CSharedString list[] = { "BoardWalk","BoardJump","BoardFall","BoardLandJumpShort","CPlayerSpeedStateBoardTrickJump","BoardJumpShort","BoardGrindLandJumpShort","BoardGrindJumpShort","BoardGetOn","BoardDrift","BoardGrind" };
		bobsleigh = contains(list, state);
		if (bobsleigh && !bobsleighBoostCancel) {
			bobsleighBoostCancel = true;
			Common::SonicContextSetCollision(TypeSonicBoost, true, sonic);
			sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_EndBoost] = 1;
			WRITE_JUMP(0xDFF268, groundBoostSuperSonicOnly);
			WRITE_JUMP(0xDFE05F, airBoostSuperSonicOnly);
		}
		else if (!bobsleigh && bobsleighBoostCancel) {
			bobsleighBoostCancel = false;
			Common::SonicContextSetCollision(TypeSonicBoost, false, sonic);
			WRITE_MEMORY(0xDFF268, uint8_t, 0xF3, 0x0F, 0x10, 0x83, 0xBC);
			WRITE_MEMORY(0xDFE05F, uint8_t, 0xF3, 0x0F, 0x10, 0x86, 0xBC);
		}
	}

	if (state != "SpecialJump") { usedRamp = false; }
}
HOOK(__int8, __fastcall, missile, 0x60EFF0, DWORD** This, int a2, int* a3, void* Edx) {
	if (cursorHidden) {
		cursor_enemy->SetHideFlag(false);
		CSDCommon::PlayAnimation(*cursor_enemy, "Lock_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 0, 1);
		cursorHidden = false;
	}
	return originalmissile(This, a2, a3, Edx);
}
HOOK(void, __fastcall, ramp, 0x11DE240, int This) {
	auto sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

	// dash rings use SpecialJump as well, this makes sure you are using a ramp by checking the animation that is playing.

	bool ramp = sonic->GetCurrentAnimationName() == "JumpBoardSpecialR" || sonic->GetCurrentAnimationName() == "JumpBoardSpecialL" || sonic->GetCurrentAnimationName() == "JumpBoardSpecial";

	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);

	if (sonic->StateFlag(eStateFlag_Boost) && ramp && !usedRamp) {
		usedRamp = true;
		Common::fCGlitterCreate(sonic, RampHandle, middlematrixNode, "jump_delux", 1);
	}

	originalramp(This);
}
// https://github.com/brianuuu/DllMods/blob/master/Source/NavigationLightdashSound/NavigationSound.cpp#L14
HOOK(void, __fastcall, MsgStartCommonButtonSign, 0x5289A0, void* thisDeclaration, void* edx, uint32_t a2)
{
	// Disable Y button prompt.
	if (*(uint32_t*)(a2 + 16) == 3)
		return;

	originalMsgStartCommonButtonSign(thisDeclaration, edx, a2);
}
void CreateConsole()
{
	if (!AllocConsole()) {
		// Add some error handling here.
		// You can call GetLastError() to get more info about the error.
		return;
	}

	// std::cout, std::clog, std::cerr, std::cin
	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
	std::cin.clear();

	// std::wcout, std::wclog, std::wcerr, std::wcin
	HANDLE hConOut = CreateFile(_T("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hConIn = CreateFile(_T("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
	SetStdHandle(STD_ERROR_HANDLE, hConOut);
	SetStdHandle(STD_INPUT_HANDLE, hConIn);
	std::wcout.clear();
	std::wclog.clear();
	std::wcerr.clear();
	std::wcin.clear();
}
void CPlayerSpeedUpdate::Install()
{
	//CreateConsole();

	expLevel = reader.GetBoolean("EXP", "Max", false) ? 99 : stoi(readExpFile(1));
	expAmount = reader.GetBoolean("EXP", "Max", false) ? 63 : stof(readExpFile(2));

	SUHud = Common::IsModEnabled("Sonic Unleashed HUD");
	usingBobsleigh = Common::IsModEnabled("Unleashed Project") || Common::IsModEnabledContains("Holoska Adventure Pack");

	SUMouthFix = reader.GetBoolean("Config","SUMouthFix",true);
	//https://github.com/LadyLunanova
	if (SUMouthFix) {
		//Right Mouth
		WRITE_MEMORY(0x015E8FB4, const char*, "Jaw_LT");
		WRITE_MEMORY(0x015E8FBC, const char*, "Lip_C_LT");
		WRITE_MEMORY(0x015E8FCC, const char*, "Lip_L_LT");
		WRITE_MEMORY(0x015E8FD4, const char*, "Lip_R_LT");
		//Left Mouth
		WRITE_MEMORY(0x015E8FFC, const char*, "Jaw_LT1");
		WRITE_MEMORY(0x015E9004, const char*, "Lip_C_LT1");
		WRITE_MEMORY(0x015E9014, const char*, "Lip_L_LT1");
		WRITE_MEMORY(0x015E901C, const char*, "Lip_R_LT1");
	}

	INSTALL_HOOK(ParseArchiveTree);
	INSTALL_HOOK(ramp);
	//INSTALL_HOOK(missile);
	INSTALL_HOOK(ChaosEnergy_MsgGetHudPosition);
	INSTALL_HOOK(CHudSonicStageDelayProcessImp);
	INSTALL_HOOK(CHudSonicStageUpdateParallel);
	INSTALL_HOOK(CPlayerSpeedUpdateParallel);
	INSTALL_HOOK(ProcMsgRestartStage);
	INSTALL_HOOK(MsgStartCommonButtonSign);
	INSTALL_HOOK(HudResult_MsgStartGoalResult);

	WRITE_MEMORY(0x16A467C, void*, CHudSonicStageRemoveCallback);

	// 06 Experience Code
	// Set absorb time to 1.2s
	static float ChaosEnergyParam[] =
	{
		3.0f,	// UpHeight
		0.3f,	// UpTime
		0.35f	// AbsorbTime
	};
	WRITE_MEMORY(0xC8EF3D, float*, ChaosEnergyParam);
	WRITE_MEMORY(0x11244A6, float*, ChaosEnergyParam);
	WRITE_JUMP(0xBE05E9, getEnemyChaosEnergyAmount);

	// Don't reward boost from enemy spawned chaos energy
	WRITE_JUMP(0xE60C6C, (void*)0xE60D79);
	// Spawn chaos energy base on currect trick level (visual only)
	WRITE_MEMORY(0x16D1970, uint32_t, 0, 0, 0, 0);
	// Give 0 chaos energy for board trick jump (visual only)
	WRITE_MEMORY(0x11A12E4, uint8_t, 0);
	// Award 5 boost when chaos energy reach Sonic
	WRITE_JUMP(0x1124594, addBoostFromChaosEnergy);
}