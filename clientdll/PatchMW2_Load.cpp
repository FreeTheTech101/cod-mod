// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Fastfile loading modifications
//
// Initial author: momo5502
// Started: 2014-03-08
// ==========================================================

#include "stdinc.h"
#include <ShellAPI.h>
#include <sys/stat.h>
#include <direct.h>
#include <io.h>

dvar_t* specialops;

void* ReallocateAssetPool(int type, unsigned int newSize);

// Allow civilians to be killed in 'No Russian' if game is censored.
void uncutGame(XZoneInfo* data, int count)
{
	// Only in campaign!
	if(!specialops)
	{
		specialops = Dvar_FindVar("specialops");
	}

	bool uncensored = false;
	for(int i = 0; i<count && !specialops->current.boolean; i++)
	{
		if(!strcmp(data[0].name, "airport"))
		{
			Dvar_SetCommand("friendlyfire_dev_disabled", "1");
			uncensored = true;
		}
	}

	if(!uncensored)
	{
		Dvar_SetCommand("friendlyfire_dev_disabled", "0");
	}
}

char returnPath[MAX_PATH];

char* addAlterZones(char* zone)
{
	if(GetFileAttributes(va( "zone\\alter\\%s", zone)) != INVALID_FILE_ATTRIBUTES)
	{
		strcpy(returnPath, "zone\\alter\\");
	}
	else if(GetFileAttributes(va("zone\\dlc\\%s", zone)) != INVALID_FILE_ATTRIBUTES)
	{
		strcpy(returnPath, "zone\\dlc\\");
	}
	else
	{
		sprintf(returnPath, "zone\\%s\\", language); // User's language
	}

	return returnPath;
}

void __cdecl loadTeamFile(XZoneInfo* data, int count, int unknown)
{
	// Still bugged. probably need to compile an own fastfile
	//data[count].name = "team_tf141";
	//data[count].type1 = 4;
	//data[count].type2 = 0;
	//count++;

	uncutGame(data, count);

	_allowZoneChange = true;

	DB_LoadXAssets(data, count, unknown);
}

static DWORD gameWorldSP;
static DWORD gameWorldMP;

void GetBSPNameHookFunc(char* buffer, size_t size, const char* format, const char* mapname)
{
	// the usual stuff
	if (!_strnicmp("mp_", mapname, 3))
	{
		format = "maps/mp/%s.d3dbsp";
	}

	_snprintf(buffer, size, format, mapname);

	// check for being MP/SP, and change data accordingly
	if (_strnicmp("mp_", mapname, 3))
	{
		if(version == 159)
		{
			// SP
			*(DWORD*)0x4B0921 = gameWorldSP + 4;		// some game data structure
		}
		else
		{
			// SP
			*(DWORD*)0x4D4AA1 = gameWorldSP + 4;		// some game data structure
		}
	}
	else
	{
		if(version == 159)
		{
			// MP
			*(DWORD*)0x4B0921 = gameWorldMP + 52;		// some game data structure
		}
		else
		{
			// MP
			*(DWORD*)0x4D4AA1 = gameWorldMP + 52;		// some game data structure
		}
	}
}

typedef struct  
{
	char unknown[16];
} xAssetEntry_t;

static xAssetEntry_t xEntries[789312];

void ReallocXAssetEntries()
{
	int newsize = 516 * 2048;
	//newEnts = malloc(newsize);

	// get (obfuscated) memory locations
	unsigned int origMin = 0xB2B360;
	unsigned int origMax = 0xB2B370;

	// scan the .text memory
	char* scanMin = (char*)0x401000;
	char* scanMax = (char*)0x694000;

	if(version == 184)
	{
		origMin = 0xB27DE0;
		origMax = 0xB27DF0;
		scanMax = (char*)0x691000;
	}

	unsigned int difference = (unsigned int)xEntries - origMin;

	char* current = scanMin;

	for (; current < scanMax; current += 1) {
		unsigned int* intCur = (unsigned int*)current;

		// if the address points to something within our range of interest
		if (*intCur == origMin || *intCur == origMax) {
			// patch it
			*intCur += difference;
		}
	}

	if(version == 159)
		*(DWORD*)0x581740 = 789312;
	else
		*(DWORD*)0x57EB40 = 789312;
}

