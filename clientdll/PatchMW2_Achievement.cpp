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
int xoffSet = 0;

void printAchievements()
{
	if(!cReward.rewardString)
		return;

	if((Com_Milliseconds() - cReward.startTime) > 7000)
		return;

	float color[] = { 1.0f, 1.0f, 1.0f, .4f };
	void* font = R_RegisterFont("fonts/normalFont");
	void* material = DB_FindXAssetHeader(ASSET_TYPE_MATERIAL, "black");

	R_AddCmdDrawStretchPic(0, 31, 800, 74, 1.0f, 1.0f, 1.0f, 1.0f, color, material);

	color[3] = 1;

	R_AddCmdDrawText(cReward.rewardString, 0x7FFFFFFF, font, 10, 70, 1.0f, 1.0f, 0.0f, color, 0);
	R_AddCmdDrawText(cReward.rewardDescription, 0x7FFFFFFF, font, 10, 95, .8f, .8f, 0.0f, color, 0);
}

void processAchievement(int rewardCode)
{
	Com_Printf(0, "Processing reward: '%s'\n", achievements[rewardCode].name);
	// Do something cool here :D

	cReward.rewardCode = rewardCode;
	cReward.startTime = Com_Milliseconds();
	cReward.rewardDescription = va("^5%s", achievements[rewardCode].description);
	cReward.rewardString = va("Achievement unlocked: ^3%s", achievements[rewardCode].name);
	Cmd_ExecuteSingleCommand(0, 0, "snd_playlocal arcademode_kill_streak_won"); // Sound is ok for now
}

// Very simple way of storing achievements. But who cares.
void setAsEarned( const char *pchName )
{
	// Neither is this zip file, nor does it contain any settings, but it looks authentic ;)
	FILE* rewardFile = fopen("players/settings_a.zip.iw4", "a+"); // a or a+ ?

	if(rewardFile)
	{
		fprintf(rewardFile, "%s\n", pchName);
		fclose(rewardFile);
	}
}

bool hasAlreadyEarnedReward( const char *pchName )
{
	FILE* rewardFile = fopen("players/settings_a.zip.iw4", "r");
	if(rewardFile)
	{
		fseek (rewardFile, 0, SEEK_END);
		long length = ftell(rewardFile);
		rewind (rewardFile);
		char* buffer = (char*)malloc(length);

		if (!buffer)
			return false;

		fread (buffer, 1, length, rewardFile); // Just assume it reads everything correctly...
		fclose(rewardFile);

		std::string contentBuffer = buffer;

		return (contentBuffer.find(pchName) != -1);
	}

	return false;
}

bool RewardAchievement( const char *pchName )
{
	if(hasAlreadyEarnedReward(pchName))
		return true;

	buildAchievementList();

	for(int i = 0;i<ACHIEVEMENT_COUNT;i++)
	{
		if(!strcmp(achievements[i].code, pchName))
		{
			processAchievement(i);
			setAsEarned(pchName);
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
		FILE * rewardFile = fopen("players/settings_a.zip.iw4", "w");

		if(rewardFile)
		{
			fclose(rewardFile);
			return true;
		}
	}
	catch(int e){} // Ignore this for now...

	return false;
}

void giveTestA()
{
	int r = 10;

	if(Cmd_Argc() == 2)
		r = atoi(Cmd_Argv(1));

	buildAchievementList();
	processAchievement(r);
}

void PatchMW2_AchievementTest()
{
	return; // Nope
	static cmd_function_t achievementTest_cmd;
	Cmd_AddCommand("giveA", giveTestA, &achievementTest_cmd, 0);
}