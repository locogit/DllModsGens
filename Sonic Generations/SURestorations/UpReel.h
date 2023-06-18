#pragma once
class UpReel
{
public:
    /// <summary>
    /// Installs the mid-ASM hooks.
    /// </summary>
    static void Install();
    static void OnUpdate(const hh::fnd::SUpdateInfo& updateInfo);
    static inline float upReelForceMultiplier;
};

