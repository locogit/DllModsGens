## Sonic Unleashed HUD v2.1.0
Update courtesy of brianuuuSonic
- Added Unleashed result screen with E-rank support (including animations for Gens, Unleashed Sonic and Werehog)
- Added Unleashed result music with E-rank and boss music support
- Added Unleashed loading screens (stages/event viewer/tails text/missions). Supports both Generations and Unleashed Project stages.
    - This includes support for custom loading screens for stage mods, by providing a `LoadingCustom.ar.00` file in bb2
- Added Unleashed mission success/fail and game over prompts
- Added Unleashed pause UI
- Added 1-up and ring drop animations to the gameplay HUD
- Added boss health bar
- Added Unleashed UI font textures

## Sonic Unleashed HUD v2.0.2
- Fixed broken HUD in Shadow boss battle

## Sonic Unleashed HUD v2.0.1
- Added Xbox 360 and Preview Build window icons. Select which window icon to use in the mod's configuration

## Sonic Unleashed HUD v2.0
- Ported original YNCP file from Sonic Unleashed (BIG thanks Skyth!) and added custom code related to CSD. This introduces the following
    - Proper gameplay HUD with correct animations and colours
    - Working speed meter
    - Rings go to the ring counter when collected
    - Working SPD marker when passing checkpoints
- Added ability to set window title to Sonic Unleashed, including application icon (thanks Hyper!)

This update marks a **huge** milestone for this mod, so I'd like to thank everyone that was involved behind the development of it. Big thanks to:
- Skyth for pretty much being *the* developer in this version
- brianuuu for writing a proper save system for Shuriken, allowing this to be possible in the first place
- Crash5b for developing Shuriken
- Sajid for developing SharpNeedle
- Hyper for cleaning up the codebase and adding the fun changes to the game's title and icon
- Ahremic for tweaking some Sonic Generations HUD elements to fit the original style of Sonic Unleashed
- M&M for providing clear examples on how to patch the archive tree in order to simplify the button prompt system

## Sonic Unleashed HUD v1.8.2
- Made speedometer longer (thanks Exe.Qution!)
- Fixed an issue where the wrong button prompts were being used for skills

## Sonic Unleashed HUD v1.8.1
- Fixed score counter background appearing when not intended

## Sonic Unleashed HUD v1.8
- Implemented Score Generations support (thanks Exe.Qution!)
- Extended default gauge size to render the full boost bar gradient
- Tweaked the colours, font and position of the timer
- Clamped boost gauge to 100% to avoid issues with extended boost gauge
- Fixed issue where the Switch "X" button would appear in the skill activation prompt even when Switch button prompts were not selected
- Fixed "Ring Energy" text getting cut
- Fixed small empty line at the end of the boost gauge
- Fixed animations issues (thanks Exe.Qution!)

## Sonic Unleashed HUD v1.7.5
- Fixed missing files

## Sonic Unleashed HUD v1.7.4
- Fixed boost gauge gradient. It now properly fades to yellow like in Unleashed.
- Force enable "Boost particles go to Sonic" code.

## Sonic Unleashed HUD v1.7.2
- Added support for automatically detecting the Boost Gauge Up skill.
- Removed the boost gauge up fix option in the mod's configuration.

## Sonic Unleashed HUD v1.7.1
- Fixed a problem in the boost bar's gradient when using forced Anisotropic Filtering and BetterFXPipeline.

## Sonic Unleashed HUD v1.7
- Added support for Xbox Series X|S button prompts. Select it from the available buttons in the "Configure Mod" window in HedgeModManager.

## Sonic Unleashed HUD v1.6.2
- Fixed not getting boost from trick rings;
- Fixed boost bar's right slant being cut and the gradients in the left slant.

## Sonic Unleashed HUD v1.6.1
- Switched to using uncompressed DDS textures for higher quality;
- Further tweaked the boost bar.

## Sonic Unleashed HUD v1.6
- Remade some hud elements from scratch, like the boost bar, in a higher resolution;
- Re-aligned Xbox bumpers.

## Sonic Unleashed HUD v1.5.1
- Fixed a bug where the buttons were stuck on Xbox 360 mode;
- Changed the names of the button prompt options to better represent their respective controllers.

## Sonic Unleashed HUD v1.5
- Added support for Switch button prompts. Select it from the available buttons in the "Configure Mod" window in HedgeModManager.

## Sonic Unleashed HUD v1.4
- Implemented HMM Mod Configuration support. Right click the mod in HedgeModManager to configure it.

## Sonic Unleashed HUD v1.3.2
Unleashed Menu Sounds fixes:
- Fixed press start volume. Now it's full stereo on it's own without relying on two Synths;
- Fixed stage title reveal sound. Sonic Team used two synths to make the sound stereo, so now it's real stereo with only one sound.

## Sonic Unleashed HUD v1.3.1
- *forgot to include the Bumper Textures woops...*

## Sonic Unleashed HUD v1.3
- Remade PlayStation buttons to be higher res and have even better alignment;
- Overhauled menu sounds by Mario Tainaka.

## Sonic Unleashed HUD v1.2.1
- Adjusted PlayStation bumper alignment/size.

## Sonic Unleashed HUD v1.2
- Added support for PlayStation button prompts. Please edit the "UnleashedHUD.ini" file in the mod's root directory.

## Sonic Unleashed HUD v1.1.3
- Fixed **AGAIN** the broken "Ready GO!" intro animation when the game is set to English.

## Sonic Unleashed HUD v1.1.2
- Fixed broken "Ready GO!" intro animation when the game is set to English.

## Sonic Unleashed HUD v1.1
- Fixed boost bar "Ring Energy" text not displaying in languages other than English and Japanese;
- Added multiple language support for the "Ready GO!" intro animation.

## Sonic Unleashed HUD v1.0.1 (RC2)
- Adjusted slant angle on boost bar;
- Public release!

## Sonic Unleashed HUD v1.0 (RC1)
- First release candidate.
