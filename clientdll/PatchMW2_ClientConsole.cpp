// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Ingame console
//
// Initial author: momo5502
// Started: 2014-03-06
// ==========================================================

#include "stdinc.h"

StompHook clKeyEventHook;
DWORD clKeyEventHookLoc = 0x442E8E;

void Con_ToggleConsole()
{
	// Toggle console visibility
	*(DWORD*)0x929140 ^= 1;

	// Idk, according to NTAuthority: 'show console output?'
	*(BYTE*)0x886FC8 = 0;

	// Clear console input field
	Field_Clear((void*)0x88C700);
}

void checkConsole(int blah, int blahh, DWORD allowToggle)
{
	if(allowToggle)
	{
		Con_ToggleConsole();
	}
}

CallHook sysInitHook;
DWORD sysInitHookLoc = 0x6042A2;

void __declspec(naked) SysInitHookStub()
{
	Cmd_SetAutoComplete("exec", "", "cfg");
	__asm jmp sysInitHook.pOriginal
}

void PatchMW2_ClientConsole()
{
	sysInitHook.initialize(sysInitHookLoc, SysInitHookStub);
	sysInitHook.installHook();

	clKeyEventHook.initialize(clKeyEventHookLoc, checkConsole);
	clKeyEventHook.installHook();

	// Force 'tilde' jump to 'clKeyEventHook'
	call(0x442EA1, 0x442E8A, PATCH_JUMP);
}