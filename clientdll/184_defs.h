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

	Cmd_AddCommand = (Cmd_AddCommand_t)0x428090;
	Cmd_ExecuteSingleCommand = (Cmd_ExecuteSingleCommand_t)0x4DA360;

	Com_Printf = (Com_Printf_t)0x466380;
	Com_Error = (Com_Error_t)0x4584F0;

	CL_IsCgameInitialized = (CL_IsCgameInitialized_t)0x43D7B0;

	R_RegisterFont = (R_RegisterFont_t)0x507FE0;
	R_AddCmdDrawText = (R_AddCmdDrawText_t)0x50E080;

	FS_ListFiles = (FS_ListFiles_t)0x4B0040;
	FS_FreeFileList = (FS_FreeFileList_t)0x4E6180;

	SE_Load = (SE_Load_t)0x4FCCC0;


	drawDevStuffHookLoc = 0x57AE79;
	windowedWindowStyleHookLoc = 0x50B823;
	winMainInitHookLoc = 0x4B1BC0;
	imageVersionCheckHookLoc = 0x544046;
	localizeAssetHookLoc = 0x6192D7;
	SELoadLanguageHookLoc = 0x619467;
	SetStringHookLoc = 0x42EC0E;
	zoneLoadHookLoc = 0x58039C;

	language = (char*)0x19FC2B0;

	cmd_id = (DWORD*)0x144C950;
	cmd_argc = (DWORD*)0x144C994;
	cmd_argv = (DWORD**)0x144C9B4;
}