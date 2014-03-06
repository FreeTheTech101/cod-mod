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

#pragma pack(1)
HINSTANCE d3d9 = 0;
FARPROC proc = 0;

void injectBridge()
{
	// Wow, that's a nasty way...
	d3d9 = LoadLibraryW(L"C:\\Windows\\System32\\d3d9.dll");
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