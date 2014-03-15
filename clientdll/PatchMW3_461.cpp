// ==========================================================
// MW2 coop
// 
// Component: IW5SP
// Sub-component: clientdll
// Purpose: Version 461 support
//
// Initial author: momo5502
// Started: 2014-02-15
// ==========================================================

#include "StdInc.h"
void PatchMW2_Minidump();
LONG WINAPI CustomUnhandledExceptionFilter(LPEXCEPTION_POINTERS ExceptionInfo);

void __stdcall _nullfunc(UINT penis)
{
	MessageBox(0, "Test", "", 0);
}

void patchExitLea(DWORD address)
{
	*(BYTE*)address = 0xEB;
}

LPTOP_LEVEL_EXCEPTION_FILTER __stdcall CustomSetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
	return SetUnhandledExceptionFilter(&CustomUnhandledExceptionFilter);
}

void PatchMW3_461()
{
	version = 461;

	winMainInitHookLoc = 0x534398;
	Com_Error = (Com_Error_t)0x425540;

	// Replace ExitProcess()
	//*(DWORD*)0x7E2264 = (DWORD)_nullfunc;

	// Patch SteamAuth
	*(DWORD*)0x4342E0 = 0x90C300B0; // mov al, 1 ; retn ; nop
	*(BYTE*)0x608871 = 0xEB;

	// Allow correct startup ( Prevent process exit )
	nop(0x53439D, 9);
	nop(0x5343C9, 5);
	nop(0x5343D3, 5);
	nop(0x53444A, 9);
	*(BYTE*)0x4862D9 = 0xEB; // Near language def.
	*(BYTE*)0x4B92EF = 0xEB;

	patchExitLea(0x4C7997);
	patchExitLea(0x405C67);
	patchExitLea(0x42FF97);
	//patchExitLea(0x4AEE77);

	nop(0x40CB9F, 5);

	// Prevent Steam popup
	nop(0x5343AB, 6);
	*(BYTE*)0x5343B6 = 0xEB;

	// Prevent exit bug call
	nop(0x4AEEC7, 7);

	*(DWORD*)0x7E2104 = (DWORD)CustomSetUnhandledExceptionFilter;

	// _setjmp3 fail
	nop(0x433565, 0x14);
	//nop(0x4C1C83, 5);
	*(BYTE*)0x4C1C80 = 0xC3;

	// I don't know what I'm doing here :(
	nop(0x47715A, 2);
	nop(0x434577, 2);
	nop(0x62C017, 2);
	nop(0x62BE5A, 2);
	nop(0x520CD8, 2);

	// Fastfile stuff
	//nop(0x586A47, 5);

	// Fix Fastfile loading
	*(DWORD*)0x506B77 = (DWORD)"%s\\zone\\%s\\%s";

	//*(DWORD*)0x403750 = 0x90C300B0; // mov al, 1 ; retn ; nop

	PatchMW2_Minidump();
}