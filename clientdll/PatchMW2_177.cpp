// ==========================================================
// MW2 mp
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Patches for multiplayer version 177
//
// Initial author: momo5502
// Started: 2014-02-15
// ==========================================================

#include "StdInc.h"

void PatchMW2_Steam();
void PatchMW2_Load();
void PatchMW2_Branding();
void PatchMW2_SPMaps();
void PatchMW2_Stats();
void PatchMW2_Images();
void PatchMW2_PartyBypass();
void PatchMW2_MPClientConsole();
void PatchMW2_CModels();
void PatchMW2_CryptoFiles();
void PatchMW2_T6Clips();
void PatchMW2_FifthInfinity();
void PatchMW2_UILoading();
void PatchMW2_LocalizedStrings();
void PatchMW2_Materialism();
void PatchMW2_Loadscreens();
void PatchMW2_Weapons();
void PatchMW2_Experimental();
void* ReallocateAssetPool(int type, unsigned int newSize);

DWORD SteamUserStuff177 = 0x47BDA0;
DWORD returnSuccess177 = 0x451169;
DWORD otherStuff177 = 0x42B210;

void __declspec(naked) steamInitPatch177()
{
	__asm
	{
		call SteamUserStuff177
		test al, al
		jz returnSafe
		jmp returnSuccess

returnSuccess:
		call otherStuff177
		test al, al
		jz returnSafe
		jmp returnSuccess177

returnSafe:
		mov al, 1
		retn
	}
}

cmd_function_t Cmd_OpenMenu;
void Cmd_OpenMenu_f()
{
	if (Cmd_Argc() != 2)
	{
		Com_Printf(0, "USAGE: openmenu <menu name>\n");
		return;
	}
	char* menu = Cmd_Argv(1);
	__asm
	{
		push menu
		push 62E2858h
		mov eax, 4CCE60h
		call eax
	}
}

void PatchMW2_FFHash()
{
	// basic checks (hash jumps, both normal and playlist)
	*(WORD*)0x5B97A3 = 0x9090;
	*(WORD*)0x5BA493 = 0x9090;

	*(WORD*)0x5B991C = 0x9090;
	*(WORD*)0x5BA60C = 0x9090;

	*(WORD*)0x5B97B4 = 0x9090;
	*(WORD*)0x5BA4A4 = 0x9090;

	// some other, unknown, check
	*(BYTE*)0x5B9912 = 0xB8;
	*(DWORD*)0x5B9913 = 1;

	*(BYTE*)0x5BA602 = 0xB8;
	*(DWORD*)0x5BA603 = 1;

	if (IsDebuggerPresent())
	{
		// dirty disk breakpoint
		*(BYTE*)0x4CF7F0 = 0xCC;
	}
}

