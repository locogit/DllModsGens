#pragma once
class Misc
{
public:
    /// <summary>
    /// Installs the mid-ASM hooks.
    /// </summary>
    static void Install();
    static inline bool fadeOutAirBoost;
    static inline float airBoostActiveTime;
    static inline float airBoostEndTime;
};

