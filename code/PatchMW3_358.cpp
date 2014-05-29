// ==========================================================
// MW2 coop
// 
// Component: IW5SP
// Sub-component: clientdll
// Purpose: Version detection
//
// Initial author: momo5502
// Started: 2014-02-15
// ==========================================================

#include "StdInc.h"

void* ReallocateAssetPool(int type, unsigned int newSize);
dvar_t* Dvar_RegisterBool_MW3(const char* name, int default, int flags);
void _strncpy_hook(char* name_buffer, char* no_name, size_t size);
ISteamFriends* __cdecl SteamFriends();
void PatchMW2_Minidump();
void PatchMW2_Branding();
void PatchMW2_NoBorder();
void PatchMW2_Steam();
void PatchMW2_Icon();

void _patchDevmap()
{
	// 0x1678177 - Pointer to devmap parameter
	if( !strcmp((char*)0x1678177, "dvarstring(ui_load_so_level)"))
	{
		dvar_MW3_t* ui_load_so_level = (dvar_MW3_t*)Dvar_FindVar("ui_load_so_level");
		strcpy((char*)0x1678177, ui_load_so_level->current.string);
	}

	// Call actual 'devmap'
	((void (*)())0x6335B0)();
}

char __returnPath[MAX_PATH];

char* _addDLCZones()
{
	char* zonePtr;

	__asm mov zonePtr, ecx // Load pointer to fastfile into our variable

	if(GetFileAttributes(va( "zone\\alter\\%s", zonePtr)) != INVALID_FILE_ATTRIBUTES)
	{
		strcpy(__returnPath, "zone\\alter\\");
	}
	else if(GetFileAttributes(va("zone\\dlc\\%s", zonePtr)) != INVALID_FILE_ATTRIBUTES)
	{
		strcpy(__returnPath, "zone\\dlc\\");
	}
	else
	{
		sprintf(__returnPath, "zone\\%s\\", (char*)0x1C006D0); // Current language
	}

	return __returnPath;
}

DWORD _SteamAPIMW3;
DWORD _returnSuccessMW3 = 0x542540;
DWORD _otherStuffMW3 = 0x4D4880;

void __declspec(naked) _steamInitPatchMW3()
{
	_SteamAPIMW3 = *(DWORD*)0x7905D4;
	*(BYTE*)0x1E166DC = 1;
	__asm
	{
		call _SteamAPIMW3
		test eax, eax
		jz returnSafe
		jmp returnSuccess

returnSuccess:
		call _otherStuffMW3
		test al, al
		jz returnSafe
		jmp _returnSuccessMW3

returnSafe:
		mov al, 1
		retn
	}
}

void fixStats()
{
	DWORD jump = 0x648CB1;
}

void printHelloWorld(int type);

void _enableConsole()
{
	// Enable external console
	((void(*)())0x46D460)();
	((void(*)())0x5050A0)();

	// Continue startup
	((void(*)())0x5ED550)();

	printHelloWorld(0);
}

void Com_Printf_MW3(int type, const char *fmt, ... );
void __cdecl SteamAPI_RunCallbacks();

void consoleExitHook_mw3()
{
	int mbId = MessageBox(0, "Do you want to close the game?", "Warning", MB_YESNOCANCEL | MB_ICONWARNING);

	if (mbId == IDYES)
	{
		((void(*)())(version == 358 ? 0x4B88A0 : 0x4ACC50))();
	}
	else if (mbId == IDNO)
	{
		DestroyWindow(*(HWND*)(version == 358 ? 0x1C04388 : 0x1C07D48));
	}
}

