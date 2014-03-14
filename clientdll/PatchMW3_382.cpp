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

void PatchMW2_Minidump();
void PatchMW2_Branding();
ISteamFriends* __cdecl SteamFriends();

void patchExit(DWORD address)
{
	*(BYTE*)address = 0xEB;
}

DWORD SteamAPIMW3;
DWORD returnSuccessMW3 = 0x443855;
DWORD otherStuffMW3 = 0x40CCB0;

void __declspec(naked) steamInitPatchMW3()
{
	SteamAPIMW3 = *(DWORD*)0x7915D4;
	*(BYTE*)0x1E1A0A0 = 1;
	__asm
	{
		call SteamAPIMW3
		test eax, eax
		jz returnSafe
		jmp returnSuccess

returnSuccess:
		call otherStuffMW3
		test al, al
		jz returnSafe
		jmp returnSuccessMW3

returnSafe:
		mov al, 1
		retn
	}
}

void patchDevmap()
{
	if( !strcmp((char*)0x1679077, "dvarstring(ui_load_so_level)"))
	{
		dvar_MW3_t* ui_load_so_level = (dvar_MW3_t*)Dvar_FindVar("ui_load_so_level");
		strcpy((char*)0x1679077, ui_load_so_level->current.string);
	}

	// Call actual 'devmap'
	((void (*)())0x6345B0)();
}

typedef int (__cdecl * DB_GetXAssetSizeHandler_t)();
DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers = (DB_GetXAssetSizeHandler_t*)0x850238;

void** DB_XAssetPool = (void**)0x8506A8;
unsigned int* g_poolSize = (unsigned int*)0x8503C8;

void* ReallocateAssetPool(int type, unsigned int newSize)
{
	// Wow need to cleanup all this...
	if(version == 358)
	{
		DB_XAssetPool = (void**)0x84F668;
		g_poolSize = (unsigned int*)0x84F388;
		DB_GetXAssetSizeHandlers = (DB_GetXAssetSizeHandler_t*)0x84F1F8;
	}

	int elSize = DB_GetXAssetSizeHandlers[type]();
	void* poolEntry = malloc(newSize * elSize);
	DB_XAssetPool[type] = poolEntry;
	g_poolSize[type] = newSize;
	return poolEntry;
}

struct script_functiondef
{
	int id;
	void (*func)(int);
	int unknown;
};

static void nullfunc(int){}

void enableConsole()
{
	*(DWORD*)0x7915CC = (DWORD)SteamFriends;

	// Remove the unnecessary line-breaks
	*(DWORD*)0x49C7D5 = (DWORD)"";
	*(DWORD*)0x49C7EB = (DWORD)"";

	// Enable external console
	((void(*)())0x417C00)();
	((void(*)())0x417080)();

	// Continue startup
	((void(*)())0x5EE380)();
}

dvar_t* Dvar_RegisterBool_MW3(const char* name, int default, int flags)
{

	dvar_t* retval = NULL;
	DWORD registerBool = 0x433060;

	__asm
	{
		push flags
		push default
		push name
		call registerBool
		mov retval, eax
		add esp, 0Ch
	}

	return retval;
}

void contentErrorHook(int type, const char* format, ...)
{
	const char* zone;

	__asm mov zone, esi

	const char* error = va("Can't find map \"%s\".", zone);
	Com_Error(type, error);
}

void Printf_Con(const char* buffer)
{
	((void(*)(const char*))0x406D80)(buffer);
}

char _returnPath[MAX_PATH];

char* addDLCZones()
{
	char* zonePtr;

	__asm mov zonePtr, ecx // Load pointer to fastfile into our variable

	if(GetFileAttributes(va( "zone\\alterSP\\%s", zonePtr)) != INVALID_FILE_ATTRIBUTES)
	{
		strcpy(_returnPath, "zone\\alterSP\\");
	}
	else if(GetFileAttributes(va("zone\\dlc\\%s", zonePtr)) != INVALID_FILE_ATTRIBUTES)
	{
		strcpy(_returnPath, "zone\\dlc\\");
	}
	else
	{
		sprintf(_returnPath, "zone\\%s\\", (char*)0x1C04090); // Current language
	}

	return _returnPath;
}

