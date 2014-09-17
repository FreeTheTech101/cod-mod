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

// OSW headers
#define NO_STEAM // to reduce header amount needed
#include "CCallback.h"
#include "ISteamUser012.h"
#include "ISteamClient008.h"
#include "ISteamUserStats010.h"

// custom headers
#include "SteamBase.h"
#include "Utils.h"
#include "Hooking.h"
#include "iw4sp.h"
#include "buildnumber.h"

#define COOP_MASTER "react.kthx.at"
#define COOP_MASTER_PORT 20810

#define VERSION "3." COMMIT_STR
#define VERSION_NUM 3,COMMIT
#define VERSIONSTRING "COD-MOD: " VERSION

#define BASEGAME "spdata"

#define CONSOLESTRING VERSIONSTRING " (built " __DATE__ " " __TIME__ " by " BUILDHOST ")"

static netadr_t currentLobbyTarget;