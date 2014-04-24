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

extern bool showprogress;
extern int startShowProgress;
reward_t* cRewards[100];
int cReward = 0;

int xOffset = 0; // From right
int yOffset = 0; // From top
int border = 10;
int height = 74;

int displayTime = 7000;
int fadeTime = 300;
int delay = 500;

float* rgba(int r, int g, int b, float a)
{
	float* color = (float*)malloc(sizeof(float) * 4);
	color[0] = r / 255.0;
	color[1] = g / 255.0;
	color[2] = b / 255.0;
	color[3] = a;
	return color;
}

void R_AddCmdDrawBoxShadow(float x, float y, float w, float h, float spread, float blur, const float* color)
{
	// Will do this later :D
}

void R_AddCmdDrawGradient(float x, float y, float w, float h, const float *color_top, const float *color_bottom)
{
	void* material = DB_FindXAssetHeader(ASSET_TYPE_MATERIAL, "white");

	for(int i = 0;i<h;i++)
	{
		float color[] = { 0, 0, 0, 0 };

		color[0] = (((h - i) * color_top[0]) + (i * color_bottom[0])) / h;
		color[1] = (((h - i) * color_top[1]) + (i * color_bottom[1])) / h;
		color[2] = (((h - i) * color_top[2]) + (i * color_bottom[2])) / h;
		color[3] = (((h - i) * color_top[3]) + (i * color_bottom[3])) / h;

		R_AddCmdDrawStretchPic(x, y + i, w, 1, 1.0f, 1.0f, 1.0f, 1.0f, color, material);
	}
}

void R_AddCmdDrawBorder(int x, int y, int w, int h, int size, const float *color, bool inset)
{
	void* material = DB_FindXAssetHeader(ASSET_TYPE_MATERIAL, "white");

	R_AddCmdDrawStretchPic(x - (inset ? 0 : size), y - (inset ? 0 : size), w + (inset ? 0 : (size * 2)), size, 1.0f, 1.0f, 1.0f, 1.0f, color, material); // Top: Left to Right
	R_AddCmdDrawStretchPic(x - (inset ? 0 : size), y + (inset ? size : 0), size, h - (inset ? (size * 2) : 0), 1.0f, 1.0f, 1.0f, 1.0f, color, material); // Left: Top to Bottom
	R_AddCmdDrawStretchPic(x - (inset ? 0 : size), y + h - (inset ? size : 0), w + (inset ? 0 : (size * 2)), size, 1.0f, 1.0f, 1.0f, 1.0f, color, material); // Bottom: Left to Right
	R_AddCmdDrawStretchPic(x + w - (inset ? size : 0), y + (inset ? size : 0), size, h - (inset ? (size * 2) : 0), 1.0f, 1.0f, 1.0f, 1.0f, color, material); // Right: Top to Bottom
}

int R_GetScaledWidth(const char* text, float sizeX, void* font)
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
		return true;
	}

	// Prepare display stuff
	float imageColor[] = { 1.0f, 1.0f, 1.0f, 0.8f };
	float* titleColor;
	float descrColor[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	float prefixColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	switch(reward->difficulty)
	{
		case bronze:
			titleColor = rgba(255, 228, 181, 1.0f);
			break;

		case silver:
			titleColor = rgba(211, 211, 211, 1.0f);
			break;

		case gold:
			titleColor = rgba(255, 215, 0, 1.0f);
			break;

		case platinum:
			titleColor = rgba(0, 255, 255, 1.0f);
			break;
	}

	void* font = R_RegisterFont("fonts/normalFont");
	void* material = DB_FindXAssetHeader(ASSET_TYPE_MATERIAL, "black");
	char* prefix = "Achievement unlocked: ";

	// Calculate dimension
	int prefixW = R_GetScaledWidth(prefix, 1.0f, font);
	int width_1 = R_GetScaledWidth(reward->rewardTitle, 1.0f, font) + prefixW;
	int width_2 = R_GetScaledWidth(reward->rewardDescription, .8f, font);
	int totalWidth = (max(width_1, width_2) + (border * 2));

	char* screenDim = Dvar_GetString(*r_mode); // Get r_mode value
	int screenWidth, screenHeight; // Height is unnecessary for now
	sscanf_s(screenDim, "%dx%d", &screenWidth, &screenHeight);

	if(version == 358)
	{
		screenWidth = *(int*)0x20E9380;
	}
	else if(version == 382)
	{
		screenWidth = *(int*)0x20ECD00;
	}

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
	float* top = rgba(89,89,89, 1.0f);
	float* bottom = rgba(14, 14, 14, 1.0f);
	float* borderC = rgba(0, 122, 204, 1.0f);

	R_AddCmdDrawGradient(actualXOffset, yOffset - subHeight, totalWidth, height, top, bottom);
	R_AddCmdDrawBorder(actualXOffset, yOffset - subHeight, totalWidth, height, 1, borderC, true);
	R_AddCmdDrawText(prefix, 0x7FFFFFFF, font, actualXOffset + border, yOffset + 39 - subHeight, 1.0f, 1.0f, 0.0f, prefixColor, 0);
	R_AddCmdDrawText(reward->rewardTitle, 0x7FFFFFFF, font, actualXOffset + border + prefixW, yOffset + 39 - subHeight, 1.0f, 1.0f, 0.0f, titleColor, 0);
	R_AddCmdDrawText(reward->rewardDescription, 0x7FFFFFFF, font, actualXOffset + border, yOffset + 64 - subHeight, .8f, .8f, 0.0f, descrColor, 0);

	return true;
}

