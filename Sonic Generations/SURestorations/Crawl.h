#pragma once
class Crawl
{
public:
    /// <summary>
    /// Installs the mid-ASM hooks.
    /// </summary>
    static void Install();
    static void OnUpdate(const hh::fnd::SUpdateInfo& updateInfo);
    static inline bool crawlToSlide;
    static inline float crawlSpeed;
    static inline float crawlTurnSpeed;
    static inline float crawlSlipMult;
    static inline bool isCrawling = false;
};

