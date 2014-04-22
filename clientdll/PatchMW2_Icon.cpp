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

HICON hicon_16x16;
HICON hicon_64x64;

HICON WINAPI LoadIconAHook(HINSTANCE hInstance, LPCSTR lpIconName)
{
	if(hicon_16x16)
	{
		return hicon_16x16;
	}
	else
	{
		return LoadIconA(hInstance, lpIconName);
	}
}

void PatchMW2_Icon()
{
	HINSTANCE hInstance = GetModuleHandle(0);

	// Load icons
	FILE* fp = fopen(TEMP_ICO, "wb");
	if(fp)
	{
		fwrite(icon_16x16, sizeof(BYTE), sizeof(icon_16x16), fp);
		fclose(fp);
		hicon_16x16 = (HICON)LoadImage(hInstance, TEMP_ICO, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		remove(TEMP_ICO);
	}

	fp = fopen(TEMP_ICO, "wb");
	if(fp)
	{
		fwrite(icon_64x64, sizeof(BYTE), sizeof(icon_64x64), fp);
		fclose(fp);
		hicon_64x64 = (HICON)LoadImage(hInstance, TEMP_ICO, IMAGE_ICON, 64, 64, LR_LOADFROMFILE);
		remove(TEMP_ICO);
	}

	DWORD loadIconLoc = 0x69433C;

	if(version == 184)
		loadIconLoc = 0x691334;

	*(DWORD*)loadIconLoc = (DWORD)LoadIconAHook;
}