void PatchMW3_382()
{
	version = 382;

	Com_Error = (Com_Error_t)0x451A10;
	Dvar_FindVar = (Dvar_FindVar_t)0x540520;
	R_AddCmdDrawText = (R_AddCmdDrawText_t)0x520D50;
	R_RegisterFont = (R_RegisterFont_t)0x484A10;
	Com_Milliseconds = (Com_Milliseconds_t)0x4AB890;
	CL_IsCgameInitialized = (CL_IsCgameInitialized_t)0x4C61D0;

	drawDevStuffHookLoc = 0x567B6C;
	winMainInitHookLoc = 0x503E79;
	dvarName = (dvar_t**)0xA5BBDC; // Actually dvar_MW3_t**

	PatchMW2_Minidump();
	PatchMW2_Branding();

	// Dvar name re-flag
	*(BYTE*)0x4BDC99 = 1; // Probably need to write a hook to apply userinfo, but coop isn't working yet, so meh...

	// nop improper quit pop up
	nop(0x541DAA, 5);

	// no startup cinematics
	nop(0x5EF691, 5);

	// m2demo stuff
	*(DWORD*)0x60F1D1 = (DWORD)"data";
	*(DWORD*)0x65402D = (DWORD)("%s\\data\\video\\%s.bik");

	// Update game content
	call(0x5EF3E4, enableConsole, PATCH_CALL);

	// Ignore 'steam must be running' error
	*(BYTE*)0x5EF431 = 0xEB;

	// Patch steam auth
	nop(0x44383A, 7);
	*(BYTE*)0x443841 = 0xEB;
	*(BYTE*)0x443846 = 0x90;
	call(0x443847, steamInitPatchMW3, PATCH_JUMP);

	// Stats stuff
	*(BYTE*)0x649E14 = 0xEB;
	*(BYTE*)0x649D1A = 0xEB;

	// Hook devmap call to fix spec ops bug
	*(DWORD*)0x4A94DC = (DWORD)patchDevmap;

	// Allow loading DLC fastfiles
	call(0x56E94D, addDLCZones, PATCH_CALL);

	// SteamApps
	nop(0x4D91B7, 2);
	nop(0x60FCD0, 2);

	// Allow offline coop
	nop(0x4F6A77, 2);
	*(BYTE*)0x44B26E = 0xEB; // Steam connect
	*(BYTE*)0x501CD4 = 0xEB;

	// No XBLive error
	nop(0x5E3E73, 6);
	*(BYTE*)0x5E3DA3 = 0xEB;
	*(BYTE*)0x5E3CC4 = 0xEB;

	// Content not found hook
	call(0x56E9B3, contentErrorHook, PATCH_CALL);
	
	patchExit(0x456395);
	patchExit(0x47DE15);
	patchExit(0x4A7925);
	patchExit(0x4FF895);
	patchExit(0x502125);
	patchExit(0x514365);
	patchExit(0x5279E5);

	// Fix leaderboarddefinition stuff
	ReallocateAssetPool(41, 228);

	// Enable chaos
	Dvar_RegisterBool_MW3("content_allow_chaos_mode", 1, 4);

	// accessToSubscriberContent = 1
	*(BYTE*)0x5EE831 = 1;

	// Thanks to NTAuthority
	script_functiondef* newFunctions = (script_functiondef*)malloc(sizeof(script_functiondef) * 192);
	memcpy(newFunctions, (void*)0x851248, sizeof(script_functiondef) * 189);

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

	*(char**)0x444C85 = newFunctionsPtr;
	*(char**)0x444C8B = newFunctionsPtr + 8;
	*(char**)0x444C91 = newFunctionsPtr + 4;

	// size of this function table
	*(DWORD*)0x444CA5 = sizeof(script_functiondef) * 192;

	// maximum id for 'global' table?!
	*(DWORD*)0x525EB9 = 0x1C7;

	*(BYTE*)0x452C15 = DVAR_FLAG_SAVED; // cg_fov
}