int getTrophyValue(diffculty_s diff)
{
	switch(diff)
	{
		case bronze:
			return 1;

		case silver:
			return 2;

		case gold:
			return 3;

		// Platinum doesn't have a value, as you can't actively achieve it.
		default:
			return 0;
	}
}

char progressText[25];
int firstPopup;
int percent;
int progressDisplayTime = 3500;

bool hasAlreadyEarnedReward( const char *pchName );

void showProgress()
{
	if((Com_Milliseconds() - startShowProgress) > progressDisplayTime + 10)
	{
		showprogress = false;
		firstPopup = 0;
		return;
	}

	if(!firstPopup)
	{
		PlaySound(rewardSound, GetModuleHandle(0), SND_MEMORY | SND_ASYNC);
		firstPopup = 1;

		int totalValue = 0;
		int playerValue = 0;

		for(int i = 0;i<ACHIEVEMENT_COUNT;i++)
		{
			totalValue += getTrophyValue(achievements[i].difficulty);
			if(hasAlreadyEarnedReward(achievements[i].code))
			{
				playerValue += getTrophyValue(achievements[i].difficulty);
			}
		}

		percent = (100 / (double)totalValue) * (double)playerValue;
	}

	int width = 400;

	sprintf(progressText, "Achievement Progress: %d%s", percent, "%");

	float titleColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	void* font = R_RegisterFont("fonts/normalFont");
	void* material = DB_FindXAssetHeader(ASSET_TYPE_MATERIAL, "black");
	void* white = DB_FindXAssetHeader(ASSET_TYPE_MATERIAL, "white");

	int textW = R_GetScaledWidth(progressText, 1.0f, font);
	int totalWidth = max(textW + (border * 2), width);

	char* screenDim = Dvar_GetString(*r_mode); // Get r_mode value
	int screenWidth, screenHeight; // Height is unnecessary for now
	sscanf_s(screenDim, "%dx%d", &screenWidth, &screenHeight);

	if(version == 358)
	{
		screenWidth = *(int*)0x20E9380;
	}
	else if(version == 382)
	{
		screenWidth = *(int*)0x20ECD00;
	}

	int actualXOffset = screenWidth - (totalWidth + xOffset);

	int subHeight = 0;
	int subWidth = 0;
	int timeDiff = Com_Milliseconds() - startShowProgress;

	// Fade-in
	if(timeDiff < fadeTime)
	{
		double rel = (double)timeDiff / (double)fadeTime; 
		subHeight = height - ((double)height * rel);
	}

	// Fade-out
	else if(timeDiff > progressDisplayTime - fadeTime)
	{
		double rel = (double)(timeDiff - (progressDisplayTime - fadeTime)) / (double)fadeTime; 
		//subWidth = -((double)width * rel); // Slide out of screen
		subHeight = ((double)height * rel);
	}

	float* top = rgba(89,89,89, 1.0f);
	float* bottom = rgba(14, 14, 14, 1.0f);
	float* borderC = rgba(0, 122, 204, 1.0f);

	R_AddCmdDrawGradient(actualXOffset - subWidth, yOffset - subHeight, totalWidth, height, top, bottom);
	R_AddCmdDrawBorder(actualXOffset - subWidth,yOffset - subHeight, totalWidth, height, 1, borderC, true);
	R_AddCmdDrawText(progressText, 0x7FFFFFFF, font, actualXOffset + border - subWidth, yOffset + 39 - subHeight, 1.0f, 1.0f, 0.0f, titleColor, 0);

	// Progressbar :D
	int bar1_width = totalWidth - (border * 2);
	int bar1_height = 20;
	int bar1_yOffset = yOffset - subHeight + height - bar1_height - border;

	int bar_border = 0;
	int bar2_width = totalWidth - (border * 2) - (bar_border * 2); // Max progress width
	int bar2_height = 20 - (bar_border * 2);
	bar2_width = ((double)bar2_width / (double)100) * (double)percent; // Current progress

	// Fade progress in :P
// 	if(timeDiff < (fadeTime*2))
// 	{
// 		bar2_width = ((double)bar2_width / (double)(fadeTime*2)) * (double)timeDiff;
// 	}

	float* barbg_top = rgba(163, 163, 163, 1.0f);
	float* barbg_bottom = rgba(255, 255, 255, 1.0f);
	R_AddCmdDrawGradient(actualXOffset + border - subWidth, bar1_yOffset, bar1_width, bar1_height, barbg_top, barbg_bottom);

	float* bar_top = rgba(0, 199, 220, 1.0f);
	float* bar_bottom = rgba(15, 120, 156, 1.0f);
	R_AddCmdDrawGradient(actualXOffset + border + bar_border - subWidth, bar1_yOffset + bar_border, bar2_width, bar2_height, bar_top, bar_bottom);
	R_AddCmdDrawBorder(actualXOffset + border - subWidth, bar1_yOffset, bar1_width, bar1_height, 1, rgba(0, 0, 0, 1.0f), false);
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

		if(!buffer)
		{
			return 0;
		}

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

void give_achievement_f()
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

	for(int i = (all ? 0 : r);i<(all ? ACHIEVEMENT_COUNT : (r+1));i++)
	{
		G_GiveAchievement(achievements[i].code);
	}
}

void PatchMW2_Achievement()
{
	buildAchievementList();

	static cmd_function_t give_achievement_cmd;
	Cmd_AddCommand("give_achievement", give_achievement_f, &give_achievement_cmd, 0);
}