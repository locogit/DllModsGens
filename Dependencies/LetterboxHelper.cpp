Hedgehog::Math::CVector2* LetterboxHelper::Resolution;
Hedgehog::Math::CVector2* LetterboxHelper::OriginalResolution;
Hedgehog::Math::CVector2* LetterboxHelper::ScreenHalfPoint;

HOOK(double, __fastcall, GetWidth, 0x675420)
{
	LetterboxHelper::OriginalResolution = new Hedgehog::Math::CVector2(originalGetWidth(), LetterboxHelper::OriginalResolution->y());
	return LetterboxHelper::Resolution->x();
}
HOOK(double, __fastcall, GetHeight, 0x675430)
{
	LetterboxHelper::OriginalResolution = new Hedgehog::Math::CVector2(LetterboxHelper::OriginalResolution->x(), originalGetHeight());
	return LetterboxHelper::Resolution->y();
}
HOOK(int, __cdecl, TestSpec, 0x682770, DWORD a1, int type, int a3, byte* a4, float a5, float offset)
{
	return originalTestSpec(a1, 1, a3, a4, a5, offset);
}
void LetterboxHelper::SetResolution(float resW = 1280, float resH = 720)
{
	Resolution = new Hedgehog::Math::CVector2(resW, resH);
}
void LetterboxHelper::ScaleScene(Chao::CSD::CScene* scene)
{
	/*printf("\n\n%f", 1.0f / (4.0f / 3.0f));
	scene->SetScale(1.0f / (4.0f / 3.0f), 1);*/
}
void LetterboxHelper::Initialize(float resW = 1280, float resH = 720)
{
	Resolution = new Hedgehog::Math::CVector2(resW, resH);
	OriginalResolution = new Hedgehog::Math::CVector2(0, 0);
	ScreenHalfPoint = new Hedgehog::Math::CVector2(Resolution->x() / 2, Resolution->y() / 2);
	INSTALL_HOOK(GetWidth);
	INSTALL_HOOK(GetHeight);
}