// ==========================================================
// alterOps project
// 
// Component: client
// Sub-component: clientdll
// Purpose: Standard includes for the code.
//
// Initial author: NTAuthority
// Started: 2010-11-21
// ==========================================================

#pragma once

#define _CRT_SECURE_NO_WARNINGS

// Windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// C/C++ headers
#include <map>
#include <vector>

// OSW headers
#define NO_STEAM // to reduce header amount needed
#include "CCallback.h"
#include "ISteamClient008.h"

// custom headers
#include "SteamBase.h"
#include "Utils.h"
#include "Hooking.h"
#include "iw4sp.h"

#define VERSION "1.0-1"
#define VERSIONSTRING "IW4MP: " VERSION
#define VERSIONSTRING_SP "IW4SP: " VERSION