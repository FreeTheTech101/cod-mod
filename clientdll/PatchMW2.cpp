// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Patches.
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

CallHook imageVersionCheckSPHook;
DWORD imageVersionCheckSPHookLoc = 0x544756;

void __declspec(naked) ImageVersionCheckSPHookFunc()
{
	__asm
	{
		cmp eax, 9
		je returnSafely

		jmp imageVersionCheckSPHook.pOriginal

	returnSafely:
		mov al, 1
		add esp, 18h
		retn
	}
}

CallHook winMainInitSPHook;
DWORD winMainInitSPHookLoc = 0x4A7910;

LONG WINAPI CustomUnhandledExceptionFilter(LPEXCEPTION_POINTERS ExceptionInfo);

void __declspec(naked) WinMainInitSPHookStub()
{
	SetUnhandledExceptionFilter(&CustomUnhandledExceptionFilter);

	__asm {
		jmp winMainInitSPHook.pOriginal
	}
}

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

localizedEntry_s* SEH_LocalizeAssetSP(int type, const char* ref)
{
	static localizedEntry_s entry;
	localizedEntry_s* dbEntry = (localizedEntry_s*)DB_FindXAssetHeader(type, ref);

	if (!dbEntry)
	{
		return dbEntry;
	}

	memcpy(&entry, dbEntry, sizeof(localizedEntry_s));

	if (!strcmp(entry.name, "MENU_INVITE_FRIEND"))
	{
		entry.value = "Quick connect";
	}
	else if (!strcmp(entry.name, "MENU_DESC_INVITE_FRIEND"))
	{
		entry.value = va("Reconnect to last player.");
	}
	else if (!strcmp(entry.name, "MENU_SP_STEAM_CHAT_HINT"))
	{
		entry.value = ""; // Leave that blank for now
	}

	return &entry;
}

char* addDLCZones(char* zone)
{
	char returnPath[14];
	char fastfile[128];

	sprintf(fastfile, "zone\\alter\\%s", zone); // Also add 'alter' zone path from aiw

	if(GetFileAttributes(fastfile) != INVALID_FILE_ATTRIBUTES)
	{
		strcpy(returnPath, "zone\\alter\\");
	}
	else
	{
		sprintf(fastfile, "zone\\dlc\\%s", zone);

		if(GetFileAttributes(fastfile) != INVALID_FILE_ATTRIBUTES)
		{
			strcpy(returnPath, "zone\\dlc\\");
		}
		else
		{
			sprintf(returnPath, "zone\\%s\\", (char*)0x19FF828); // User's language
		}
	}

	return returnPath;
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

void PatchMW2SP_Modifications()
{
	PatchMW2_SteamFriends();
	PatchMW2_Coop();
	PatchMW2_NoBorder();
	PatchMW2_ClientConsole();
	PatchMW2_Branding();

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
	*(WORD*)0x63587C = 0xE990; // read only
	*(WORD*)0x63594E = 0xE990; // cheat protected
	*(WORD*)0x6358E0 = 0xE990; // write protected
		
	// Ignore zone version missmatch
	*(BYTE*)0x4256D8 = 0xEB;

	// Support for alternative zones
	call(0x582F9C, addDLCZones, PATCH_CALL);

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
	*(DWORD*)0x46F57B = (DWORD)(VERSIONSTRING_SP " (built " __DATE__ " " __TIME__ ")");

	// version string
	*(DWORD*)0x60426F = (DWORD)(VERSIONSTRING_SP " (built " __DATE__ " " __TIME__ ")");

	// Apply m2demo stuff
	*(DWORD*)0x631561 = (DWORD)"data";

	// Change external console title
	*(DWORD*)0x446A48 = (DWORD)"IW4SP: Console";

	*(DWORD*)0x6943A8 = (DWORD)custom_gethostbyname;

	// Yay, hitmarker in sp :D
	Dvar_RegisterBool("scr_damageFeedback", 0, DVAR_FLAG_SAVED, "Show marker when hitting enemies.");

	imageVersionCheckSPHook.initialize(imageVersionCheckSPHookLoc, ImageVersionCheckSPHookFunc);
	imageVersionCheckSPHook.installHook();

	winMainInitSPHook.initialize(winMainInitSPHookLoc, WinMainInitSPHookStub);
	winMainInitSPHook.installHook();

	// Custom localized strings
	call(0x61BB17, SEH_LocalizeAssetSP, PATCH_CALL);

	// Load steam game overlay
	call(0x604350, loadGameOverlay, PATCH_CALL);
}

void Sys_Init()
{
	// Non-steam version
	if(*(DWORD*)0x41BFA3 == 0x91648166)
	{
		PatchMW2SP_Modifications();
	}

	// Latest Steam version
	else if(*(DWORD*)0x41BFA3 == 0x7501ED83)
	{
		PatchMW2_New();
	}

	else
	{
		MessageBox(0, "Your iw4sp.exe version is not supported. The game will now launch without any modifications. Send your iw4sp.exe to MauriceHeumann@gmail.com so that I can add support.", "Warning", MB_ICONEXCLAMATION);
	}

	if (GetProcAddress(GetModuleHandle("kernel32.dll"), "InitializeSRWLock"))
	{
		LoadLibrary("gdimm_32.dll");
	}
}