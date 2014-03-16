// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Providing the ability to earn 
//          achievements in non-steam version.
//
// Initial author: momo5502
// Started: 2014-03-16
// ==========================================================

#include "stdinc.h"

bool RewardArchievement( const char *pchName )
{
	Com_Printf(0, "Reward processing for '%s' queued!", pchName);
	return true;
}