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

reward_t cReward;

void printAchievements()
{
	if(!cReward.rewardString)
		return;

	if((Com_Milliseconds() - cReward.startTime) > 7000)
		return;

	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	void* font = R_RegisterFont("fonts/normalFont");

	R_AddCmdDrawText(cReward.rewardString, 0x7FFFFFFF, font, 10, 70, 0.7f, 0.7f, 0.0f, color, 0);
}

void processAchievement(int rewardCode)
{
	Com_Printf(0, "Processing reward: '%s'\n", achievements[rewardCode].name);
	// Do something cool here :D

	cReward.rewardCode = rewardCode;
	cReward.startTime = Com_Milliseconds();
	cReward.rewardString = va("Achievement unlocked: ^3%s",achievements[rewardCode].name);
	Cmd_ExecuteSingleCommand(0, 0, "snd_playlocal mouse_click"); // I don't have any better sound yet :( - Probably load trainer on startup to get flag_spawned equivalent
}

bool RewardAchievement( const char *pchName )
{
	buildAchievementList();

	for(int i = 0;i<ACHIEVEMENT_COUNT;i++)
	{
		if(!strcmp(achievements[i].code, pchName))
		{
			processAchievement(i);
			return true;
		}
	}

	Com_Printf(0, "Invalid reward: %s\n", pchName);
	return false;
}

bool resetAchievements()
{
	try
	{
		return true;
	}
	catch (int e)
	{
		return false;
	}
}