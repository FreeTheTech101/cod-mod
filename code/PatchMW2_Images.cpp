// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: steam_api
// Purpose: Support for Iwi version 9
//
// Initial author: NTAuthority
// Started: 2011-11-23
// ==========================================================

#include "stdinc.h"

CallHook imageVersionCheckHook;

void __declspec(naked) ImageVersionCheckHookFunc()
{
	__asm
	{
		cmp eax, 9
		je returnSafely

		jmp imageVersionCheckHook.pOriginal

	returnSafely:
		mov al, 1
		add esp, 18h
		retn
	}
}

void PatchMW2_Images()
{
	imageVersionCheckHook.initialize(imageVersionCheckHookLoc, ImageVersionCheckHookFunc);
	imageVersionCheckHook.installHook();
}