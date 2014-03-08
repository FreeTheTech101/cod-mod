// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Patches for version 159
//
// Initial author: momo5502
// Started: 2014-02-15
// ==========================================================

#include "StdInc.h"
#include <time.h>
#include <dbghelp.h>
#include <WinSock2.h>

void PatchMW2_ClientConsole();
void PatchMW2_NoBorder();
void PatchMW2_SteamFriends();
void PatchMW2_Coop();
void PatchMW2_Branding();
void PatchMW2_New();
void PatchMW2_UILoading();
void PatchMW2_Minidump();
void PatchMW2_Images();
void PatchMW2_LocalizedStrings();
void PatchMW2_Load();

char ingameUsername[32];

char* GetUsername()
{
	if((BOOL)(*dvarName))
	{
		strncpy(ingameUsername, (*dvarName)->current.string, sizeof(ingameUsername));
	}
	else
	{
		strncpy(ingameUsername, "Unknown Player", sizeof(ingameUsername));
	}

	return ingameUsername;
}

hostent* WINAPI custom_gethostbyname(const char* name) {
	// if the name is IWNet's stuff...
	unsigned int ip1 = oneAtATimeHash("ip1.pc.iw4.iwnet.infinityward.com");
	unsigned int log1 = oneAtATimeHash("log1.pc.iw4.iwnet.infinityward.com");
	unsigned int match1 = oneAtATimeHash("match1.pc.iw4.iwnet.infinityward.com");
	unsigned int web1 = oneAtATimeHash("web1.pc.iw4.iwnet.infinityward.com");
	unsigned int blob1 = oneAtATimeHash("blob1.pc.iw4.iwnet.infinityward.com");

	unsigned int current = oneAtATimeHash((char*)name);
	char* hostname = (char*)name;

	if (current == log1 || current == match1 || current == blob1 || current == ip1 || current == web1)
	{
		hostname = "127.0.0.1";
	}

	return gethostbyname(hostname);
}

void loadGameOverlay()
{
	// Initialize renderer
	callVoid(0x48A400);

	// load steam gameoverlay
	if (GetModuleHandle("gameoverlayrenderer.dll") != NULL)
	{
		return;
	}

	try
	{
		std::string m_steamDir;
		HKEY hRegKey;

		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Valve\\Steam", 0, KEY_QUERY_VALUE, &hRegKey) == ERROR_SUCCESS)
		{
			char pchSteamDir[MAX_PATH];
			DWORD dwLength = sizeof(pchSteamDir);
			RegQueryValueExA(hRegKey, "InstallPath", NULL, NULL, (BYTE*)pchSteamDir, &dwLength);
			RegCloseKey(hRegKey);

			m_steamDir = pchSteamDir;
		}

		char path[MAX_PATH];
		sprintf(path, "%s\\gameoverlayrenderer.dll", m_steamDir);

		Com_Printf(0, "Loading %s...\n", path);
		LoadLibrary(path);
	}
	catch (int e)
	{
		Com_Printf(0, "Failed to inject Steam's gameoverlay!");
	}
}

void PatchMW2_159()
{
	version = 159;

	PatchMW2_Minidump();
	PatchMW2_SteamFriends();
	PatchMW2_Coop();
	PatchMW2_NoBorder();
	PatchMW2_ClientConsole();
	PatchMW2_Branding();
	PatchMW2_Images();
	PatchMW2_LocalizedStrings();
	PatchMW2_Load();
	PatchMW2_UILoading();

	// prevent stat loading from steam
	*(BYTE*)0x43FB33 = 0xC3;

	// remove limit on IWD file loading
	*(BYTE*)0x630FF3 = 0xEB;

	// remove fs_game check for moddable rawfiles - allows non-fs_game to modify rawfiles
	nop(0x612932, 2);

	// Ignore 'steam must be running' error
	nop(0x6040A3, 0x30);

	// Patch steam auth
	*(WORD*)0x43FAD5 = 0x15FF; // Prepare long call
	*(DWORD*)0x43FAD7 = 0x694554; // SteamAPI_init
	*(DWORD*)0x43FADB = 0x90C301B0; // mov al, 1 - retn

	// Remove dvar restrictions
	*(BYTE*)0x635841 = 0xEB; // read only
	*(BYTE*)0x635913 = 0xEB; // cheat protected
	*(BYTE*)0x6358A5 = 0xEB; // write protected
	*(BYTE*)0x635974 = 0xEB; // latched

	// Don't show intro
	*(BYTE*)0x6035BD = 0;
	*(BYTE*)0x6035BB = 0;

	// Flag cg_fov as saved
	*(BYTE*)0x41ED35 = DVAR_FLAG_SAVED;

	// Disable UPNP stuff
	*(WORD*)0x66DE7D = 0xE990;

	// No UPNP Output
	*(DWORD*)0x4D84D1 = (DWORD)"";

	// Open NAT
	*(DWORD*)0x7379F0 = 1;

	// Launch into mp
	//strcpy((char*)0x6C2D50, "iw4m.exe");
	//strcpy((char*)0x6CB090, "iw4m.exe");

	// Force debug logging
	nop(0x456BE5, 2);

	// No improper quit popup
	memset((void*)0x4F5B3A, 0x90, 2);

	// Force external console
	memset((void*)0x604071, 0x90, 21);

	// console '%s: %s> ' string
	*(DWORD*)0x579364 = (DWORD)(VERSIONSTRING_SP "> ");

	// console version string
	*(DWORD*)0x46F57B = (DWORD)(CONSOLESTRING);

	// version string
	*(DWORD*)0x60426F = (DWORD)(CONSOLESTRING);

	// Apply m2demo stuff
	*(DWORD*)0x631561 = (DWORD)"data";

	// Change external console title
	*(DWORD*)0x446A48 = (DWORD)"IW4SP: Console";

	*(DWORD*)0x6943A8 = (DWORD)custom_gethostbyname;

	// Yay, hitmarker in sp :D
	Dvar_RegisterBool("scr_damageFeedback", 0, DVAR_FLAG_SAVED, "Show marker when hitting enemies.");

	// Load steam game overlay
	call(0x604350, loadGameOverlay, PATCH_CALL);
}