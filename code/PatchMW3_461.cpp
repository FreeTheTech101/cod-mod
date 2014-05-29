// ==========================================================
// MW2 coop
// 
// Component: IW5SP
// Sub-component: clientdll
// Purpose: Version detection
//
// Initial author: momo5502
// Started: 2014-05-29
// ==========================================================

#include "StdInc.h"

void patchExit(DWORD address);
void __cdecl SteamAPI_RunCallbacks();
void __cdecl SteamAPI_RegisterCallback(CCallbackBase *pCallback, int iCallback);
char __cdecl SteamAPI_RestartAppIfNecessary();
void __cdecl SteamAPI_Shutdown();
void __cdecl SteamAPI_UnregisterCallResult(CCallbackBase* pResult, SteamAPICall_t APICall);
void __cdecl SteamAPI_UnregisterCallback(CCallbackBase *pCallback, int iCallback);
void __cdecl SteamAPI_RegisterCallResult(CCallbackBase* pResult, SteamAPICall_t APICall);

bool __stdcall IsDebuggerPresent_Hook()
{
	return 0;
}

void PatchMW3_461()
{
	version = 461;

	// Replace Steam api
	*(DWORD*)0x7E2608 = (DWORD)SteamAPI_RestartAppIfNecessary;
	*(DWORD*)0x7E25D8 = (DWORD)SteamAPI_RegisterCallback;
	*(DWORD*)0x7E25DC = (DWORD)SteamAPI_RunCallbacks;
	*(DWORD*)0x7E25F4 = (DWORD)SteamAPI_Shutdown;
	*(DWORD*)0x7E25EC = (DWORD)SteamAPI_UnregisterCallResult;
	*(DWORD*)0x7E25E8 = (DWORD)SteamAPI_UnregisterCallback;
	*(DWORD*)0x7E25F0 = (DWORD)SteamAPI_RegisterCallResult;

	*(DWORD*)0x7E2100 = (DWORD)IsDebuggerPresent_Hook;

	// ------------ Probably the worst way to patch CEG ------------

	// CEG shit (Ticket count stuff)
	patchExit(0x466457);
	patchExit(0x405C67);
	patchExit(0x41A8F7);
	patchExit(0x42F5D7);
	patchExit(0x42FF97);
	patchExit(0x489367);
	patchExit(0x4AEE77);
	patchExit(0x4C7997);
	patchExit(0x4D7B47);
	patchExit(0x4F42F7);

	// Even more naughty CEG stuff (idk, still it overwrites the stack)
	patchExit(0x41305C);
	patchExit(0x47311C);
	patchExit(0x496EAC);
	patchExit(0x4ADBDC);
	patchExit(0x4AE25C);
	patchExit(0x4B4E8C);
	patchExit(0x4C9B5C);
	patchExit(0x5062AC);
	patchExit(0x6B65DC);
	patchExit(0x6B661C);
	patchExit(0x6B665C);
	patchExit(0x6B669C);

	// Patch calls from above
	*(BYTE*)0x49FF10 = 0xC3;
	*(BYTE*)0x4124D0 = 0xC3;
	*(BYTE*)0x445450 = 0xC3;
	*(BYTE*)0x4F9510 = 0xC3;
	*(BYTE*)0x49A930 = 0xC3;
	*(BYTE*)0x451A70 = 0xC3;
	*(BYTE*)0x488710 = 0xC3;
	*(BYTE*)0x6B66D0 = 0xC3;
	*(BYTE*)0x6B6730 = 0xC3;
	*(BYTE*)0x6B6790 = 0xC3;
	*(BYTE*)0x6B67F0 = 0xC3;

	// Test
	*(BYTE*)0x478BB0 = 0xC3; // Return crashing stuff
	*(WORD*)0x457C7F = 0x01B0;
}
