// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Steam patches
//
// Initial author: momo5502
// Started: 2014-03-09
// ==========================================================

#include "stdinc.h"

void PatchMW2_SteamFriends();
void PatchMW2_RunCallbacks();
void PatchMW2_SteamUserStats();
void PatchMW2_SteamMatchmaking();
void replaceLocalizedStrings();
void connectHook();

ISteamUser* __cdecl SteamUser();

hostent* WINAPI custom_gethostbyname(const char* name) {
	// if the name is IWNet's stuff...
	unsigned int ip1 = oneAtATimeHash("ip1.pc.iw4.iwnet.infinityward.com");
	unsigned int log1 = oneAtATimeHash("log1.pc.iw4.iwnet.infinityward.com");
	unsigned int match1 = oneAtATimeHash("match1.pc.iw4.iwnet.infinityward.com");
	unsigned int web1 = oneAtATimeHash("web1.pc.iw4.iwnet.infinityward.com");
	unsigned int blob1 = oneAtATimeHash("blob1.pc.iw4.iwnet.infinityward.com");

	unsigned int _ip1 = oneAtATimeHash("mw3-pc-auth.prod.demonware.net");
	unsigned int _log1 = oneAtATimeHash("mw3-pc-lobby.prod.demonware.net");

	unsigned int current = oneAtATimeHash((char*)name);
	char* hostname = (char*)name;

	if (current == log1 || current == match1 || current == blob1 || current == ip1 || current == web1 || current == _ip1 || current == _log1)
	{
		hostname = "127.0.0.1";
	}

	return gethostbyname(hostname);
}

void steamPatches_159()
{
// 	// Disable UPNP stuff
// 	*(WORD*)0x66DE7D = 0xE990;
// 
// 	// No UPNP Output
// 	*(DWORD*)0x4D84D1 = (DWORD)"";
// 
// 	// Open NAT
// 	*(DWORD*)0x7379F0 = 1;

	// Replace 'xshowfriendslist'
	*(DWORD*)0x57D7F8 = (DWORD)connectHook;

	*(DWORD*)0x6943A8 = (DWORD)custom_gethostbyname;

	// Don't make the game believe we're steam!
	nop(0x4E9458, 7);
	nop(0x4E9470, 7);
}

void steamPatches_184()
{
// 	// Disable UPNP stuff
// 	*(WORD*)0x66B5BD = 0xE990;
// 
// 	// No UPNP Output
// 	*(DWORD*)0x4B9E46 = (DWORD)"";
// 
// 	// Open NAT
// 	*(DWORD*)0x7349F0 = 1;

	// Replace 'xshowfriendslist'
	*(DWORD*)0x57AB68 = (DWORD)connectHook;

	*(DWORD*)0x6913A0 = (DWORD)custom_gethostbyname;

	// Don't make the game believe we're steam!
	nop(0x4C5958, 7);
	nop(0x4C5970, 7);

	PatchMW2_RunCallbacks();
}

void _strncpy_hook(char* name_buffer, char* no_name, size_t size);

void steamPatches_358()
{
	*(DWORD*)0x790420 = (DWORD)custom_gethostbyname;

	*(DWORD*)0x7905DC = (DWORD)SteamUser;

	call(0x4F70DF, _strncpy_hook, PATCH_CALL);
	*(WORD*)0x4F70CF = 0x9090; // Force playername stuff to go to strncpy hook to allow colored names
}

void steamPatches_382()
{
	*(DWORD*)0x791420 = (DWORD)custom_gethostbyname;

	call(0x5236C3, _strncpy_hook, PATCH_CALL);
}

void loadGameOverlay()
{
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

		Com_Printf(0, "Loading %s\\gameoverlayrenderer.dll...\n", m_steamDir.c_str());
		LoadLibrary(va("%s\\gameoverlayrenderer.dll", m_steamDir.c_str()));
	}
	catch (int e)
	{
		Com_Printf(0, "Failed to inject Steam's gameoverlay: %d", e);
	}
}

void initializeSteamPatch()
{
	if(version == 159)
	{
		steamPatches_159();
	}
	else if(version == 184)
	{
		steamPatches_184();
	}
	else if (version == 358)
	{
		steamPatches_358();
	}
	else if (version == 382)
	{
		steamPatches_382();
	}

	if(version == 177)
	{
		PatchMW2_SteamMatchmaking();
		PatchMW2_RunCallbacks();
		*(DWORD*)0x6D7458 = (DWORD)custom_gethostbyname;
	}
	else if (version < 358)
	{
		replaceLocalizedStrings();
		PatchMW2_SteamUserStats();
	}

	PatchMW2_SteamFriends();
	loadGameOverlay();
}

void buildAchievementList();
int isSteamInstace;

void checkSteamRun()
{
	// Initialize renderer
	callVoid(initializeRenderer);

	// Check if game got started using steam
	if (!GetModuleHandle("gameoverlayrenderer.dll"))
	{
		// Patch steam stuff
		isSteamInstace = 0;
		initializeSteamPatch();
	}
	else
	{
		isSteamInstace = 1;
		buildAchievementList();
	}
}

void PatchMW2_Steam()
{
	// Load steam gameoverlay
	call(loadGameOverlayHookLoc, checkSteamRun, PATCH_CALL);
}