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

Com_Error_t Com_Error = (Com_Error_t)0x43DD90;
Com_Printf_t Com_Printf = (Com_Printf_t)0x41BD20;
Com_PrintError_t Com_PrintError = (Com_PrintError_t)0x4C6980;
Com_Milliseconds_t Com_Milliseconds = (Com_Milliseconds_t)0x44E130;

Dvar_FindVar_t Dvar_FindVar = (Dvar_FindVar_t)0x4B29D0;
Dvar_SetCommand_t Dvar_SetCommand = (Dvar_SetCommand_t)0x4959D0;
Dvar_RegisterBool_t Dvar_RegisterBool = (Dvar_RegisterBool_t)0x429390;
Dvar_RegisterString_t Dvar_RegisterString = (Dvar_RegisterString_t)0x49E0B0;
Dvar_RegisterInt_t Dvar_RegisterInt = (Dvar_RegisterInt_t)0x4E9490;

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

Field_Clear_t Field_Clear = (Field_Clear_t)0x45C350;

CL_IsCgameInitialized_t CL_IsCgameInitialized = (CL_IsCgameInitialized_t)0x4EEA50;
};

dvar_t** dvarName = (dvar_t**)0x929154;

DWORD drawDevStuffHookLoc = 0x57DB09;
DWORD windowedWindowStyleHookLoc = 0x50C0B3;
DWORD winMainInitHookLoc = 0x4A7910;
DWORD imageVersionCheckHookLoc = 0x544756;

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