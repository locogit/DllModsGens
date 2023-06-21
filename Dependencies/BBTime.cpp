#include "BBTime.h"
#include "Helpers.h"
HOOK(void*, __fastcall, UpdateApplication, 0xE7BED0, void* This, void* Edx, float elapsedTime, uint8_t a3)
{
	BBTime::Update(elapsedTime);
	return originalUpdateApplication(This, Edx, elapsedTime, a3);
}

void BBTime::Start() {
	BBTime::Reset();
	INSTALL_HOOK(UpdateApplication);
}