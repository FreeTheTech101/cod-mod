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
	Cbuf_AddText_t Cbuf_AddText = (Cbuf_AddText_t)0x404B20;

	CL_AddReliableCommand_t CL_AddReliableCommand = (CL_AddReliableCommand_t)0x454F40;
	CL_IsCgameInitialized_t CL_IsCgameInitialized = (CL_IsCgameInitialized_t)0x43EB20;

	Cmd_AddCommand_t Cmd_AddCommand = (Cmd_AddCommand_t)0x470090;
	Cmd_AddServerCommand_t Cmd_AddServerCommand = (Cmd_AddServerCommand_t)0x4DCE00;
	Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand = (Cmd_ExecuteSingleCommand_t)0x609540;
	Cmd_SetAutoComplete_t Cmd_SetAutoComplete = (Cmd_SetAutoComplete_t)0x40EDC0;

	Com_Error_t Com_Error = (Com_Error_t)0x4B22D0;
	Com_ParseExt_t Com_ParseExt = (Com_ParseExt_t)0x4C0350;
	Com_ParseExt2_t Com_ParseExt2 = (Com_ParseExt2_t)0x474D60;
	Com_BeginParseSession_t Com_BeginParseSession = (Com_BeginParseSession_t)0x4AAB80;
	Com_EndParseSession_t Com_EndParseSession = (Com_EndParseSession_t)0x4B80B0;
	Com_Printf_t Com_Printf = (Com_Printf_t)0x402500;
	Com_Milliseconds_t Com_Milliseconds = (Com_Milliseconds_t)0x42A660;
	Com_PrintError_t Com_PrintError = (Com_PrintError_t)0x4F8C70;

	DB_FindXAssetHeader_t DB_FindXAssetHeader = (DB_FindXAssetHeader_t)0x407930;
	DB_FindXAssetHeader_MW3_t DB_FindXAssetHeader_MW3 = (DB_FindXAssetHeader_MW3_t)0;
	DB_LoadXAssets_t DB_LoadXAssets = (DB_LoadXAssets_t)0x4E5930;

	Dvar_RegisterBool_t Dvar_RegisterBool = (Dvar_RegisterBool_t)0x4CE1A0;
	Dvar_RegisterFloat_t Dvar_RegisterFloat = (Dvar_RegisterFloat_t)0x648440;
	Dvar_RegisterFloat2_t Dvar_RegisterFloat2 = (Dvar_RegisterFloat2_t)0x4F6070;
	Dvar_RegisterFloat3_t Dvar_RegisterFloat3 = (Dvar_RegisterFloat3_t)0x4EF8E0;
	Dvar_RegisterFloat4_t Dvar_RegisterFloat4 = (Dvar_RegisterFloat4_t)0x4F28E0;
	Dvar_RegisterInt_t Dvar_RegisterInt = (Dvar_RegisterInt_t)0x479830;
	Dvar_RegisterEnum_t Dvar_RegisterEnum = (Dvar_RegisterEnum_t)0x412E40;
	Dvar_RegisterString_t Dvar_RegisterString = (Dvar_RegisterString_t)0x4FC7E0;
	Dvar_RegisterColor_t Dvar_RegisterColor = (Dvar_RegisterColor_t)0x471500;

	Dvar_FindVar_t Dvar_FindVar = (Dvar_FindVar_t)0x4D5390;
	Dvar_InfoString_Big_t Dvar_InfoString_Big = (Dvar_InfoString_Big_t)0x4D98A0;
	Dvar_SetCommand_t Dvar_SetCommand = (Dvar_SetCommand_t)0x4EE430;
	Dvar_SetStringByName_t Dvar_SetStringByName = (Dvar_SetStringByName_t)0x4F52E0;

	FS_FreeFile_t FS_FreeFile = (FS_FreeFile_t)0x4416B0;
	FS_ReadFile_t FS_ReadFile = (FS_ReadFile_t)0x4F4B90;
	FS_ListFiles_t FS_ListFiles = (FS_ListFiles_t)0x441BB0;
	FS_FreeFileList_t FS_FreeFileList = (FS_FreeFileList_t)0x4A5DE0;
	FS_FOpenFileAppend_t FS_FOpenFileAppend = (FS_FOpenFileAppend_t)0x410BB0;
	FS_FOpenFileAppend_t FS_FOpenFileWrite = (FS_FOpenFileAppend_t)0x4BA530;
	FS_FOpenFileRead_t FS_FOpenFileRead = (FS_FOpenFileRead_t)0x46CBF0;
	FS_FCloseFile_t FS_FCloseFile = (FS_FCloseFile_t)0x462000;
	FS_WriteFile_t FS_WriteFile = (FS_WriteFile_t)0x426450;
	FS_Write_t FS_Write = (FS_Write_t)0x4C06E0;
	FS_Read_t FS_Read = (FS_Read_t)0x4A04C0;
	FS_Seek_t FS_Seek = (FS_Seek_t)0x4A63D0;

	G_LogPrintf_t G_LogPrintf = (G_LogPrintf_t)0x4B0150;

	MSG_Init_t MSG_Init = (MSG_Init_t)0x45FCA0;
	MSG_ReadData_t MSG_ReadData = (MSG_ReadData_t)0x4527C0;
	MSG_ReadLong_t MSG_ReadLong = (MSG_ReadLong_t)0x4C9550;
	MSG_ReadShort_t MSG_ReadShort = (MSG_ReadShort_t)0x40BDD0;
	MSG_ReadInt64_t MSG_ReadInt64 = (MSG_ReadInt64_t)0x4F1850;
	MSG_ReadString_t MSG_ReadString = (MSG_ReadString_t)0x60E2B0;
	MSG_WriteByte_t MSG_WriteByte = (MSG_WriteByte_t)0x48C520;
	MSG_WriteData_t MSG_WriteData = (MSG_WriteData_t)0x4F4120;
	MSG_WriteLong_t MSG_WriteLong = (MSG_WriteLong_t)0x41CA20;

	NET_AdrToString_t NET_AdrToString = (NET_AdrToString_t)0x469880;
	NET_CompareAdr_t NET_CompareAdr = (NET_CompareAdr_t)0x4D0AA0;
	NET_StringToAdr_t NET_StringToAdr = (NET_StringToAdr_t)0x409010;

	SV_GameSendServerCommand_t SV_GameSendServerCommand = (SV_GameSendServerCommand_t)0x4BC3A0;

	TeleportPlayer_t TeleportPlayer = (TeleportPlayer_t)0x496850;

	Sys_SendPacket_t Sys_SendPacket = (Sys_SendPacket_t)0x48F500;
	Sys_ListFiles_t Sys_ListFiles = (Sys_ListFiles_t)0x45A660;
	Sys_FreeFileList_t Sys_FreeFileList = (Sys_FreeFileList_t)0x4D8580;

	Cmd_TokenizeString_t Cmd_TokenizeString = (Cmd_TokenizeString_t)0x463E40;
	Cmd_EndTokenizedString_t Cmd_EndTokenizedString = (Cmd_EndTokenizedString_t)0x4D9540;

	// other stuff
	CommandCB_t Cbuf_AddServerText_f = (CommandCB_t)0x4BB9B0;

	R_RegisterFont_t R_RegisterFont = (R_RegisterFont_t)0x505670;
	R_AddCmdDrawText_t R_AddCmdDrawText = (R_AddCmdDrawText_t)0x509D80;

	Steam_JoinLobby_t Steam_JoinLobby = (Steam_JoinLobby_t)0x49CF70;

	SL_GetString_t SL_GetString = (SL_GetString_t)0x4CDC10;

	// 159 stuff
	SE_Load_t SE_Load = (SE_Load_t)0x502A30;
	LoadScriptFile_t LoadScriptFile = (LoadScriptFile_t)0x480110;
	PC_ReadToken_t 	PC_ReadToken = (PC_ReadToken_t)0x4ACCD0;
	FreeMemory_t FreeMemory = (FreeMemory_t)0x4D6640;
	Script_Alloc_t Script_Alloc = (Script_Alloc_t)0x422E70;
	Script_SetupTokens_t Script_SetupTokens = (Script_SetupTokens_t)0x4E6950;
	Script_CleanString_t Script_CleanString = (Script_CleanString_t)0x498220;
	R_AddCmdDrawStretchPic_t R_AddCmdDrawStretchPic = (R_AddCmdDrawStretchPic_t)0x50E190;
	Field_Clear_t Field_Clear = (Field_Clear_t)0x45C350;
	BG_LoadWeaponDef_LoadObj_t BG_LoadWeaponDef_LoadObj = (BG_LoadWeaponDef_LoadObj_t)0x57B5F0;
};

