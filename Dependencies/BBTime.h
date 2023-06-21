#pragma once
class BBTime
{
	static inline float timescale = 1;
	static inline bool timescaleEnabled = false;
	static inline float deltaTime = 0;
	static inline float unscaledTime = 0;
	static inline double unscaledTimeAsDouble = 0;
	static inline int frameCount = 0;

public:

	static void SetTimescale(float _timescale, bool _enabled) {
		timescale = _timescale;
		timescaleEnabled = _enabled;
		*(bool*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x19C }) = _enabled;
		*(float*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x1A0 }) = _timescale;
	}

	static void Update(float elapsedTime) {
		deltaTime = elapsedTime;
		unscaledTime += elapsedTime;
		unscaledTimeAsDouble += (double)elapsedTime;
		frameCount++;
	}

	static void Reset() {
		deltaTime = 0;
		unscaledTime = 0;
		unscaledTimeAsDouble = 0;
		frameCount = 0;
	}

	static void Start();

	static float DeltaTime() {
		return deltaTime;
	}

	static float UnscaledTime() {
		return unscaledTime;
	}

	static double UnscaledTimeAsDouble() {
		return unscaledTimeAsDouble;
	}

	static int FrameCount() {
		return frameCount;
	}

	static float GetTimeScale() {
		return *(float*)Common::GetMultiLevelAddress(0x1E0BE5C, { 0x8, 0x1A0 });
	}
};

