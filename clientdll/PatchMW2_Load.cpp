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

char* addAlterSPZones(char* zone)
{
	char returnPath[14];
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

void PatchMW2_Load()
{
	if(version == 159)
	{
		// Ignore zone version missmatch
		*(BYTE*)0x4256D8 = 0xEB;
	}
	else if(version == 184)
	{
		// Ignore zone version missmatch
		*(BYTE*)0x4A4E98 = 0xEB;
	}

	call(zoneLoadHookLoc, addAlterSPZones, PATCH_CALL);
}