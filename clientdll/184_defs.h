#include "stdinc.h"

void define184Stuff()
{
	version = 184;


	Dvar_FindVar = (Dvar_FindVar_t)0x4BCA20;
	Dvar_SetCommand = (Dvar_SetCommand_t)0x43A000;
	Dvar_RegisterString = (Dvar_RegisterString_t)0x433DF0;
	Dvar_RegisterBool = (Dvar_RegisterBool_t)0x4866B0;
	Dvar_RegisterEnum = (Dvar_RegisterEnum_t)0x4FB5C0;

	DB_FindXAssetHeader = (DB_FindXAssetHeader_t)0x4F6F10;
	DB_LoadXAssets = (DB_LoadXAssets_t)0x49EDB0;

	Cmd_AddCommand = (Cmd_AddCommand_t)0x428090;
	Cmd_ExecuteSingleCommand = (Cmd_ExecuteSingleCommand_t)0x4DA360;

	Com_Printf = (Com_Printf_t)0x466380;
	Com_Error = (Com_Error_t)0x4584F0;
	Com_Milliseconds = (Com_Milliseconds_t)0x44E1B0;
	Com_PrintError = (Com_PrintError_t)0x461AF0;

	CL_IsCgameInitialized = (CL_IsCgameInitialized_t)0x43D7B0;

	R_RegisterFont = (R_RegisterFont_t)0x507FE0;
	R_AddCmdDrawText = (R_AddCmdDrawText_t)0x50E080;

	FS_ListFiles = (FS_ListFiles_t)0x4B0040;
	FS_FreeFileList = (FS_FreeFileList_t)0x4E6180;
	FS_ReadFile = (FS_ReadFile_t)0x467510;
	FS_FOpenFileRead = (FS_FOpenFileRead_t)0x442130;
	FS_FCloseFile = (FS_FCloseFile_t)0x5009E0;
	FS_Read = (FS_Read_t)0x4CB360;

	SE_Load = (SE_Load_t)0x4FCCC0;

	// Menu stuff
	LoadScriptFile = (LoadScriptFile_t)0x4EE180;
	PC_ReadToken = (PC_ReadToken_t)0x4D7A00;
	FreeMemory = (FreeMemory_t)0x45B8C0;
	Script_Alloc = (Script_Alloc_t)0x452CC0;
	Script_SetupTokens = (Script_SetupTokens_t)0x4E7D20;
	Script_CleanString = (Script_CleanString_t)0x4443B0;


	drawDevStuffHookLoc = 0x57AE79;
	windowedWindowStyleHookLoc = 0x50B823;
	winMainInitHookLoc = 0x4B1BC0;
	imageVersionCheckHookLoc = 0x544046;
	localizeAssetHookLoc = 0x6192D7;
	SELoadLanguageHookLoc = 0x619467;
	SetStringHookLoc = 0x42EC0E;
	zoneLoadHookLoc = 0x58039C;
	ffLoadHook1Loc = 0x4D0558;
	menuFileHookLoc = 0x62B570;	
	scrGetFunctionHookLoc = 0x42ECAC;
	scrDeclareFunctionTableEntryLoc = 0x4A6E90;
	loadGameOverlayHookLoc = 0x601B10;

	initializeRenderer = 0x421100;
	SteamFriendsLoc = 0x691524;

	dvarName = (dvar_t**)0x925BCC;

	language = (char*)0x19FC2B0;

	cmd_id = (DWORD*)0x144C950;
	cmd_argc = (DWORD*)0x144C994;
	cmd_argv = (DWORD**)0x144C9B4;

	sourceFiles = (source_t **)0x7410E8;
	menuParseKeywordHash = (keywordHash_t **)0x1930830;
}