#include "stdinc.h"

void define159Stuff()
{
	version = 159;


	R_RegisterFont = (R_RegisterFont_t)0x508910;
	R_AddCmdDrawText = (R_AddCmdDrawText_t)0x50E7A0;
	R_AddCmdDrawStretchPic = (R_AddCmdDrawStretchPic_t)0x50E190;
	R_TextWidth = (R_TextWidth_t)0x508960;

	BG_LoadWeaponDef_LoadObj = (BG_LoadWeaponDef_LoadObj_t)0x659DA0;

	Com_Error = (Com_Error_t)0x43DD90;
	Com_Printf = (Com_Printf_t)0x41BD20;
	Com_PrintError = (Com_PrintError_t)0x4C6980;
	Com_Milliseconds = (Com_Milliseconds_t)0x44E130;

	Dvar_FindVar = (Dvar_FindVar_t)0x4B29D0;
	Dvar_SetCommand = (Dvar_SetCommand_t)0x4959D0;
	Dvar_RegisterBool = (Dvar_RegisterBool_t)0x429390;
	Dvar_RegisterString = (Dvar_RegisterString_t)0x49E0B0;
	Dvar_RegisterInt = (Dvar_RegisterInt_t)0x4E9490;
	Dvar_RegisterEnum = (Dvar_RegisterEnum_t)0x4CB7C0;
	Dvar_GetString = (Dvar_GetString_t)0x44D830;

	Cmd_AddCommand = (Cmd_AddCommand_t)0x4478A0;
	Cmd_ExecuteSingleCommand = (Cmd_ExecuteSingleCommand_t)0x46AFD0;
	Cmd_SetAutoComplete = (Cmd_SetAutoComplete_t)0x48A880;

	DB_FindXAssetHeader = (DB_FindXAssetHeader_t)0x40B200;
	DB_LoadXAssets = (DB_LoadXAssets_t)0x4CFC90;

	Steam_JoinLobby = (Steam_JoinLobby_t)0x43A4F0;
	NET_StringToAdr = (NET_StringToAdr_t)0x40A590;

	FS_ReadFile = (FS_ReadFile_t)0x4A5480;
	FS_FOpenFileRead = (FS_FOpenFileRead_t)0x48DD10;
	FS_FCloseFile = (FS_FCloseFile_t)0x44E0A0;
	FS_Read = (FS_Read_t)0x42EDC0;
	FS_ListFiles = (FS_ListFiles_t)0x4448F0;
	FS_FreeFileList = (FS_FreeFileList_t)0x41C7A0;
	FS_FreeFile = (FS_FreeFile_t)0x4A7510;
	FS_WriteFile = (FS_WriteFile_t)0x0;

	Field_Clear = (Field_Clear_t)0x45C350;

	CL_IsCgameInitialized = (CL_IsCgameInitialized_t)0x4EEA50;

	SE_Load = (SE_Load_t)0x409910;

	G_GiveAchievement = (G_GiveAchievement_t)0x4E9420;

	Image_LoadFromFileWithReader = (Image_LoadFromFileWithReader_t)0x544EF0;
	Image_Release = (Image_Release_t)0x520BC0;

	// Menu stuff
	LoadScriptFile = (LoadScriptFile_t)0x405CD0;
	PC_ReadToken = (PC_ReadToken_t)0x42F1A0;
	FreeMemory = (FreeMemory_t)0x4A7D20;
	Script_Alloc = (Script_Alloc_t)0x41BCD0;
	Script_SetupTokens = (Script_SetupTokens_t)0x4B59B0;
	Script_CleanString = (Script_CleanString_t)0x4316A0;

	dvarName = (dvar_t**)0x929154;
	r_mode = (dvar_t**)0x1F1CC30;

	language = (char*)0x19FF828;

	drawDevStuffHookLoc = 0x57DB09;
	windowedWindowStyleHookLoc = 0x50C0B3;
	winMainInitHookLoc = 0x4A7910;
	imageVersionCheckHookLoc = 0x544756;
	localizeAssetHookLoc = 0x61BB17;
	SELoadLanguageHookLoc = 0x61BCA7;
	SetStringHookLoc = 0x4759FE;
	zoneLoadHookLoc = 0x582F9C;
	ffLoadHook1Loc = 0x418DE8;
	menuFileHookLoc = 0x62DDD0;	
	scrGetFunctionHookLoc = 0x4ADF9C;
	scrDeclareFunctionTableEntryLoc = 0x4F59C0;
	loadGameOverlayHookLoc = 0x604350;
	findSoundAliasHookLoc = 0x631A39;
	printVersionHookLoc = 0x6030C2;
	weaponFileHookLoc = 0x659E00;
	vehicleFileHookLoc = 0x667D90;
	dbAddXAssetHookLoc = 0x581EE0;
	dbAddXAssetHookRet = 0x581EE7;
	getBSPNameHookLoc = 0x430C09;
	materialLoadHookLoc = 0x509080;

	initializeRenderer = 0x48A400;
	SteamFriendsLoc = 0x694528;
	SteamUserStatsLoc = 0x69455C;
	SteamUserStatsInstanceLoc = 0x1BEE1C0;

	sourceFiles = (source_t **)0x7440E8;
	menuParseKeywordHash = (keywordHash_t **)0x1933DB0;

	// console commands
	cmd_id = (DWORD*)0x144FED0;
	cmd_argc = (DWORD*)0x144FF14;
	cmd_argv = (DWORD**)0x144FF34;
}