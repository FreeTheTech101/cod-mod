// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Version detection
//
// Initial author: momo5502
// Started: 2014-02-15
// ==========================================================

#include "StdInc.h"

void PatchMW2_159();
void PatchMW2_184();
void PatchMW3_358();
void PatchMW3_382();

void echoError(int version, char* game)
{
	MessageBox(0, va("The version of %s you're using (build %d) is not supported as of now!", game, version), "Warning", MB_ICONWARNING);
}

void Sys_Init()
{
	DetermineGameFlags();

	// --------------------------- MW2 ---------------------------

	// MW2 - Version 159
	if(!strcmp((char*)0x6A7DB8, "159"))
	{
		PatchMW2_159();
	}

	// MW2 - Version 184
	else if(!strcmp((char*)0x6D2784, "184"))
	{
		PatchMW2_184();
	}

	// --------------------------- MW3 ---------------------------

	// MW3 - Version 358
	else if(!strcmp((char*)0x7B9B6C, "358"))
	{
		PatchMW3_358();
	}

	// MW3 - Version 382
	else if(!strcmp((char*)0x7C65D8, "382"))
	{
		PatchMW3_382();
	}

	// MW3 - Version 446
	else if(*(int*)0x43A486 == 446)
	{
		echoError(446, "MW3");
	}

	// MW3 - Version 461
	else if(*(int*)0x50BBD6 == 461)
	{
		echoError(461, "MW3");
	}
}