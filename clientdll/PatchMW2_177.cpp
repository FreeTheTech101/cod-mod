// ==========================================================
// MW2 mp
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Patches for multiplayer version 177
//
// Initial author: momo5502
// Started: 2014-02-15
// ==========================================================

#include "StdInc.h"

void PatchMW2_Steam();
void* ReallocateAssetPool(int type, unsigned int newSize);

DWORD SteamUserStuff177 = 0x47BDA0;
DWORD returnSuccess177 = 0x451169;
DWORD otherStuff177 = 0x42B210;

void __declspec(naked) steamInitPatch177()
{
	__asm
	{
		call SteamUserStuff177
		test al, al
		jz returnSafe
		jmp returnSuccess

returnSuccess:
		call otherStuff177
		test al, al
		jz returnSafe
		jmp returnSuccess177

returnSafe:
		mov al, 1
		retn
	}
}

void _Con_ToggleConsole()
{
	*(DWORD*)0xB2C538 ^= 1;
	Field_Clear((void*)0xA1B6B0);
	*(BYTE*)0xA15F38 = 0;
}

CallHook clKeyEventToggleConsoleHook1;
DWORD clKeyEventToggleConsoleHook1Loc =	0x4F690C;

StompHook clKeyEventToggleConsoleHook2;
DWORD clKeyEventToggleConsoleHook2Loc = 0x4F65A5;

static void PatchMW2_ClientConsole_Toggle()
{
	Field_Clear = (Field_Clear_t)0x437EB0;

	clKeyEventToggleConsoleHook1.initialize(clKeyEventToggleConsoleHook1Loc, _Con_ToggleConsole);
	clKeyEventToggleConsoleHook1.installHook();

	clKeyEventToggleConsoleHook2.initialize(clKeyEventToggleConsoleHook2Loc, _Con_ToggleConsole);
	clKeyEventToggleConsoleHook2.installHook();
}

void PatchMW2_177()
{
	version = 177;

	Com_Printf = (Com_Printf_t)0x402500;

	loadGameOverlayHookLoc = 0x60BE51;
	initializeRenderer = 0x4A6A70;

	PatchMW2_Steam();
	PatchMW2_ClientConsole_Toggle();

	// Entirely remove steam support... we don't want you to get banned :D
	*(WORD*)0x45114E = 0x01B0;
	nop(0x451145, 5);
	call(0x451160, steamInitPatch177, PATCH_JUMP);

	// external console
	nop(0x60BB58, 11);

	// SteamNetworking stuff
	*(BYTE*)0x4E6650 = 0xC3;

	// Stats stuff
	*(BYTE*)0x44AAF0 = 0xC3;
	*(BYTE*)0x467CC0 = 0xC3;

	// IDK what this does, could be important though...
	nop(0x593A1A, 3);
	nop(0x593A52, 2);

	// No improper quit popup
	*(BYTE*)0x4113BB = 0xEB;

	// Test
	ReallocateAssetPool(ASSET_TYPE_WEAPON, 2400);
}