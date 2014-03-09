// ==========================================================
// MW3 coop
// 
// Component: IW5SP
// Sub-component: clientdll
// Purpose: MW3 1.4 Test
//
// Initial author: momo5502
// Started: 2014-03-10
// ==========================================================

#include "stdinc.h"

DWORD SteamAPIMW3;
DWORD returnSuccessMW3 = 0x443855;
DWORD otherStuffMW3 = 0x40CCB0;

void __declspec(naked) steamInitPatchMW3()
{
	SteamAPIMW3 = *(DWORD*)0x7915D4;
	*(BYTE*)0x1E1A0A0 = 1;
	__asm
	{
		call SteamAPIMW3
		test eax, eax
		jz returnSafe
		jmp returnSuccess

returnSuccess:
		call otherStuffMW3
		test al, al
		jz returnSafe
		jmp returnSuccessMW3

returnSafe:
		mov al, 1
		retn
	}
}

void PatchMW3()
{
	// Ignore 'steam must be running' error
	*(BYTE*)0x5EF431 = 0xEB;

	// Patch steam auth
	nop(0x44383A, 7);
	*(BYTE*)0x443841 = 0xEB;
	*(BYTE*)0x443846 = 0x90;
	call(0x443847, steamInitPatchMW3, PATCH_JUMP);

	// DLC Stuff
	nop(0x4D91B7, 2);
	*(BYTE*)0x60FCC0 = 0xC3;
}