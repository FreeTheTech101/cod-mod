// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Handlers for recovering lost D3D9 (non-Ex)
//          devices.
//
// Initial author: NTAuthority
// Started: 2013-02-07
// ==========================================================

#include "StdInc.h"

void ForAllCustomImages(void (*handler)(void*));

CallHook beginRecoverDeviceHook;

typedef void (__cdecl * DB_EnumXAssets_t)(assetType_t type, void (*handler)(void*), int maybe0);
static DB_EnumXAssets_t DB_EnumXAssets = (DB_EnumXAssets_t)0;

void BeginRecoverDeviceHookFunc(assetType_t imageType, void (*handler)(void*), int stuff)
{
	ForAllCustomImages(handler);

	if (imageType == ASSET_TYPE_IMAGE)
	{
		DB_EnumXAssets(imageType, handler, stuff);
	}
}

CallHook endRecoverDeviceHook;

void EndRecoverDeviceHookFunc(assetType_t imageType, void (*handler)(void*), int stuff)
{
	ForAllCustomImages(handler);

	if (imageType == ASSET_TYPE_IMAGE)
	{
		DB_EnumXAssets(imageType, handler, stuff);
	}
}

void ClearAllCustomMaterials();

void PreVidRestartHookFunc()
{
	BeginRecoverDeviceHookFunc(ASSET_TYPE_COL_MAP_MP, (void(*)(void*))(version == 159 ? 0x521340 : 0x520CC0), 0);
	ClearAllCustomMaterials();
}

void PostVidRestartHookFunc()
{
	EndRecoverDeviceHookFunc(ASSET_TYPE_COL_MAP_MP, (void(*)(void*))(version == 159 ? 0x521360 : 0x520CE0), 0);
}

CallHook postVidRestartHook;

void __declspec(naked) PostVidRestartHookStub()
{
	__asm
	{
		call PostVidRestartHookFunc
		jmp postVidRestartHook.pOriginal
	}
}

void PatchMW2_RecoverDevice()
{
	DB_EnumXAssets = (DB_EnumXAssets_t)(version == 159 ? 0x4A7A60 : 0x4BFFD0);

	beginRecoverDeviceHook.initialize((version == 159 ? 0x50CD0D : 0x50C58D), BeginRecoverDeviceHookFunc);
	beginRecoverDeviceHook.installHook();

	endRecoverDeviceHook.initialize((version == 159 ? 0x50CD93 : 0x50C613), EndRecoverDeviceHookFunc);
	endRecoverDeviceHook.installHook();

	call((version == 159 ? 0x4F4F79 : 0x4E5169), PreVidRestartHookFunc, PATCH_CALL);

	postVidRestartHook.initialize((version == 159 ? 0x4F4FEC : 0x4E51DC), PostVidRestartHookStub);
	postVidRestartHook.installHook();
}