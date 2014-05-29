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

#define TEMP_ICO "iwx_ico.temp"

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
		if(version >= 358)
		{
			fwrite(icon_mw3, sizeof(BYTE), sizeof(icon_mw3), fp);
		}
		else
		{
			fwrite(icon_mw2, sizeof(BYTE), sizeof(icon_mw2), fp);
		}
		fclose(fp);
		hicon = (HICON)LoadImage(GetModuleHandle(0), TEMP_ICO, IMAGE_ICON, 64, 64, LR_LOADFROMFILE);
		remove(TEMP_ICO);
	}

	if(version == 159)
	{
		*(DWORD*)0x69433C = (DWORD)LoadIconAHook;
	}
	else if(version == 184)
	{
		*(DWORD*)0x691334 = (DWORD)LoadIconAHook;
	}
	// I hate mixing MW2 and MW3 stuff...
	else if(version == 358)
	{
		*(DWORD*)0x790390 = (DWORD)LoadIconAHook;
	}
	else if(version == 382)
	{
		*(DWORD*)0x791390 = (DWORD)LoadIconAHook;
	}
}