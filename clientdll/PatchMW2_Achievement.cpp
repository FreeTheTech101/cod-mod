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
		playSound("arcademode_kill_streak_won");
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
	float descrColor[] = { 0.75f, 0.75f, 0.75f, 1.0f };
	void* font = R_RegisterFont("fonts/normalFont");
	void* material = DB_FindXAssetHeader(ASSET_TYPE_MATERIAL, "black");

	// Calculate dimension
	int width_1 = getScaledWidth(reward->rewardString, 1.0f, font);
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
	R_AddCmdDrawText(reward->rewardString, 0x7FFFFFFF, font, actualXOffset + border, yOffset + 39 - subHeight, 1.0f, 1.0f, 0.0f, titleColor, 0);
	R_AddCmdDrawText(reward->rewardDescription, 0x7FFFFFFF, font, actualXOffset + border, yOffset + 64 - subHeight, .8f, .8f, 0.0f, descrColor, 0);

	return true;
}

void processAchievement(int rewardCode)
{
	//Com_Printf(0, "Processing reward: '%s'\n", achievements[rewardCode].name);

	reward_t* reward = (reward_t*)malloc(sizeof(reward_t));
	reward->rewardCode = rewardCode;
	reward->handledOnce = false;

	char* title = (char*)malloc(200);
	char* description = (char*)malloc(200);

	sprintf(title, "Achievement unlocked: ^3%s", achievements[rewardCode].name);
	sprintf(description, "%s", achievements[rewardCode].description);

	reward->rewardDescription = description;
	reward->rewardString = title;
	
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