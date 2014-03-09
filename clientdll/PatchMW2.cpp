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
void PatchMW3();

void Sys_Init()
{
	// Version 159
	if(!strcmp((char*)0x6A7DB8, "159"))
	{
		PatchMW2_159();
	}

	// Version 184
	else if(!strcmp((char*)0x6D2784, "184"))
	{
		PatchMW2_184();
	}

	// Test
	else if(*(DWORD*)0x5672B0 = 0xF324CBEB)
	{
		PatchMW3();
	}
}