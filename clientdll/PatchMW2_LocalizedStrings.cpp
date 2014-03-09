// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Localized Strings
//
// Initial author: momo5502
// Started: 2014-03-07
// ==========================================================

#include "stdinc.h"
#include "LocalizedStrings.h"

localizedEntry_s* loadLocalizeAsset(int type, const char* reference)
{
	static localizedEntry_s entry; 

	if(isCustomLoc(reference))
	{
		buildCustomEntry(&entry, reference);
	}
	else
	{
		static localizedEntry_s* entryPtr; 
		entryPtr = (localizedEntry_s*)DB_FindXAssetHeader(type, reference);

		if(!entryPtr)
		{
			entry.name = reference;
			entry.value = reference;
		}
		else
		{
			entry = *entryPtr;
		}
	}

	return &entry;
}

// Code by zxz0O0
int LoadStrFiles(char* language)
{
	char buf[256];
	sprintf_s(buf, sizeof(buf), "%s/localizedstrings/", language);
	int count;
	char** list = FS_ListFiles(buf, "str", 0, &count);
	for(int i = 0;i < count;i++)
	{
		sprintf_s(buf, sizeof(buf), "%s/localizedstrings/%s", language, list[i]);
		Com_Printf(0, "Parsing localized string file: %s\n", buf);
		char* Error = SE_Load(buf, 0);
		if(Error)
			Com_Printf(0, "Error parsing localized string file: %s\n", Error);
	}
	FS_FreeFileList(list);

	return count;
}

void SELoadLanguageHookStub()
{
	//get language from loc_modLanguage
	dvar_t* modLang = Dvar_FindVar("loc_modLanguage");
	char** enumList = *(char***)&(modLang->max); //not sure if this is really dvar_maxmin_t max.
	char* language = enumList[modLang->current.integer];

	if(LoadStrFiles(language)==0)
		LoadStrFiles("english");//load english str files if there aren't any of selected language
}

CallHook SELoadLanguageHook;

void __declspec(naked) SetString(char* key, char* value, bool isEnglish)
{
	__asm
	{
		push [esp+8]
		push [esp+8]
		call addLocStr
		add esp, 8h
		mov eax,[esp] //get return address
		add esp,0x10 //remove stack space for parameters + return address
		jmp eax //return
	}
}

void loadLanguage()
{
	// First load original stuff
	__asm call SELoadLanguageHook.pOriginal

	// Change localized string loading method
	call(SetStringHookLoc, SetString, PATCH_CALL);

	// Then load custom strings
	SELoadLanguageHookStub();
}

char* ModLanguageEnum[] = { "english", "french", "german", "italian", "spanish", "english", "russian", "polish",  0 };

void replaceFriendConnect()
{
	addLocStr("MENU_INVITE_FRIEND", "Quick connect");
	addLocStr("MENU_DESC_INVITE_FRIEND", "Reconnect to last player.");
	addLocStr("MENU_SP_STEAM_CHAT_HINT", "");
}

void PatchMW2_LocalizedStrings()
{
	SELoadLanguageHook.initialize(SELoadLanguageHookLoc, loadLanguage);
	SELoadLanguageHook.installHook();

	call(localizeAssetHookLoc, loadLocalizeAsset, PATCH_CALL);

	Dvar_RegisterEnum("loc_modLanguage", ModLanguageEnum, 0, DVAR_FLAG_SAVED, "Preferred language for custom localized strings.");
}