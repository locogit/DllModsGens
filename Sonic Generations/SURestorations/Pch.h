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
#include <any>
#include <regex>

// Dependencies
#include "DebugDrawText.h"
#include <INIReader.h>
#include "CSDCommon.h"
#include "Common.h"
#include "ArchiveTreePatcher.h"
#include "AnimationSetPatcher.h"
#include "Helpers.h"
#include <StringHelper.h>
#include "Timer.h"

#include "Misc.h"
#include "EXP.h"
#include "HubUI.h"
#include "Missile.h"
#include "Medal.h"
#include "Sweepkick.h"
#include "UpReel.h"
#include "Stumble.h"
#include "ShortJump.h"
#include "Crawl.h"
#include "Pole.h"
#include "Ramp.h"
#include "UPC.h"
#include "LetterboxHelper.h"
#include "Shop.h"
#include "Save.h"
#include "Ring.h"
#include "FallCam.h"