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

int xOffset = 10;
int yOffset = 31;

int displayTime = 7000;
int fadeTime = 300; // Milliseconds

char title[200];
char description[200];

int getScaledWidth(const char* text, float sizeX, void* font)
{
	int normalWidth = R_TextWidth(text, 0x7FFFFFFF, (Font*)font);
	double scaledWidth = normalWidth * sizeX;
	return (int)scaledWidth;
}

bool printAchievements()
{
	if(!cReward.rewardString)
		return false;

	if((Com_Milliseconds() - cReward.startTime) > displayTime)
		return false;

	float imageColor[] = { 1.0f, 1.0f, 1.0f, .6f };
	float textColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	void* font = R_RegisterFont("fonts/normalFont");
	void* material = DB_FindXAssetHeader(ASSET_TYPE_MATERIAL, "black");

	int width_1 = getScaledWidth(cReward.rewardString, 1.0f, font);
	int width_2 = getScaledWidth(cReward.rewardDescription, .8f, font);
	int totalWidth = (max(width_1, width_2) + (xOffset * 2));

	int subWidth = 0;
	int timeDiff = Com_Milliseconds() - cReward.startTime;

	// Fade-in
	if(timeDiff < fadeTime)
	{
		double rel = (double)timeDiff / (double)fadeTime; 
		subWidth = totalWidth - ((double)totalWidth * rel);
	}

	// Fade-out
	else if(timeDiff > displayTime - fadeTime)
	{
		double rel = (double)(timeDiff - (displayTime - fadeTime)) / (double)fadeTime; 
		subWidth = (double)totalWidth * rel;
	}

	R_AddCmdDrawStretchPic(-subWidth, yOffset, totalWidth, 74, 1.0f, 1.0f, 1.0f, 1.0f, imageColor, material);
	R_AddCmdDrawText(cReward.rewardString, 0x7FFFFFFF, font, xOffset - subWidth, yOffset + 39, 1.0f, 1.0f, 0.0f, textColor, 0);
	R_AddCmdDrawText(cReward.rewardDescription, 0x7FFFFFFF, font, xOffset - subWidth, yOffset + 64, .8f, .8f, 0.0f, textColor, 0);

	return true;
}

void processAchievement(int rewardCode)
{
	Com_Printf(0, "Processing reward: '%s'\n", achievements[rewardCode].name);
	// Do something cool here :D

	cReward.rewardCode = rewardCode;
	cReward.startTime = Com_Milliseconds();

	sprintf(title, "Achievement unlocked: ^3%s", achievements[rewardCode].name);
	sprintf(description, "^5%s", achievements[rewardCode].description);

	cReward.rewardDescription = description;
	cReward.rewardString = title;
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