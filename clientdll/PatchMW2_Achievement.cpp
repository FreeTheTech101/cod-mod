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
#include "achievements.h"

void processAchievement( const char *pchName )
{
	Com_Printf(0, "Processing reward: %s\n", pchName);
	// Do something cool here :D
}

bool RewardAchievement( const char *pchName )
{
	// 51 achievements afaik...
	for(int i = 0;i<51;i++)
	{
		if(!strcmp(achievements[i], pchName))
		{
			processAchievement(pchName);
			return true;
		}
	}

	Com_Printf(0, "Invalid reward: %s\n", pchName);
	return false;
}