dvar_t** dvarName = (dvar_t**)0x929154;

char* language = (char*)0x19FF828;

DWORD drawDevStuffHookLoc = 0x57DB09;
DWORD windowedWindowStyleHookLoc = 0x50C0B3;
DWORD winMainInitHookLoc = 0x4A7910;
DWORD imageVersionCheckHookLoc = 0x544756;
DWORD localizeAssetHookLoc = 0x629B97;
DWORD SELoadLanguageHookLoc = 0x629D27;
DWORD SetStringHookLoc = 0x4CE5EE;
DWORD zoneLoadHookLoc = 0x582F9C;
DWORD ffLoadHook1Loc = 0x418DE8;
DWORD menuFileHookLoc = 0x63FE70;
DWORD scrGetFunctionHookLoc = 0x4ADF9C;
DWORD scrDeclareFunctionTableEntryLoc = 0x4F59C0;
DWORD loadGameOverlayHookLoc = 0x604350;
DWORD findSoundAliasHookLoc = 0x631A39;
DWORD printVersionHookLoc = 0x6030C2;
DWORD weaponFileHookLoc = 0x57B650;
DWORD vehicleFileHookLoc = 0x67CD40;
DWORD dbAddXAssetHookLoc = 0x581EE0;
DWORD dbAddXAssetHookRet = 0x581EE7;
DWORD getBSPNameHookLoc = 0x430C09;

DWORD initializeRenderer = 0x48A400;
DWORD SteamFriendsLoc = 0x694528;
DWORD SteamUserStatsLoc = 0x69455C;
DWORD SteamUserStatsInstanceLoc = 0x1BEE1C0;

source_t **sourceFiles = (source_t **)0x7C4A98;
keywordHash_t **menuParseKeywordHash = (keywordHash_t **)0x63AE928;

int version = 0;

// console commands
DWORD* cmd_id = (DWORD*)0x144FED0;
DWORD* cmd_argc = (DWORD*)0x144FF14;
DWORD** cmd_argv = (DWORD**)0x144FF34;

const char *current_zone = nullptr;
bool _allowZoneChange = true;

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