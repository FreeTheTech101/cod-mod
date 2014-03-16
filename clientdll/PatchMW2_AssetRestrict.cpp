// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Modern Warfare 2 patches: asset restrictions and
//          pre-load modifications.
//
// Initial author: NTAuthority
// Started: 2011-05-20
// ==========================================================

#include "StdInc.h"

void AssetRestrict_PreLoadFromMaps(assetType_t type, void* entry, const char* zone);

StompHook dbAddXAssetHook;

void DoBeforeLoadAsset(assetType_t type, void** entry)
{
	if (entry)
	{
		AssetRestrict_PreLoadFromMaps(type, *entry, CURRENT_ZONE_NAME);
	}
}

void __declspec(naked) DB_AddXAssetHookStub()
{
	__asm
	{
		mov eax, [esp + 4]
		mov ecx, [esp + 8]
		push ecx
		push eax
		call DoBeforeLoadAsset
		add esp, 08h

		mov eax, [esp + 8]
		sub esp, 14h
		jmp dbAddXAssetHookRet
	}
}

void PatchMW2_AssetRestrict()
{
	dbAddXAssetHook.initialize(dbAddXAssetHookLoc, DB_AddXAssetHookStub, 7);
	dbAddXAssetHook.installHook();
}