// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: DLL Bridge
//
// Initial author: momo5502
// Started: 2014-03-05
// ==========================================================

#include "stdinc.h"
#include <windows.h>

FARPROC proc = 0;

void injectBridge()
{
	TCHAR lpBuffer[MAX_PATH];
	GetSystemDirectory(lpBuffer, MAX_PATH);
	HINSTANCE d3d9 = LoadLibrary(va("%s\\d3d9.dll", lpBuffer));
	proc = GetProcAddress(d3d9,"Direct3DCreate9");
}

__declspec(dllexport) DWORD __stdcall Direct3DCreate9(UINT sdkversion)
{
	__asm
	{
		push sdkversion
		call proc
	}
}