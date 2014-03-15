// ==========================================================
// MW2 coop
// 
// Component: IW5SP
// Sub-component: clientdll
// Purpose: Version 433 support
//
// Initial author: momo5502
// Started: 2014-02-15
// ==========================================================

#include "StdInc.h"

void __stdcall _nullfunc(UINT penis);
char __cdecl SteamAPI_RestartAppIfNecessary();

void PatchMW3_433()
{
	version = 433;

	*(BYTE*)0x6CCD10 = 0xC3;

	*(DWORD*)0x7E1604 = (DWORD)SteamAPI_RestartAppIfNecessary;
	*(DWORD*)0x7E125C = (DWORD)_nullfunc;
}