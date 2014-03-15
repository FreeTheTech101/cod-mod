// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: UI file loading
//
// Initial author: momo5502
// Started: 2014-03-15
// ==========================================================

#include "StdInc.h"

void Printf_Con(const char* buffer);

void* loadMenuHook(int type, const char* reference, bool stuff)
{
	Printf_Con(va("Loading menu: '%s'\n", reference));
	return DB_FindXAssetHeader_MW3(type, reference, stuff);
}


void PatchMW3_UILoading()
{
	call(0x5415E9, loadMenuHook, PATCH_CALL);
}