CallHook ffLoadHook1;
DWORD _ffLoadHook1Loc = 0x506BC7;

void __cdecl FFLoadHook1(XZoneInfo* data, int count, int unknown) 
{
	data[0].type1 = 1;
	data[1].type1 = 1;

	// Adding DLC fastfiles
	data[count].name = "dlc1_ui_mp";
	data[count].type1 = 2;
	data[count].type2 = 0;
	count++;

	data[count].name = "dlc2_ui_mp";
	data[count].type1 = 2;
	data[count].type2 = 0;
	count++;

	return DB_LoadXAssets(data, count, unknown);
}

void __declspec(naked) FFLoadHook1Stub() {
	__asm {
		jmp FFLoadHook1
	}
}

CallHook uiLoadHook1;
DWORD uiLoadHook1Loc = 0x60B4AC;

void __cdecl UILoadHook1(XZoneInfo* data, int count, int unknown) {
	XZoneInfo newData[5];

	memcpy(&newData[0], data, sizeof(XZoneInfo) * 2);

	count = 0;

	// DLC fastfiles
	newData[0].name = "dlc1_ui_mp";
	newData[0].type1 = 3;
	newData[0].type2 = 0;
	count++;

	newData[1].name = "dlc2_ui_mp";
	newData[1].type1 = 3;
	newData[1].type2 = 0;
	count++;

	return DB_LoadXAssets(newData, count, unknown);
}

void __declspec(naked) UILoadHook1Stub() {
	__asm jmp UILoadHook1
}

// zone\dlc patches
CallHook zoneLoadHook1;
DWORD zoneLoadHook1Loc = 0x5BC82C;

CallHook zoneLoadHook2;
DWORD zoneLoadHook2Loc = 0x4CCE08;

char zone_language[MAX_PATH];
char* loadedPath = "";
char* zonePath = "";


dvar_t** fs_basepath = (dvar_t**)0x63D0CD4;
dvar_t** fs_cdpath = (dvar_t**)0x63D0BB0;

bool FileExists(const char* file)
{
	struct stat st;

	// note that this doesn't count any of the other ways stat could fail, but that'd be more error checking elsewhere
	if (stat(file, &st) >= 0)
	{
		return true;
	}

	return false;
}

char* GetZoneLocation(const char* name) {
	static char path[MAX_PATH];

	_snprintf(path, MAX_PATH, "%s\\zone\\%s\\%s.ff", (*fs_basepath)->current.string, "alter\\", name);

	if (FileExists(path)) {
		sprintf(path, "%s\\zone\\alter\\", (*fs_basepath)->current.string);
		return path;
	}

	if ((*fs_cdpath)->current.string[0])
	{
		_snprintf(path, MAX_PATH, "%s\\zone\\%s\\%s.ff", (*fs_cdpath)->current.string, "alter\\", name);

		if (FileExists(path)) {
			sprintf(path, "%s\\zone\\alter\\", (*fs_cdpath)->current.string);
			return path;
		}
	}

	_snprintf(path, MAX_PATH, "%s\\zone\\%s\\%s.ff", (*fs_basepath)->current.string, "dlc\\", name);

	if (FileExists(path)) {
		sprintf(path, "%s\\zone\\dlc\\", (*fs_basepath)->current.string);
		return path;
	}

	if ((*fs_cdpath)->current.string[0])
	{
		_snprintf(path, MAX_PATH, "%s\\zone\\%s\\%s.ff", (*fs_cdpath)->current.string, "dlc\\", name);

		if (FileExists(path)) {
			sprintf(path, "%s\\zone\\dlc\\", (*fs_cdpath)->current.string);
			return path;
		}
	}

	DWORD getLang = 0x45CBA0;
	char* language;

	__asm {
		call getLang
			mov language, eax
	}

	_snprintf(path, MAX_PATH, "%s\\zone\\%s\\%s.ff", (*fs_basepath)->current.string, language, name);

	if (FileExists(path)) {
		sprintf(path, "%s\\zone\\%s\\", (*fs_basepath)->current.string, language);
		return path;
	}

	if ((*fs_cdpath)->current.string[0])
	{
		_snprintf(path, MAX_PATH, "%s\\zone\\%s\\%s.ff", (*fs_cdpath)->current.string, language, name);

		if (FileExists(path)) {
			sprintf(path, "%s\\zone\\%s\\", (*fs_cdpath)->current.string, language);
			return path;
		}
	}

	return NULL;
}

