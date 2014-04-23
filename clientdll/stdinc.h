// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Standard includes for the code.
//
// Initial author: momo5502
// Started: 2014-01-01
// ==========================================================

#pragma once

#define _CRT_SECURE_NO_WARNINGS

// Windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// C/C++ headers
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <winsock.h>
#include <stdint.h>

// Direct3D
#include "d3d9.h"
#include "myIDirect3D9.h"
#include "myIDirect3DDevice9.h"
#include "myIDirect3DSwapChain9.h"

// OSW headers
#define NO_STEAM // to reduce header amount needed
#include "CCallback.h"
#include "ISteamClient008.h"
#include "ISteamUserStats010.h"

// custom headers
#include "SteamBase.h"
#include "Utils.h"
#include "Hooking.h"
#include "iw4sp.h"

#define VERSION "2.3"
#define VERSION_NUM 2,3
#define VERSIONSTRING "COD-MOD: " VERSION

#define CONSOLESTRING VERSIONSTRING " (built " __DATE__ " " __TIME__ ")"

static netadr_t currentLobbyTarget;