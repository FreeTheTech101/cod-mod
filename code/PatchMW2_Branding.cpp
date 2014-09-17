// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Branding
//
// Initial author: momo5502
// Started: 2014-03-04
// ==========================================================

#include "stdinc.h"
#include "ColorShift.h"
#include <playsoundapi.h>

bool printAchievements();

dvar_t* Dvar_RegisterBool_MW3(const char* name, int default, int flags);
int R_GetScaledWidth(const char* text, float sizeX, void* font);

CallHook drawDevStuffHook;
dvar_t* cl_paused;
dvar_t* cg_drawVersion;
dvar_t* cg_newColors;
dvar_MW3_t* cg_drawFPS;
HWND renderWindow;

bool progressWasLastPressed;
bool showprogress;
int startShowProgress;
void showProgress();

int lastFrame = 0;
int lastUpdate = 0;
int frames = 0;
char fpsText[20];

int getScreenWidth()
{
	RECT rect;

	if (GetWindowRect(renderWindow, &rect))
	{
		return rect.right - rect.left;
	}

	return 0;
}

HWND WINAPI CreateWindowExAHook(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	renderWindow = CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	return renderWindow;
}


void drawFps()
{
	if ((version != 358 && version != 382) || !cg_drawFPS || !cg_drawFPS->current.boolean || !CL_IsCgameInitialized())
		return;

	frames++;
	float font_size = 1.5f;
	void* font = R_RegisterFont("fonts/smalldevfont");

	if (Com_Milliseconds() - lastUpdate > 40)
	{
		int fps = 1000.0 / ((double)(Com_Milliseconds() - lastUpdate) / (double)frames);

		snprintf(fpsText, 20, "%d FPS", fps);
		lastUpdate = Com_Milliseconds();
		frames = 0;
	}

	int xOffset = 100;
	int yOffset = 100;
	int textWidth = R_GetScaledWidth(fpsText, font_size, font);

	int width = getScreenWidth() - (xOffset + textWidth);

	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	R_AddCmdDrawText(fpsText, 0x7FFFFFFF, font, width, yOffset, font_size, font_size, 0.0f, color, 0);
}

int printError;
errorParm_s printErrorType;
char* printErrorMessage;

void DrawDemoWarning()
{
	// Achievement stuff
	if(/*version > 358 && */!showprogress && !printAchievements())
	{
		if(GetKeyState(VK_F11) & 0x8000)
		{
			if(!progressWasLastPressed)
			{
				progressWasLastPressed = true;
				showprogress = true;
				startShowProgress = Com_Milliseconds();
			}
		}
		else
		{
			progressWasLastPressed = false;
		}
	}

	if(showprogress)
	{
		showProgress();
	}

	drawFps();

	// Branding stuff
	if(!(version >= 358 ? ((dvar_MW3_t*)cg_drawVersion)->current.boolean : cg_drawVersion->current.boolean))
		return;

	shiftColorHue();

	if(!cl_paused)
		cl_paused = Dvar_FindVar("cl_paused");

	// Reduce opacity ingame
	if(CL_IsCgameInitialized() && !(version >= 358 ? ((dvar_MW3_t*)cl_paused)->current.boolean : cl_paused->current.boolean))
	{
		color[3] = 0.3f;
	}

	void* font = R_RegisterFont("fonts/normalFont");
	R_AddCmdDrawText(VERSIONSTRING, 0x7FFFFFFF, font, 10, 30, 0.7f, 0.7f, 0.0f, color, 0);

	if(printError)
	{
		printError = false;
		Com_Error(printErrorType, printErrorMessage);
	}
}

DWORD getRGB(unsigned char r, unsigned char g, unsigned char b)
{
	char color[4];
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = 0xFF;

	return *(DWORD*)color;
}

int lastValue = 2;
DWORD default_color_table[8];
DWORD* color_table;

#define NEW_COLORS_VALUE (version >= 358 ? ((dvar_MW3_t*)cg_newColors)->current.boolean : cg_newColors->current.boolean)

void PatchColorTable()
{
	if(lastValue == 2)
	{
		memcpy(default_color_table, color_table, sizeof(default_color_table));
	}

	if(NEW_COLORS_VALUE && NEW_COLORS_VALUE != lastValue)
	{
		// Apply NTA's W² colors :3 (slightly modified though^^)
		color_table[1] = getRGB(255, 49, 49);
		color_table[2] = getRGB(134, 192, 0);
		color_table[3] = getRGB(255, 173, 34);
		color_table[4] = getRGB(0, 135, 193);
		color_table[5] = getRGB(32, 197, 255);
		color_table[6] = getRGB(151, 80, 221);

		lastValue = cg_newColors->current.boolean;
	}
	else if(!NEW_COLORS_VALUE && NEW_COLORS_VALUE != lastValue)
	{
		memcpy(color_table, default_color_table, sizeof(default_color_table));

		lastValue = NEW_COLORS_VALUE;
	}
}

void __declspec(naked) DrawDevStuffHookStub()
{
	__asm
	{
		call PatchColorTable
		call DrawDemoWarning
		jmp drawDevStuffHook.pOriginal
	}
}

void PatchMW2_Branding()
{
	if (version >= 358)
	{
		cg_drawVersion = Dvar_RegisterBool_MW3("cg_drawVersion", 1, DVAR_FLAG_SAVED);
		cg_newColors = Dvar_RegisterBool_MW3("cg_newColors", true, DVAR_FLAG_SAVED);
		cg_drawFPS = (dvar_MW3_t*)Dvar_RegisterBool_MW3("cg_drawFPS", 0, DVAR_FLAG_SAVED);

		if (version == 358)
		{
			call(0x65E57A, CreateWindowExAHook, PATCH_CALL);
			*(BYTE*)0x65E579 = 0x90;
		}
		else if (version == 382)
		{
			call(0x65F6BA, CreateWindowExAHook, PATCH_CALL);
			*(BYTE*)0x65F6B9 = 0x90;
		}
	}
	else
	{
		cg_drawVersion = Dvar_RegisterBool("cg_drawVersion", 1, DVAR_FLAG_SAVED, "Draw brandstring version.");
		cg_newColors = Dvar_RegisterBool("cg_newColors", true, DVAR_FLAG_SAVED, "Use Warfare² color code style.");
	}


	drawDevStuffHook.initialize(drawDevStuffHookLoc, DrawDevStuffHookStub);
	drawDevStuffHook.installHook();

	// Find color table
	color_table = (DWORD*)FindPattern(0x400000, 0x800000, (PBYTE)"\x00\x00\x00\xFF\xFF\x5C\x5C\xFF\x00\xFF\x00\xFF\xFF\xFF\x00\xFF\x00\x00\xFF\xFF\x00\xFF\xFF\xFF\xFF\x5C\xFF\xFF\xFF\xFF\xFF\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}