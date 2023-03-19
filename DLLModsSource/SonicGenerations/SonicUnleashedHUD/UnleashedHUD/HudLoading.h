#pragma once
class HudLoading
{
public:
	enum LoadingArchiveType : int
	{
		LAT_Gens,
		LAT_UP,
		LAT_Custom
	};

	static void Install();

	static void StartFadeOut();
	static bool IsFadeOutCompleted();
	static void StartResidentLoading();
	static void EndResidentLoading();
};

