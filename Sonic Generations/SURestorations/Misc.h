#pragma once
class Misc
{
public:
    /// <summary>
    /// Installs the mid-ASM hooks.
    /// </summary>
    static void Install();
    static void OnUpdate(const hh::fnd::SUpdateInfo& updateInfo);
    static void OnHUDUpdate( const hh::fnd::SUpdateInfo& in_rUpdateInfo);
    static inline bool fadeOutAirBoost;
    static inline float airBoostActiveTime;
    static inline float airBoostEndTime;
};

