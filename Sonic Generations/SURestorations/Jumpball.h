#pragma once
static enum JumpBallType {
    Default = 0,
    Preview = 1
};
class Jumpball
{
public:
    /// <summary>
    /// Installs the mid-ASM hooks.
    /// </summary>
    static void Install();
    static inline JumpBallType ballType;
};

