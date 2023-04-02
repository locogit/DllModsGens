#pragma once

class LetterboxHelper
{

public:
	static Hedgehog::Math::CVector2* Resolution;
	static Hedgehog::Math::CVector2* OriginalResolution;
	static Hedgehog::Math::CVector2* ScreenHalfPoint;
	static void Initialize(float resW, float resH);
	static void SetResolution(float resW, float resH);
	static void ScaleScene(Chao::CSD::CScene* scene);
};