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

	//*(BYTE*)0x6CCD10 = 0xC3;

	*(DWORD*)0x7E1604 = (DWORD)SteamAPI_RestartAppIfNecessary;
	*(DWORD*)0x7E125C = (DWORD)_nullfunc;

	// Prevent ExitProcess call ( probably also debugger traps
	nop(0x451336, 3);
	nop(0x6B55D8, 3);

	// Hangup at mov...weird
	nop(0x4B2A6F, 5);

	// Prevent stack getting overwritten
	// -> call to ExitProcess replaces current stack callback
	nop(0x5207CF, 3);
}