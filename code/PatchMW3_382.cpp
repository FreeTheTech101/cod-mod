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

void PatchMW2_LocalizedStrings();
void PatchMW2_Minidump();
void PatchMW2_Branding();
void PatchMW2_NoBorder();
void PatchMW2_Steam();
void PatchMW2_Icon();
void PatchMW3_UILoading();
char* GetUsername();
ISteamFriends* __cdecl SteamFriends();
hostent* WINAPI custom_gethostbyname(const char* name);

void patchExit(DWORD address)
{
	*(BYTE*)address = 0xEB;
}

DWORD SteamAPIMW3;
DWORD returnSuccessMW3 = 0x443855;
DWORD otherStuffMW3 = 0x40CCB0;

void Printf_Con(const char* buffer);

void Com_Printf_MW3(int type, const char *fmt, ... )
{
	va_list ap;
	char *dest = (char*)malloc(1024);
	va_start(ap, fmt);
	vsprintf( dest, fmt, ap );
	va_end(ap);
	Printf_Con(dest);
}

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
	else if(version == 159)
	{
		DB_XAssetPool = (void**)0x7337F8;
		g_poolSize = (unsigned int*)0x733510;
		DB_GetXAssetSizeHandlers = (DB_GetXAssetSizeHandler_t*)0x733408;
	}
	else if(version == 177)
	{
		DB_XAssetPool = (void**)0x7998A8;
		g_poolSize = (unsigned int*)0x7995E8;
		DB_GetXAssetSizeHandlers = (DB_GetXAssetSizeHandler_t*)0x799488;
	}
	else if(version == 184)
	{
		DB_XAssetPool = (void**)0x7307F8;
		g_poolSize = (unsigned int*)0x730510;
		DB_GetXAssetSizeHandlers = (DB_GetXAssetSizeHandler_t*)0x730408;
	}

	int elSize = DB_GetXAssetSizeHandlers[type]();
	void* poolEntry = malloc(newSize * elSize);
	DB_XAssetPool[type] = poolEntry;
	g_poolSize[type] = newSize;
	return poolEntry;
}

void printHelloWorld(int type);
void __cdecl SteamAPI_RunCallbacks();

void enableConsole()
{
	// Remove the unnecessary line-breaks
	*(DWORD*)0x49C7D5 = (DWORD)"";
	*(DWORD*)0x49C7EB = (DWORD)"";

	// Enable external console
	((void(*)())0x417C00)();
	((void(*)())0x417080)();

	// Continue startup
	((void(*)())0x5EE380)();

	printHelloWorld(0);
}

dvar_t* Dvar_RegisterBool_MW3(const char* name, int default, int flags)
{

	dvar_t* retval = NULL;
	DWORD registerBool = 0x433060;

	if(version == 358)
		registerBool = 0x50F740;

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

void contentErrorHook(errorParm_s type, const char* format, ...)
{
	const char* zone;

	__asm mov zone, esi

	const char* error = va("Can't find map \"%s\".", zone);
	Com_Error(type, error);
}

void Printf_Con(const char* buffer)
{
	((void(*)(const char*))(version == 382 ? 0x406D80 : 0x49CEE0))(buffer);
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

void _strncpy_hook(char* name_buffer, char* no_name, size_t size)
{
	strncpy(name_buffer, GetUsername(), size);
}

void consoleExitHook_mw3();

void PatchMW3_382()
{
	version = 382;

	Com_Error = (Com_Error_t)0x451A10;
	Com_Printf = Com_Printf_MW3;
	Dvar_FindVar = (Dvar_FindVar_t)0x540520;
	Dvar_GetString = (Dvar_GetString_t)0x474B00;
	R_AddCmdDrawText = (R_AddCmdDrawText_t)0x520D50;
	R_AddCmdDrawStretchPic = (R_AddCmdDrawStretchPic_t)0x5151A0;
	R_RegisterFont = (R_RegisterFont_t)0x484A10;
	R_TextWidth = (R_TextWidth_t)0x4D0820;
	Com_Milliseconds = (Com_Milliseconds_t)0x4AB890;
	CL_IsCgameInitialized = (CL_IsCgameInitialized_t)0x4C61D0;
	DB_FindXAssetHeader = (DB_FindXAssetHeader_t)0x52CED0;

	drawDevStuffHookLoc = 0x567B6C;
	winMainInitHookLoc = 0x503E79;
	windowedWindowStyleHookLoc = 0x65F652;
	SteamFriendsLoc = 0x7915CC;
	loadGameOverlayHookLoc = 0x5EF60C;
	initializeRenderer = (DWORD)nullfunc;
	localizeAssetHookLoc = 0x4C34E3;
	localizeAssetHookLoc2 = 0x4EEFF4;
	localizeAssetHookLoc3 = 0x5F9EFF;

	dvarName = (dvar_t**)0xA5BBDC; // Actually dvar_MW3_t**
	r_mode = (dvar_t**)0x20E730C;

	PatchMW2_LocalizedStrings();
	PatchMW2_Minidump();
	PatchMW2_Branding();
	PatchMW2_NoBorder();
	PatchMW3_UILoading();
	PatchMW2_Steam();
	PatchMW2_Icon();

	// Allow closing external console
	call(0x4F7F58, consoleExitHook_mw3, PATCH_CALL);

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

	// Steam user stuff
	*(DWORD*)0x523890 = 0x90C300B0; // mov al, 0 - retn - nop

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
	//nop(0x567665, 5);
	//*(BYTE*)0x567621 = 0xEB; // xrequirelivesignin stuff

	// No XBLive error
	nop(0x5E3E73, 6);
	*(BYTE*)0x5E3DA3 = 0xEB;
	*(BYTE*)0x5E3CC4 = 0xEB;

	// Content not found hook
	call(0x56E9B3, contentErrorHook, PATCH_CALL);
	
	// CEG stuff?
	patchExit(0x456395);
	patchExit(0x47DE15);
	patchExit(0x4A7925);
	patchExit(0x4FF895);
	patchExit(0x502125);
	patchExit(0x514365);
	patchExit(0x5279E5);

	// Change window titles
	*(DWORD*)0x40ACC4 = (DWORD)"COD-MOD: Console";
	*(DWORD*)0x65F6AF = (DWORD)"COD-MOD: Game";

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

	// Test disable running
	//nop(0x46B06F, 5);

	// Set stat version
	strcpy((char*)0x7CA090, "%s_1.stat\0");
}