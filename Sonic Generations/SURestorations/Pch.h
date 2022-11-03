#pragma once

#define WIN32_LEAN_AND_MEAN

// Detours
#include <Windows.h>
#include <detours.h>
#include <tchar.h>

// BlueBlur must be included before STL
#include "..\BlueBlurCommon.h"

// Standard library
#include <cstdint>
#include <cstdio>

#include <filesystem>

#include <iostream>
#include <fstream>

// Dependencies
#include <INIReader.h>
#include "CSDCommon.h"
#include "Common.h"
#include "ArchiveTreePatcher.h"
#include "AnimationSetPatcher.h"
#include "Helpers.h"
#include <StringHelper.h>

#include "EXP.h"
#include "HubUI.h"
#include "Missile.h"
#include "Medal.h"
#include "CPlayerSpeedUpdate.h"