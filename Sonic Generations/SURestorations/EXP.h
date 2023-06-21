#pragma once
class EXP
{
public:
    static void Install();
    static void Save();
    static void OnHUDUpdate(const hh::fnd::SUpdateInfo& in_rUpdateInfo);
    static inline bool maxStats;
    static inline bool useStats;
    static inline float expMultiplier;
};
