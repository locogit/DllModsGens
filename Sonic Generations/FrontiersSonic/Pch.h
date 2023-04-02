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

// std
#include <stdint.h>
#include <stdio.h>
#include <array>
#include <set>
#include <vector>
#include <sstream>
#include <iostream>
#include <thread>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <filesystem>

// Dependencies
#include "Update.h"
#include "Common.h"
#include "DebugDrawText.h"
#include <Helpers.h>
#include <INIReader.h>
#include <StringHelper.h>
#include "ArchiveTreePatcher.h"
#include "AnimationSetPatcher.h"