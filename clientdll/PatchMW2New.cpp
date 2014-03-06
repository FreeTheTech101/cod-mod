// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Patches for latest steam version
//
// Initial author: momo5502
// Started: 2014-03-05
// ==========================================================

#include "StdInc.h"

void connectWrapper();
void PatchMW2_Branding();
void PatchMW2_NoBorder();

dvar_t* dvarHook(const char* name, const char* default, int flag, const char* description)
{
	Dvar_RegisterString("connect_ip", va("127.0.0.1:%d", *(DWORD*)0x48C5F7), DVAR_FLAG_SAVED, "Temporary dvar used to connect to coop.");

	return Dvar_RegisterString(name, default, flag, description);
}

void PatchMW2_New()
{
	Dvar_FindVar = (Dvar_FindVar_t)0x4BCA20;
	Dvar_SetCommand = (Dvar_SetCommand_t)0x43A000;
	Dvar_RegisterString = (Dvar_RegisterString_t)0x433DF0;
	Dvar_RegisterBool = (Dvar_RegisterBool_t)0x4866B0;
	Cmd_AddCommand = (Cmd_AddCommand_t)0x428090;
	Cmd_ExecuteSingleCommand = (Cmd_ExecuteSingleCommand_t)0x4DA360;
	Com_Printf = (Com_Printf_t)0x466380;
	R_RegisterFont = (R_RegisterFont_t)0x507FE0;
	R_AddCmdDrawText = (R_AddCmdDrawText_t)0x50E080;

	drawDevStuffSPHookLoc = 0x57AE79;
	windowedWindowStyleHookLoc = 0x50B823;
	
	cmd_id = (DWORD*)0x144C950;
	cmd_argc = (DWORD*)0x144C994;
	cmd_argv = (DWORD**)0x144C9B4;

	PatchMW2_Branding();
	PatchMW2_NoBorder();

	// Force external console
	memset((void*)0x60182F, 0x90, 23);

	// Ignore protocol mismatch
	*(BYTE*)0x65A952 = 0xEB;
	*(BYTE*)0x65A97D = 0xEB;

	// Ignore XUID match
	*(BYTE*)0x65AA3F = 0xEB;

	// Change 'connect' to 'connect_coop'
	*(DWORD*)0x4F3B27 = (DWORD)"connect_coop";

	// Apply m2demo stuff
	*(DWORD*)0x62ED01 = (DWORD)"data";

	// Don't show intro
	*(BYTE*)0x600D6D = 0;
	*(BYTE*)0x600D6B = 0;

	// Code below doesn't work due to some assertion stuff
	// Ignore 'steam must be running' error
	// nop(0x601863, 0x30);

	// Patch steam auth
	// *(WORD*)0x47BE55 = 0x15FF; // Prepare long call
	// *(DWORD*)0x47BE57 = 0x69154C; // SteamAPI_init
	// *(DWORD*)0x47BE5B = 0x90C301B0; // mov al, 1 - retn

	// Remove dvar restrictions
	*(WORD*)0x6330AC = 0xE990; // read only
	*(WORD*)0x63317E = 0xE990; // cheat protected
	*(WORD*)0x633110 = 0xE990; // write protected

	// No improper quit popup
	memset((void*)0x41796A, 0x90, 2);

	static cmd_function_t connectWrapper_cmd;
	Cmd_AddCommand("connect", connectWrapper, &connectWrapper_cmd, 0);

	// Register 'connect_ip'
	call(0x48C5E3, dvarHook, PATCH_CALL);
}