void PatchMW3_358()
{
	version = 358;

	Dvar_FindVar = (Dvar_FindVar_t)0x4755D0;
	Dvar_GetString = (Dvar_GetString_t)0x43BC90;
	Com_Error = (Com_Error_t)0x472BC0;
	Com_Printf = Com_Printf_MW3;
	Cmd_AddCommand = (Cmd_AddCommand_t)0x475C40;
	Cmd_ExecuteSingleCommand = (Cmd_ExecuteSingleCommand_t)0x4DA7D0;
	R_AddCmdDrawText = (R_AddCmdDrawText_t)0x502730;
	R_RegisterFont = (R_RegisterFont_t)0x466BE0;
	R_AddCmdDrawStretchPic = (R_AddCmdDrawStretchPic_t)0x4E4130;
	R_TextWidth = (R_TextWidth_t)0x41B3F0;
	Com_Milliseconds = (Com_Milliseconds_t)0x4DA1B0;
	DB_FindXAssetHeader = (DB_FindXAssetHeader_t)0x4565B0;
	CL_IsCgameInitialized = (CL_IsCgameInitialized_t)0x506120;

	drawDevStuffHookLoc = 0x566D5C;
	winMainInitHookLoc = 0x470E09;
	windowedWindowStyleHookLoc = 0x65E512;
	SteamFriendsLoc = 0x7905CC;
	loadGameOverlayHookLoc = 0x5EE7CC;
	initializeRenderer = (DWORD)nullfunc;

	r_mode = (dvar_t**)0x20E393C;
	dvarName = (dvar_t**)0xA5AB58;

	PatchMW2_Minidump();
	PatchMW2_Branding();
	PatchMW2_NoBorder();
	PatchMW2_Steam();
	PatchMW2_Icon();

	// Allow closing external console
	call(0x49D768, consoleExitHook_mw3, PATCH_CALL);

	// nop improper quit pop up
	memset((void*)0x4E18EA, 0x90, 5);

	// no startup cinematics
	memset((void*)0x5EE851, 0x90, 5);

	// Fix leaderboarddefinition stuff
	ReallocateAssetPool(41, 228);

	// m2demo ;)
	*(DWORD*)0x60E281 = (DWORD)"data";

	*(DWORD*)0x41890C = (DWORD)_patchDevmap; // Hook devmap call to fix spec ops bug
	call(0x56DCED, _addDLCZones, PATCH_CALL);
	call(0x5EE5A4, _enableConsole, PATCH_CALL);

	// Ignore 'steam must be running' error
	*(BYTE*)0x5EE5F1 = 0xEB;

	// Patch steam auth
	nop(0x542525, 7);
	*(BYTE*)0x54252C = 0xEB;
	*(BYTE*)0x542531 = 0x90;
	call(0x542532, _steamInitPatchMW3, PATCH_JUMP);

	// SteamApps
	*(DWORD*)0x48E050 = 0x90C301B0; // mov al, 1 - retn - nop

	// Steam user stuff
	*(DWORD*)0x4427C0 = 0x90C300B0; // mov al, 0 - retn - nop

	// Fix stats
	nop(0x648C11, 25);
	*(BYTE*)0x648C53 = 0xEB;

	// Fix more stats
	//nop(0x648AEF, 122);
	*(BYTE*)0x648B69 = 0xEB;

	// Reflag dvar name
	*(BYTE*)0x4B8379 = 1;

	// Change window titles
	*(DWORD*)0x40C9E4 = (DWORD)"COD-MOD: Console";
	*(DWORD*)0x65E56F = (DWORD)"COD-MOD: Game";

	// Enable chaos
	Dvar_RegisterBool_MW3("content_allow_chaos_mode", 1, 4);

	// accessToSubscriberContent = 1
	Dvar_RegisterBool_MW3("accessToSubscriberContent", 1, 0);

	// Thanks to NTAuthority
	script_functiondef* newFunctions = (script_functiondef*)malloc(sizeof(script_functiondef) * 192);
	memcpy(newFunctions, (void*)0x850208, sizeof(script_functiondef) * 189);

	newFunctions[189].id = 0x1C5;
	newFunctions[189].func = nullfunc;
	newFunctions[189].unknown = 0;

	newFunctions[190].id = 0x1C6;
	newFunctions[190].func = nullfunc;
	newFunctions[190].unknown = 0;

	// Lol. Originl one only has 191 entries, but aparently that 192nd one is needed.
	newFunctions[191].id = 0x1C7;
	newFunctions[191].func = nullfunc;
	newFunctions[191].unknown = 0;

	// function table start
	char* newFunctionsPtr = (char*)newFunctions;

	*(char**)0x4E13B5 = newFunctionsPtr;
	*(char**)0x4E13BB = newFunctionsPtr + 8;
	*(char**)0x4E13C1 = newFunctionsPtr + 4;

	// size of this function table
	*(DWORD*)0x4E13D5 = sizeof(script_functiondef) * 192;

	// maximum id for 'global' table?!
	*(DWORD*)0x4E66F9 = 0x1C7;

	*(BYTE*)0x4043C5 = DVAR_FLAG_SAVED; // cg_fov

	// Set stat version
	strcpy((char*)0x79325C, "%s_0.stat\0");
}