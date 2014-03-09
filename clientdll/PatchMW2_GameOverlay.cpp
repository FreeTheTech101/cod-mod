// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Steam's GameOverlay
//
// Initial author: momo5502
// Started: 2014-03-09
// ==========================================================

#include "stdinc.h"

void loadGameOverlay()
{
	// Initialize renderer
	callVoid(initializeRenderer);

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

void PatchMW2_GameOverlay()
{
	call(loadGameOverlayHookLoc, loadGameOverlay, PATCH_CALL);
}