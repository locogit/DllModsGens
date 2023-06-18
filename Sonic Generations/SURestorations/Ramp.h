#pragma once
class Ramp
{
public:
    /// <summary>
    /// Installs the mid-ASM hooks.
    /// </summary>
    static void Install();
    static void OnUpdate(const hh::fnd::SUpdateInfo& updateInfo);
};

