// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Patches for version 159
//
// Initial author: momo5502
// Started: 2014-02-15
// ==========================================================

#include "StdInc.h"
#include <dbghelp.h>

void PatchMW2_ClientConsole();
void PatchMW2_NoBorder();
void PatchMW2_Coop();
void PatchMW2_Branding();
void PatchMW2_New();
void PatchMW2_UILoading();
void PatchMW2_Minidump();
void PatchMW2_Images();
void PatchMW2_LocalizedStrings();
void PatchMW2_Load();
void PatchMW2_Script();
void PatchMW2_Steam();
void PatchMW2_MusicalTalent();
void PatchMW2_ConsoleStart();

char ingameUsername[32];

char* GetUsername()
{
	if((BOOL)(*dvarName))
	{
		strncpy(ingameUsername, (*dvarName)->current.string, sizeof(ingameUsername));
	}
	else
	{
		strncpy(ingameUsername, "Unknown Player", sizeof(ingameUsername));
	}

	return ingameUsername;
}

DWORD SteamUserStuff = 0x4293F0;
DWORD returnSuccess = 0x43FAF9;

void __declspec(naked) steamInitPatch()
{
	__asm
	{
		call SteamUserStuff
		test al, al
		jz returnSafe
		jmp returnSuccess

returnSafe:
		mov al, 1
		retn
	}
}

void PatchMW2_159()
{
	version = 159;

	PatchMW2_Minidump();
	PatchMW2_Coop();
	PatchMW2_NoBorder();
	PatchMW2_ClientConsole();
	PatchMW2_Branding();
	PatchMW2_Images();
	PatchMW2_LocalizedStrings();
	PatchMW2_Load();
	PatchMW2_UILoading();
	PatchMW2_Script();
	PatchMW2_Steam();
	PatchMW2_MusicalTalent();
	PatchMW2_ConsoleStart();

	// prevent stat loading from steam
	*(BYTE*)0x43FB33 = 0xC3;

	// remove limit on IWD file loading
	*(BYTE*)0x630FF3 = 0xEB;

	// remove fs_game check for moddable rawfiles - allows non-fs_game to modify rawfiles
	nop(0x612932, 2);

	// Ignore 'steam must be running' error
	nop(0x6040A3, 0x30);

	// Patch steam auth
	call(0x43FAF0, steamInitPatch, PATCH_JUMP);

	// Prevent matchmaking stuff
	*(BYTE*)0x43BAE0 = 0xEB;

	// Remove dvar restrictions
	*(BYTE*)0x635841 = 0xEB; // read only
	*(BYTE*)0x635913 = 0xEB; // cheat protected
	*(BYTE*)0x6358A5 = 0xEB; // write protected
	*(BYTE*)0x635974 = 0xEB; // latched

	// Show intro (or not)
	*(BYTE*)0x6035BD = 0;

	// Unflag dvar intro
	*(BYTE*)0x6035BB = 0;

	// Flag cg_fov as saved
	*(BYTE*)0x41ED35 = DVAR_FLAG_SAVED;

	// Ignore config problems
	*(BYTE*)0x4D3FD3 = 0xEB;

	// Video folders
	*(DWORD*)0x50A0B2 = 0x723390; // raw -> main
	*(DWORD*)0x50A094 = (DWORD)"%s\\data\\video\\%s.bik"; // main -> data

	// Force debug logging
	nop(0x456BE5, 2);

	// No improper quit popup
	memset((void*)0x4F5B3A, 0x90, 2);

	// Force external console
	memset((void*)0x604071, 0x90, 21);

	// console '%s: %s> ' string
	*(DWORD*)0x579364 = (DWORD)(VERSIONSTRING "> ");

	// Remove ''
	nop(0x6030A6, 5);

	// version string
	*(DWORD*)0x60426F = (DWORD)(CONSOLESTRING);

	// Apply m2demo stuff
	*(DWORD*)0x631561 = (DWORD)"data";

	// Change external console title
	*(DWORD*)0x446A48 = (DWORD)"IW4SP: Console";

	// Yay, hitmarker in sp :D
	Dvar_RegisterBool("scr_damageFeedback", 0, DVAR_FLAG_SAVED, "Show marker when hitting enemies.");
}