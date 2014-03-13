// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Version detection
//
// Initial author: momo5502
// Started: 2014-02-15
// ==========================================================

#include "StdInc.h"

void patchExit(DWORD address)
{
	*(BYTE*)address = 0xEB;
}

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

typedef int (__cdecl * DB_GetXAssetSizeHandler_t)();
DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers = (DB_GetXAssetSizeHandler_t*)0x850238;

void** DB_XAssetPool = (void**)0x8506A8;
unsigned int* g_poolSize = (unsigned int*)0x8503C8;

void* ReallocateAssetPool(int type, unsigned int newSize)
{
	int elSize = DB_GetXAssetSizeHandlers[type]();
	void* poolEntry = malloc(newSize * elSize);
	DB_XAssetPool[type] = poolEntry;
	g_poolSize[type] = newSize;
	return poolEntry;
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

	// SteamApps
	nop(0x4D91B7, 2);
	nop(0x60FCD0, 2);
	
	patchExit(0x47DE15);
	patchExit(0x5279E5);

	// Fix leaderboarddefinition stuff
	ReallocateAssetPool(41, 228);
}