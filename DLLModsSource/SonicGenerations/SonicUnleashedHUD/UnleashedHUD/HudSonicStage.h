#pragma once

class HudSonicStage
{
public:
	static bool scoreEnabled;
	static void Install();

	static float xAspectOffset;
	static float yAspectOffset;
	static void CalculateAspectOffsets();
};

