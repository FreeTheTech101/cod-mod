// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Fix damaged UI Script due to uiscript hook
//
// Initial author: momo5502
// Started: 2014-03-22
// ==========================================================

#include "StdInc.h"
#include "PatchMW2UIScripts.h"

using namespace std;

void UIScript_Quit(char* args)
{
	Cmd_ExecuteSingleCommand(0, 0, "uploadStats");
	Cmd_ExecuteSingleCommand(0, 0, "quit");
}

void UIScript_DirectConnect(char* args)
{
	dvar_t* dvar = Dvar_FindVar("ui_server");
	Cmd_ExecuteSingleCommand(0, 0, va("connect %s", dvar->current.string));
}

struct mapArena_t
{
	char uiName[32];
	char mapName[16];
	char pad[2768];
};

static int* numArenas = (int*)0x62E6930;
static mapArena_t* arenas = (mapArena_t*)0x62E6934;

string getBasemapValueForKey( const char* buffer, char* key )
{
	auto bufSplit = explode( buffer, "\"" );

	for(int i=0;i<bufSplit.size();i+=2)
	{
		if(bufSplit[i].find(key) != -1)
		{
			return bufSplit[i + 1];
		}
	}

	return "";
}

void UIScript_LoadArenas(char* FuckTheseArgs)
{
	char* buffer;
	int length = FS_ReadFile("mp/basemaps.arena", &buffer);

	if( length == 0)
	{
		*numArenas = 0;
		return;
	}

	int count = 0;
	auto partialMapSplit = explode( buffer, "}" );

	for(int i = 0; i < partialMapSplit.size(); i++)
	{
		strncpy(arenas[i].mapName, getBasemapValueForKey( partialMapSplit[i].c_str(), "map" ).c_str(), sizeof(arenas[i].mapName));
		strncpy(arenas[i].uiName, getBasemapValueForKey( partialMapSplit[i].c_str(), "longname" ).c_str(), sizeof(arenas[i].uiName));
		count++;
	}

	*numArenas = count;
}

void UIScript_startSingleplayer(char* blah)
{
	strcpy((char*)0x649FB68, "iw4sp_ceg.exe");
	//((void(*)())0x64CFB0)();
	//*(BYTE*)0x649FB68 = 0x00;
	Cmd_ExecuteSingleCommand(0, 0, "quit");
}

void PatchMW2_UIScriptFix()
{
	AddUIScript("quit", UIScript_Quit);
	AddUIScript("DirectConnnect", UIScript_DirectConnect);
	AddUIScript("LoadArenas", UIScript_LoadArenas);
	AddUIScript("startSingleplayer", UIScript_startSingleplayer);
}