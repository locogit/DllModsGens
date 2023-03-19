#pragma once

#define WIN32_LEAN_AND_MEAN

// Detours
#include <Windows.h>
#include "detours\include\detours.h"

// BlueBlur must be included before STL
#include <BlueBlur.h>

// Standard library
#include <cstdint>
#include <cstdio>
#include <string>
#include <filesystem>

// Dependencies
#include "include\Helpers.h"
#include "include\INIReader.h"
#include "include\ScoreGenerationsAPI\ScoreGenerationsAPI.h"
#include "include\Common.h"
#include "ArchiveTreePatcher.h"
#include "Configuration.h"
#include "DllMain.h"
#include "HudSonicStage.h"
#include "HudResult.h"
#include "HudStatus.h"
#include "HudLoading.h"
#include "HudPause.h"
#include "Patches.h"
#include "Resources.h"
#include "CSDCommon.h"