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
#include "Update.h"
#include "Common.h"
#include "AnimationSetPatcher.h"
#include "DebugDrawText.h"
#include <Helpers.h>
#include <INIReader.h>
#include <StringHelper.h>