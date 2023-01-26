#pragma once
class Shop
{
public:
    /// <summary>
    /// Installs the mid-ASM hooks.
    /// </summary>
    static void Install();
    static void CreateScreen(Sonic::CGameObject* pParentGameObject);
    static void KillScreen();
    static void ToggleScreen(const bool visible, Sonic::CGameObject* pParentGameObject);
};