#pragma once
class Missile
{
public:
    static void Install();
    static void OnUpdate(const hh::fnd::SUpdateInfo& updateInfo);
    static void OnHUDUpdate(const hh::fnd::SUpdateInfo& in_rUpdateInfo);
};
