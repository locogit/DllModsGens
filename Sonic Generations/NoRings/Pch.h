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
#include <conio.h>

#include <filesystem>

#include <iostream>
#include <fstream>

// Dependencies
#include <INIReader.h>
#include "Common.h"
#include "Helpers.h"
#include <StringHelper.h>

#include "Update.h"
#include "DebugDrawText.h"
#include "ArchiveTreePatcher.h"
#include "CSDCommon.h"
#include "LetterboxHelper.h"