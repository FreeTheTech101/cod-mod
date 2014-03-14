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
#include <winsock.h>

void PatchMW2_SteamFriends();
void PatchMW2_RunCallbacks();
void replaceFriendConnect();
void connectHook();

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

	PatchMW2_RunCallbacks();
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

	replaceFriendConnect();
	PatchMW2_SteamFriends();
	loadGameOverlay();
}

void checkSteamRun()
{
	// Initialize renderer
	callVoid(initializeRenderer);

	// Check if game got started using steam
	if (GetModuleHandle("gameoverlayrenderer.dll") == NULL)
	{
		// Patch steam stuff
		initializeSteamPatch();
	}
}

void PatchMW2_Steam()
{
	// Load steam gameoverlay
	call(loadGameOverlayHookLoc, checkSteamRun, PATCH_CALL);
}