char* GetZonePath(const char* fileName)
{
	// we do it a lot simpler than IW did.
	return GetZoneLocation(fileName);
}

void __declspec(naked) ZoneLoadHook1Stub() {
	__asm {
		mov loadedPath, esi // MAKE SURE TO EDIT THIS REGISTER FOR OTHER EXE VERSIONS
	}

	zonePath = GetZonePath(loadedPath);

	__asm {
		mov eax, zonePath
			retn
	}
}

void __declspec(naked) ZoneLoadHook2Stub() {
	__asm {
		mov loadedPath, eax
	}

	zonePath = GetZonePath(loadedPath);

	__asm {
		mov eax, zonePath
			retn
	}
}

// RAWFILE SUPPORT
// more functions
typedef void* (__cdecl * LoadModdableRawfile_t)(int a1, const char* filename);
LoadModdableRawfile_t LoadModdableRawfile = (LoadModdableRawfile_t)0x61ABC0;

// wrapper function for LoadModdableRawfile
void* __cdecl LoadModdableRawfileFunc(const char* filename, void* buffer, int bufSize) {
	// call LoadModdableRawfile
	// we should actually return buffer instead, but as we can't make LoadModdableRawfile load to a custom buffer
	// and code shouldn't use the 'buffer' parameter directly, this should work in most cases
	// even then, we can hardly properly memcpy to the buffer as we don't know the actual file buffer size
	return LoadModdableRawfile(0, filename);
}

void __declspec(naked) LoadModdableRawfileStub() {
	__asm jmp LoadModdableRawfileFunc
}

// wrapper function for LoadModdableRawfile
void* __cdecl LoadModdableRawfileFunc2(const char* filename) {
	// call LoadModdableRawfile
	// we should actually return buffer instead, but as we can't make LoadModdableRawfile load to a custom buffer
	// and code shouldn't use the 'buffer' parameter directly, this should work in most cases
	// even then, we can hardly properly memcpy to the buffer as we don't know the actual file buffer size
	return LoadModdableRawfile(0, filename);
}

void __declspec(naked) LoadModdableRawfileStub2() {
	__asm jmp LoadModdableRawfileFunc2
}

CallHook rawFileHook1;
DWORD rawFileHook1Loc = 0x632155;

CallHook rawFileHook2;
DWORD rawFileHook2Loc = 0x5FA46C;

CallHook rawFileHook3;
DWORD rawFileHook3Loc = 0x5FA4D6;

CallHook rawFileHook4;
DWORD rawFileHook4Loc = 0x6321EF;

CallHook rawFileHook5;
DWORD rawFileHook5Loc = 0x630A88;

CallHook rawFileHook6;
DWORD rawFileHook6Loc = 0x59A6F8;

CallHook rawFileHook7;
DWORD rawFileHook7Loc = 0x57F1E6;

CallHook rawFileHook8;
DWORD rawFileHook8Loc = 0x57ED36;

CallHook rawFileHook9;
DWORD rawFileHook9Loc = 0x4C77A2;

CallHook rawFileHook10;
DWORD rawFileHook10Loc = 0x4E46D1;

StompHook getZoneRootHook;
DWORD getZoneRootHookLoc = 0x4326E0;

const char* GetZoneRootHookFunc(const char* zoneName)
{
	char filename[260];

	if ((*fs_cdpath)->current.string[0])
	{
		_snprintf(filename, sizeof(filename) - 1, "%s\\%s", (*fs_cdpath)->current.string, zoneName);

		if (GetFileAttributes(filename) != INVALID_FILE_ATTRIBUTES)
		{
			return (*fs_cdpath)->current.string;
		}
	}

	return (*fs_basepath)->current.string;
}

CallHook zoneTestHook;
DWORD zoneTestHookLoc = 0x4CCE23;

void ZoneTestHookFunc(char* buffer, int length, const char* format, const char* absPath, const char* zonePath, const char* zoneName)
{
	_snprintf(buffer, length, "%s%s.ff", zonePath, zoneName);
}

CallHook zoneSprintfHook;
DWORD zoneSprintfHookLoc = 0x4B2F15;