void PatchMW2_177()
{
	version = 177;

	// Need to clean these up
	Cmd_AddCommand = (Cmd_AddCommand_t)0x470090;
	Com_Printf = (Com_Printf_t)0x402500;
	R_RegisterFont = (R_RegisterFont_t)0x505670;
	R_AddCmdDrawText = (R_AddCmdDrawText_t)0x509D80;
	Dvar_RegisterBool = (Dvar_RegisterBool_t)0x4CE1A0;
	Field_Clear = (Field_Clear_t)0x437EB0;
	Com_Milliseconds = (Com_Milliseconds_t)0x42A660;
	CL_IsCgameInitialized = (CL_IsCgameInitialized_t)0x43EB20;
	Dvar_FindVar = (Dvar_FindVar_t)0x4D5390;
	DB_LoadXAssets = (DB_LoadXAssets_t)0x4E5930;
	FS_WriteFile = (FS_WriteFile_t)0x426450;
	FS_ReadFile = (FS_ReadFile_t)0x4F4B90;
	FS_FreeFile = (FS_FreeFile_t)0x4416B0;

	imageVersionCheckHookLoc = 0x53A456;
	drawDevStuffHookLoc = 0x5ACB99;
	zoneLoadHookLoc = 0x4D7378;
	loadGameOverlayHookLoc = 0x60BE51;
	initializeRenderer = 0x4A6A70;
	SteamFriendsLoc = 0x6D75EC;

	language = (char*)0x649E748;
	dvarName = (dvar_t**)0xB2C680;

	cmd_id = (DWORD*)0x1AAC5D0;
	cmd_argc = (DWORD*)0x1AAC614;
	cmd_argv = (DWORD**)0x1AAC634;

	PatchMW2_MPClientConsole();
	PatchMW2_PartyBypass();
	PatchMW2_Branding();
	PatchMW2_CModels();
	PatchMW2_Images();
	PatchMW2_Steam();
	PatchMW2_Stats();
	PatchMW2_Load();
	PatchMW2_SPMaps();
	PatchMW2_UILoading();
	PatchMW2_Materialism();
	PatchMW2_Loadscreens();
	PatchMW2_LocalizedStrings();
	PatchMW2_Weapons();
	PatchMW2_Experimental();
	PatchMW2_FFHash();
	
	// Extdll stuff
	PatchMW2_FifthInfinity();
	PatchMW2_T6Clips();
	PatchMW2_CryptoFiles();

	Cmd_AddCommand("openmenu", Cmd_OpenMenu_f, &Cmd_OpenMenu, 0);

	// Entirely remove steam support... we don't want you to get banned :D
	*(WORD*)0x45114E = 0x01B0;
	nop(0x451145, 5);
	call(0x451160, steamInitPatch177, PATCH_JUMP);

	// external console
	nop(0x60BB58, 11);

	// SteamNetworking stuff
	*(BYTE*)0x4E6650 = 0xC3;

	// Stats stuff
	*(BYTE*)0x44AAF0 = 0xC3;
	*(BYTE*)0x467CC0 = 0xC3;

	// IDK what this does, could be important though...
	nop(0x593A1A, 3);
	nop(0x593A52, 2);
	//nop(0x593A6B, 2);

	// This looks dangerous to patch
	nop(0x594260, 5);

	// No improper quit popup
	*(BYTE*)0x4113BB = 0xEB;

	// Ignore popup_getting_data (need to pix the playlist stuff instead of patching this)
	*(BYTE*)0x473829 = 0xEB;

	// Let's assume we're online and we call ourself 'STEAM'
	*(BYTE*)0x4FC2D8 = 0xEB;
	*(BYTE*)0x4FC2F4 = 0xEB;
	*(BYTE*)0x649D6F0 = 1;

	// m2demo stuff
	*(DWORD*)0x6431D1 = (DWORD)"data";

	// Disable 'x' commands (except UPNP, that should be added lateron)
	//nop(0x4059E5, 5);

	// alt-tab support
	*(BYTE*)0x45ACE0 = 0xB0;
	*(BYTE*)0x45ACE1 = 0x01;
	*(BYTE*)0x45ACE2 = 0xC3;

	// xblive_rankedmatch
	*(DWORD*)0x420A34 = DVAR_FLAG_NONE;

	// cg_fov
	*(BYTE*)0x4F8E35 = DVAR_FLAG_SAVED;
	*(float*)0x6E5788 = 90.0f;

	// remove fs_game check for moddable rawfiles - allows non-fs_game to modify rawfiles
	*(WORD*)0x61AB76 = 0x9090;

	// remove limit on IWD file loading
	//memset((void*)0x643B94, 0x90, 6);
	*(BYTE*)0x642BF3 = 0xEB;

	// Force debug logging
	nop(0x4AA89F, 2);
	nop(0x4AA8A1, 6);

	// Configstring stuff
	*(BYTE*)0x5AC2C3 = 0xEB; // CL_ParseGamestate

	// Test
	ReallocateAssetPool(ASSET_TYPE_WEAPON, 2400);

	// Hide console
	FreeConsole();
}
