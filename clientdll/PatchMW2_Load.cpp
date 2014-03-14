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

dvar_t* specialops;

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

char* addAlterSPZones(char* zone)
{
	char fastfile[128];

	sprintf(fastfile, "zone\\alterSP\\%s", zone); // Add 'alterSP' zone path

	if(GetFileAttributes(fastfile) != INVALID_FILE_ATTRIBUTES)
	{
		strcpy(returnPath, "zone\\alterSP\\");
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

void PatchMW2_Load()
{
	if(version == 159)
	{
		// Ignore zone version missmatch
		*(BYTE*)0x4256D8 = 0xEB;

		// Build os path stuff
		*(BYTE*)0x6300BF = 0xEB;
	}
	else if(version == 184)
	{
		// Ignore zone version missmatch
		*(BYTE*)0x4A4E98 = 0xEB;
	}

	call(zoneLoadHookLoc, addAlterSPZones, PATCH_CALL);

	call(ffLoadHook1Loc, loadTeamFile, PATCH_CALL);
}