void ZoneSprintfHookFunc(char* buffer, int length, const char* format, const char* absPath, const char* zonePath, const char* zoneName)
{
	_snprintf(buffer, length, "%s%s", zonePath, zoneName);
}

void PatchMW2_Load()
{
	if(version == 159)
	{
		// Ignore zone version missmatch
		*(BYTE*)0x4256D8 = 0xEB;

		// Ignore 'Disc read error.'
		nop(0x4B7335, 2);
		//*(BYTE*)0x4B7356 = 0xEB;
		//*(BYTE*)0x413629 = 0xEB;
		//*(BYTE*)0x581227 = 0xEB;
		*(BYTE*)0x4256B9 = 0xEB;

		gameWorldSP = (*(DWORD*)0x4B0921) - 4;
	}
	else if(version == 184)
	{
		// Ignore zone version missmatch
		*(BYTE*)0x4A4E98 = 0xEB;

		// Ignore 'Disc read error.'
		nop(0x4F6E05, 2);
		//*(BYTE*)0x4F6E26 = 0xEB;
		//*(BYTE*)0x47FE69 = 0xEB;
		//*(BYTE*)0x57E637 = 0xEB;
		*(BYTE*)0x4A4E79 = 0xEB;

		gameWorldSP = (*(DWORD*)0x4D4AA1) - 4;
	}

	if(version == 177)
	{
		// rawfile moddability
		rawFileHook1.initialize(rawFileHook1Loc, LoadModdableRawfileStub);
		rawFileHook1.installHook(); 

		rawFileHook2.initialize(rawFileHook2Loc, LoadModdableRawfileStub);
		rawFileHook2.installHook();

		rawFileHook3.initialize(rawFileHook3Loc, LoadModdableRawfileStub);
		rawFileHook3.installHook();

		rawFileHook4.initialize(rawFileHook4Loc, LoadModdableRawfileStub2);
		rawFileHook4.installHook();

		rawFileHook5.initialize(rawFileHook5Loc, LoadModdableRawfileStub);
		rawFileHook5.installHook();

		rawFileHook6.initialize(rawFileHook6Loc, LoadModdableRawfileStub);
		rawFileHook6.installHook();

		rawFileHook7.initialize(rawFileHook7Loc, LoadModdableRawfileStub);
		rawFileHook7.installHook();

		rawFileHook8.initialize(rawFileHook8Loc, LoadModdableRawfileStub);
		rawFileHook8.installHook();

		ffLoadHook1.initialize(_ffLoadHook1Loc, FFLoadHook1Stub);
		ffLoadHook1.installHook();

		uiLoadHook1.initialize(uiLoadHook1Loc, UILoadHook1Stub);
		uiLoadHook1.installHook();

		zoneLoadHook1.initialize(zoneLoadHook1Loc, ZoneLoadHook1Stub);
		zoneLoadHook1.installHook();

		zoneLoadHook2.initialize(zoneLoadHook2Loc, ZoneLoadHook2Stub);
		zoneLoadHook2.installHook();

		getZoneRootHook.initialize(getZoneRootHookLoc, GetZoneRootHookFunc);
		getZoneRootHook.installHook();

		zoneTestHook.initialize(zoneTestHookLoc, ZoneTestHookFunc);
		zoneTestHook.installHook();

		zoneSprintfHook.initialize(zoneSprintfHookLoc, ZoneSprintfHookFunc);
		zoneSprintfHook.installHook();

		if (strstr(GetCommandLine(), "fs_cdpath") != NULL)
		{
			// fs_basepath for 'players' -> fs_cdpath
			*(DWORD*)0x482408 = 0x63D0BB0;
		}

		// disable 'ignoring asset' notices
		memset((void*)0x5BB902, 0x90, 5);

		// ignore 'no iwd files found in main'
		memset((void*)0x642A4B, 0x90, 5);
	}
	else
	{
		gameWorldMP = (DWORD)ReallocateAssetPool(ASSET_TYPE_GAME_MAP_MP, 1);

		call(getBSPNameHookLoc, GetBSPNameHookFunc, PATCH_CALL);
		call(ffLoadHook1Loc, loadTeamFile, PATCH_CALL);
		call(zoneLoadHookLoc, addAlterZones, PATCH_CALL);
		ReallocXAssetEntries();
	}
}