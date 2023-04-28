#pragma once
class Crawl
{
public:
    /// <summary>
    /// Installs the mid-ASM hooks.
    /// </summary>
    static void Install();
    static inline bool crawlToSlide;
    static inline float crawlSpeed;
    static inline float crawlTurnSpeed;
};

