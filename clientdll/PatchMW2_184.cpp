// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Patches for version 184
//
// Initial author: momo5502
// Started: 2014-03-05
// ==========================================================

#include "StdInc.h"
#include "184_defs.h"

void connectWrapper();
void PatchMW2_Branding();
void PatchMW2_NoBorder();
void PatchMW2_Minidump();
void PatchMW2_Images();
void PatchMW2_LocalizedStrings();
void PatchMW2_Load();
void PatchMW2_UILoading();

dvar_t* dvarHook(const char* name, const char* default, int flag, const char* description)
{
	Dvar_RegisterString("connect_ip", va("127.0.0.1:%d", *(DWORD*)0x48C5F7), DVAR_FLAG_SAVED, "Temporary dvar used to connect to coop.");

	return Dvar_RegisterString(name, default, flag, description);
}

void patchSteam()
{
	// Ignore 'steam must be running' error
	nop(0x601863, 0x30);

	// Patch steam auth
	*(WORD*)0x47BE55 = 0x15FF; // Prepare long call
	*(DWORD*)0x47BE57 = 0x69154C; // SteamAPI_init
	*(DWORD*)0x47BE5B = 0x90C301B0; // mov al, 1 - retn
}

void PatchMW2_184()
{
	define184Stuff();

	PatchMW2_Minidump();
	PatchMW2_Branding();
	PatchMW2_NoBorder();
	PatchMW2_Images();
	PatchMW2_LocalizedStrings();
	PatchMW2_Load();
	//PatchMW2_UILoading();

	// Steam patch doesn't really work due to some assertion stuff
	patchSteam();

	// Force external console
	memset((void*)0x60182F, 0x90, 23);

	// Ignore protocol mismatch
	*(BYTE*)0x65A952 = 0xEB;
	*(BYTE*)0x65A97D = 0xEB;

	// Ignore XUID match
	*(BYTE*)0x65AA3F = 0xEB;

	// Change 'connect' to 'connect_coop'
	*(DWORD*)0x4F3B27 = (DWORD)"connect_coop";

	// console version string
	*(DWORD*)0x48681B = (DWORD)(CONSOLESTRING);

	// version string
	*(DWORD*)0x601A2F = (DWORD)(CONSOLESTRING);

	// Apply m2demo stuff
	*(DWORD*)0x62ED01 = (DWORD)"data";

	// Change external console title
	*(DWORD*)0x414828 = (DWORD)"IW4SP: Console";

	// Flag cg_fov as saved
	*(BYTE*)0x47C165 = DVAR_FLAG_SAVED;

	// Flag 'name' as saved
	*(BYTE*)0x4F3856 = DVAR_FLAG_SAVED;

	// Don't show intro
	*(BYTE*)0x600D6D = 0;
	*(BYTE*)0x600D6B = 0;

	// Force debug logging
	nop(0x4B3AE5, 2);

	// Remove dvar restrictions
	*(BYTE*)0x633071 = 0xEB; // read only
	*(BYTE*)0x633143 = 0xEB; // cheat protected
	*(BYTE*)0x6330D5 = 0xEB; // write protected
	*(BYTE*)0x6331A4 = 0xEB; // latched

	// No improper quit popup
	memset((void*)0x41796A, 0x90, 2);

	// Yay, hitmarker in sp :D
	Dvar_RegisterBool("scr_damageFeedback", 0, DVAR_FLAG_SAVED, "Show marker when hitting enemies.");

	static cmd_function_t connectWrapper_cmd;
	Cmd_AddCommand("connect", connectWrapper, &connectWrapper_cmd, 0);

	// Register 'connect_ip'
	call(0x48C5E3, dvarHook, PATCH_CALL);
}