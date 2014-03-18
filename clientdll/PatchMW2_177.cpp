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

void PatchMW2_177()
{
	version = 177;

	Com_Printf = (Com_Printf_t)0x402500;

	loadGameOverlayHookLoc = 0x60BE51;
	initializeRenderer = 0x4A6A70;

	PatchMW2_Steam();

	// Patch SteamAuth
	*(WORD*)0x45114E = 0x01B0;

	// Test
	ReallocateAssetPool(ASSET_TYPE_WEAPON, 2400);
}