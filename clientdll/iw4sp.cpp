// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: steam_api
// Purpose: Game-specific code implementations.
//
// Initial author: momo5502
// Started: 2014-03-02
// ==========================================================

#include "StdInc.h"

// function definitions
extern "C"
{
R_RegisterFont_t R_RegisterFont = (R_RegisterFont_t)0x508910;
R_AddCmdDrawText_t R_AddCmdDrawText = (R_AddCmdDrawText_t)0x50E7A0;

BG_LoadWeaponDef_LoadObj_t BG_LoadWeaponDef_LoadObj = (BG_LoadWeaponDef_LoadObj_t)0x659DA0;

Com_Error_t Com_Error = (Com_Error_t)0x43DD90;
Com_Printf_t Com_Printf = (Com_Printf_t)0x41BD20;
Com_PrintError_t Com_PrintError = (Com_PrintError_t)0x4C6980;
Com_Milliseconds_t Com_Milliseconds = (Com_Milliseconds_t)0x44E130;

Dvar_FindVar_t Dvar_FindVar = (Dvar_FindVar_t)0x4B29D0;
Dvar_SetCommand_t Dvar_SetCommand = (Dvar_SetCommand_t)0x4959D0;
Dvar_RegisterBool_t Dvar_RegisterBool = (Dvar_RegisterBool_t)0x429390;
Dvar_RegisterString_t Dvar_RegisterString = (Dvar_RegisterString_t)0x49E0B0;
Dvar_RegisterInt_t Dvar_RegisterInt = (Dvar_RegisterInt_t)0x4E9490;
Dvar_RegisterEnum_t Dvar_RegisterEnum = (Dvar_RegisterEnum_t)0x4CB7C0;

Cmd_AddCommand_t Cmd_AddCommand = (Cmd_AddCommand_t)0x4478A0;
Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand = (Cmd_ExecuteSingleCommand_t)0x46AFD0;
Cmd_SetAutoComplete_t Cmd_SetAutoComplete = (Cmd_SetAutoComplete_t)0x48A880;

DB_FindXAssetHeader_t DB_FindXAssetHeader = (DB_FindXAssetHeader_t)0x40B200;
DB_LoadXAssets_t DB_LoadXAssets = (DB_LoadXAssets_t)0x4CFC90;

Steam_JoinLobby_t Steam_JoinLobby = (Steam_JoinLobby_t)0x43A4F0;
NET_StringToAdr_t NET_StringToAdr = (NET_StringToAdr_t)0x40A590;

FS_ReadFile_t FS_ReadFile = (FS_ReadFile_t)0x4A5480;
FS_FOpenFileRead_t FS_FOpenFileRead = (FS_FOpenFileRead_t)0x48DD10;
FS_FCloseFile_t FS_FCloseFile = (FS_FCloseFile_t)0x44E0A0;
FS_Read_t FS_Read = (FS_Read_t)0x42EDC0;
FS_ListFiles_t FS_ListFiles = (FS_ListFiles_t)0x4448F0;
FS_FreeFileList_t FS_FreeFileList = (FS_FreeFileList_t)0x41C7A0;

Field_Clear_t Field_Clear = (Field_Clear_t)0x45C350;

CL_IsCgameInitialized_t CL_IsCgameInitialized = (CL_IsCgameInitialized_t)0x4EEA50;

SE_Load_t SE_Load = (SE_Load_t)0x409910;

// Menu stuff
LoadScriptFile_t LoadScriptFile = (LoadScriptFile_t)0x405CD0;
PC_ReadToken_t 	PC_ReadToken = (PC_ReadToken_t)0x42F1A0;
FreeMemory_t FreeMemory = (FreeMemory_t)0x4A7D20;
Script_Alloc_t Script_Alloc = (Script_Alloc_t)0x41BCD0;
Script_SetupTokens_t Script_SetupTokens = (Script_SetupTokens_t)0x4B59B0;
Script_CleanString_t Script_CleanString = (Script_CleanString_t)0x4316A0;
};

dvar_t** dvarName = (dvar_t**)0x929154;

char* language = (char*)0x19FF828;

DWORD drawDevStuffHookLoc = 0x57DB09;
DWORD windowedWindowStyleHookLoc = 0x50C0B3;
DWORD winMainInitHookLoc = 0x4A7910;
DWORD imageVersionCheckHookLoc = 0x544756;
DWORD localizeAssetHookLoc = 0x61BB17;
DWORD SELoadLanguageHookLoc = 0x61BCA7;
DWORD SetStringHookLoc = 0x4759FE;
DWORD zoneLoadHookLoc = 0x582F9C;
DWORD ffLoadHook1Loc = 0x418DE8;
DWORD menuFileHookLoc = 0x62DDD0;	
DWORD scrGetFunctionHookLoc = 0x4ADF9C;
DWORD scrDeclareFunctionTableEntryLoc = 0x4F59C0;
DWORD loadGameOverlayHookLoc = 0x604350;
DWORD findSoundAliasHookLoc = 0x631A39;
DWORD printVersionHookLoc = 0x6030C2;
DWORD weaponFileHookLoc = 0x659E00;
DWORD vehicleFileHookLoc = 0x667D90;

DWORD initializeRenderer = 0x48A400;
DWORD SteamFriendsLoc = 0x694528;

source_t **sourceFiles = (source_t **)0x7440E8;
keywordHash_t **menuParseKeywordHash = (keywordHash_t **)0x1933DB0;

int version = 0;

// console commands
DWORD* cmd_id = (DWORD*)0x144FED0;
DWORD* cmd_argc = (DWORD*)0x144FF14;
DWORD** cmd_argv = (DWORD**)0x144FF34;

/*
============
Cmd_Argc
============
*/
/*int		Cmd_Argc( void ) {
	return cmd_argc[*cmd_id];
}*/

/*
============
Cmd_Argv
============
*/
/*char	*Cmd_Argv( int arg ) {
	if ( (unsigned)arg >= cmd_argc[*cmd_id] ) {
		return "";
	}
	return (char*)(cmd_argv[*cmd_id][arg]);	
}*/

// get convar string
char* GetStringConvar(char* key) {
	dvar_t* var = Dvar_FindVar(key);

	if (!var) return "";

	return var->current.string;
}