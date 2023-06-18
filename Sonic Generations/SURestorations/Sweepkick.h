#pragma once
class Sweepkick
{
public:
    /// <summary>
    /// Installs the mid-ASM hooks.
    /// </summary>
    static void Install();
    static void OnUpdate(const hh::fnd::SUpdateInfo& updateInfo);
    static inline bool transitionToStand;
    static inline bool useSupportShockwave;
    
    static inline float supportShockwaveDelay;
    static inline float lightInDelay;
    static inline float lightLifeTime;
    static inline float lerpSpeedIn;
    static inline float lightRange;

    static inline float offsetX, offsetY, offsetZ;

    static inline float lerpSpeedOut;

    static inline float colorR, colorG, colorB;

    static inline float colorScalar;

    static inline float colorRSS, colorGSS, colorBSS;

    static inline float colorScalarSS;

    static inline float sweepInputTime;

    static inline bool useLight;
};

