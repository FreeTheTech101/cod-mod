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
#include "rewardSound.h"
#include <playsoundapi.h>

reward_t* cRewards[100];
int cReward = 0;

int xOffset = 0; // From right
int yOffset = 0; // From top
int border = 10;
int height = 74;

int displayTime = 7000;
int fadeTime = 300;
int delay = 500;

int getScaledWidth(const char* text, float sizeX, void* font)
{
	if(!R_TextWidth)
	{
		return 0;
	}

	int normalWidth = R_TextWidth(text, 0x7FFFFFFF, (Font*)font);
	double scaledWidth = normalWidth * sizeX;
	return (int)scaledWidth;
}

void pushQueue(reward_t* reward)
{
	cRewards[cReward++] = reward;
}

void popQueue()
{
	for(int i = 1;i<cReward;i++)
	{
		cRewards[i-1] = cRewards[i];
	}

	if(cReward)
	{
		cReward--;
	}
}

reward_t* currentQueue()
{
	if(!cReward)
	{
		return NULL;
	}

	return cRewards[0];
}

bool printAchievements()
{
	reward_t* reward = currentQueue();

	// Check for reward in queue
	if(!reward)
	{
		return false;
	}

	// Play sound
	if(!reward->handledOnce)
	{
		reward->startTime = Com_Milliseconds();
		reward->handledOnce = true;
		PlaySound(rewardSound, GetModuleHandle(0), SND_MEMORY | SND_ASYNC);
	}

	// Check if reward is valid
	if((Com_Milliseconds() - reward->startTime) > displayTime + delay)
	{
		popQueue();
		return false;
	}

	// Prepare display stuff
	float imageColor[] = { 1.0f, 1.0f, 1.0f, 0.8f };
	float titleColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float descrColor[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	float prefixColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	switch(reward->difficulty)
	{
	case bronze:
		titleColor[0] = 0.68f;
		titleColor[1] = 0.5f;
		titleColor[2] = 0.26f;
		break;

	case silver:
		titleColor[0] = 0.75f;
		titleColor[1] = 0.75f;
		titleColor[2] = 0.75f;
		break;

	case gold:
		titleColor[0] = 1.0f;
		titleColor[1] = 0.84f;
		titleColor[2] = 0;
		break;

	case platinum:
		titleColor[0] = 0;
		titleColor[1] = 0.87f;
		titleColor[2] = 1.0f;
		break;
	}

	void* font = R_RegisterFont("fonts/normalFont");
	void* material = DB_FindXAssetHeader(ASSET_TYPE_MATERIAL, "black");
	char* prefix = "Achievement unlocked: ";

	// Calculate dimension
	int prefixW = getScaledWidth(prefix, 1.0f, font);
	int width_1 = getScaledWidth(reward->rewardTitle, 1.0f, font) + prefixW;
	int width_2 = getScaledWidth(reward->rewardDescription, .8f, font);
	int totalWidth = (max(width_1, width_2) + (border * 2));

	char* screenDim = Dvar_GetString(*r_mode); // Get r_mode value
	int screenWidth, screenHeight; // Height is unnecessary for now
	sscanf_s(screenDim, "%dx%d", &screenWidth, &screenHeight);
	int actualXOffset = screenWidth - (totalWidth + xOffset);

	int subHeight = 0;
	int timeDiff = Com_Milliseconds() - reward->startTime;

	// Fade-in
	if(timeDiff < fadeTime)
	{
		double rel = (double)timeDiff / (double)fadeTime; 
		subHeight = height - ((double)height * rel);
	}

	// Fade-out
	else if(timeDiff > displayTime - fadeTime)
	{
		double rel = (double)(timeDiff - (displayTime - fadeTime)) / (double)fadeTime; 
		subHeight = (double)height * rel;
	}

	// Draw stuff
	R_AddCmdDrawStretchPic(actualXOffset, yOffset - subHeight, totalWidth, height, 1.0f, 1.0f, 1.0f, 1.0f, imageColor, material);
	R_AddCmdDrawText(prefix, 0x7FFFFFFF, font, actualXOffset + border, yOffset + 39 - subHeight, 1.0f, 1.0f, 0.0f, prefixColor, 0);
	R_AddCmdDrawText(reward->rewardTitle, 0x7FFFFFFF, font, actualXOffset + border + prefixW, yOffset + 39 - subHeight, 1.0f, 1.0f, 0.0f, titleColor, 0);
	R_AddCmdDrawText(reward->rewardDescription, 0x7FFFFFFF, font, actualXOffset + border, yOffset + 64 - subHeight, .8f, .8f, 0.0f, descrColor, 0);

	return true;
}

void processAchievement(int rewardCode)
{
	reward_t* reward = (reward_t*)malloc(sizeof(reward_t));
	reward->rewardCode = rewardCode;
	reward->handledOnce = false;
	reward->difficulty = achievements[rewardCode].difficulty;
	reward->rewardTitle = achievements[rewardCode].name;
	reward->rewardDescription = achievements[rewardCode].description;
	pushQueue(reward);
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

		bool result = (contentBuffer.find(pchName) != -1);
		free(buffer);
		return result;
	}

	return false;
}

void checkPlatinum()
{
	for(int i = 0;i<ACHIEVEMENT_COUNT;i++)
	{
		if(!hasAlreadyEarnedReward(achievements[i].code))
		{
			return;
		}
	}
	processAchievement(ACHIEVEMENT_COUNT);
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
			checkPlatinum();
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
	dvar_t* sv_cheats = Dvar_FindVar("sv_cheats");

	if(!sv_cheats->current.boolean)
		return;

	int r = 10;
	bool all = false;

	if(Cmd_Argc() == 2)
	{
		if(!strcmp(Cmd_Argv(1), "all"))
		{
			all = true;
		}
		else
		{
			r = atoi(Cmd_Argv(1));
		}
	}

	buildAchievementList();

	for(int i = (all ? 0 : r);i<(all ? ACHIEVEMENT_COUNT : (r+1));i++)
	{
		G_GiveAchievement(achievements[i].code);
	}
}

void PatchMW2_AchievementTest()
{
	static cmd_function_t achievementTest_cmd;
	Cmd_AddCommand("give_achievement", giveTestA, &achievementTest_cmd, 0);
}