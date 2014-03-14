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

	if(GetFileAttributes(va( "zone\\alterSP\\%s", zonePtr)) != INVALID_FILE_ATTRIBUTES)
	{
		strcpy(__returnPath, "zone\\alterSP\\");
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

void PatchMW3_358()
{
	version = 358;

	Dvar_FindVar = (Dvar_FindVar_t)0x4755D0;
	Com_Error = (Com_Error_t)0x472BC0;

	// nop improper quit pop up
	memset((void*)0x4E18EA, 0x90, 5);

	// no startup cinematics
	memset((void*)0x5EE851, 0x90, 5);

	// Fix leaderboarddefinition stuff
	ReallocateAssetPool(41, 228);

	// m2demo ;)
	*(DWORD*)0x60E286 = (DWORD)"data";

	*(DWORD*)0x41890C = (DWORD)_patchDevmap; // Hook devmap call to fix spec ops bug
	call(0x56DCED, _addDLCZones, PATCH_CALL);

	// Ignore 'steam must be running' error
	*(BYTE*)0x5EE5F1 = 0xEB;

	// Patch steam auth
	nop(0x542525, 7);
	*(BYTE*)0x54252C = 0xEB;
	*(BYTE*)0x542531 = 0x90;
	call(0x542532, _steamInitPatchMW3, PATCH_JUMP);

	// SteamApps
	*(DWORD*)0x48E050 = 0x90C301B0; // mov al, 1 - retn - nop
}