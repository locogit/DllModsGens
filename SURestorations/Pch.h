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
#include <mmsystem.h>
#include <iostream>
#include <fstream>
#include <deque>
// Dependencies
#include "CSDCommon.h"
#include "CPlayerSpeedUpdate.h"
#include <Helpers.h>
#include <INIReader.h>
#include <StringHelper.h>