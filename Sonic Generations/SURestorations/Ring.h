#pragma once
class Ring
{
public:
    /// <summary>
    /// Installs the mid-ASM hooks.
    /// </summary>
    static void Install();
    static inline float ringLess100Add;
    static inline float ringGreat100Less200Add;
    static inline float ringGreat200Add;
};

