// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: A simple replacement for the party-based server
//          handshake.
//
// Initial author: NTAuthority
// Started: 2013-04-02
// ==========================================================

#include "StdInc.h"

void PatchMW2_PartyBypass()
{
	// various changes to SV_DirectConnect-y stuff to allow non-party joinees
	*(WORD*)0x460D96 = 0x90E9;
	*(BYTE*)0x460F0A = 0xEB;
	*(BYTE*)0x401CA4 = 0xEB;
	*(BYTE*)0x401C15 = 0xEB;
}