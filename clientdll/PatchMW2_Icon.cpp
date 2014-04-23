// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Change Icon at runtime
//
// Initial author: momo5502
// Started: 2014-04-22
// ==========================================================

#include "stdinc.h"
#include "icon.h"

#define TEMP_ICO "iw4_ico.temp"

HICON hicon;

HICON WINAPI LoadIconAHook(HINSTANCE hInstance, LPCSTR lpIconName)
{
	if(hicon)
	{
		return hicon;
	}
	else
	{
		return LoadIconA(hInstance, lpIconName);
	}
}

void PatchMW2_Icon()
{
	// Load icon
	FILE* fp = fopen(TEMP_ICO, "wb");
	if(fp)
	{
		fwrite(icon, sizeof(BYTE), sizeof(icon), fp);
		fclose(fp);
		hicon = (HICON)LoadImage(GetModuleHandle(0), TEMP_ICO, IMAGE_ICON, 64, 64, LR_LOADFROMFILE);
		remove(TEMP_ICO);
	}

	DWORD loadIconLoc = 0x69433C;

	if(version == 184)
		loadIconLoc = 0x691334;

	*(DWORD*)loadIconLoc = (DWORD)LoadIconAHook;
}