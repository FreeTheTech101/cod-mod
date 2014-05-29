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

void replaceLocalizedStrings()
{
	addLocStr("MENU_INVITE_FRIEND", "Search party");
	addLocStr("MENU_DESC_INVITE_FRIEND", "Search another player.");
	addLocStr("MENU_SP_STEAM_CHAT_HINT", "");
	addLocStr("PATCH_STRICTHINT", "");
	addLocStr("PATCH_STRICTHINT_COOP", "");
}

void PatchMW2_LocalizedStrings()
{
	SELoadLanguageHook.initialize(SELoadLanguageHookLoc, loadLanguage);
	SELoadLanguageHook.installHook();

	call(localizeAssetHookLoc, loadLocalizeAsset, PATCH_CALL);

	Dvar_RegisterEnum("loc_modLanguage", ModLanguageEnum, 0, DVAR_FLAG_SAVED, "Preferred language for custom localized strings.");

	// String by Dekart811
	if(version == 177)
	{
		addLocStr("EXE_AWAITINGGAMESTATE", "Awaiting gamestate");

		// menu strings
		addLocStr("MPUI_TITLE", "IW4");
		addLocStr("MPUI_LAN", "LAN");
		addLocStr("MPUI_NEWS", "News:");
		addLocStr("MPUI_NEWS_CAPS", "NEWS:");

		// menu buttons
		addLocStr("MENU_RECONNECTING_TO_PARTY", "CONNECTING TO SERVER");
		addLocStr("MENU_PASSWORD_CAPS", "PASSWORD");
		addLocStr("MENU_MODS_CAPS", "MODS");
		addLocStr("MENU_NEWS", "News");
		addLocStr("MENU_NEWS_CAPS", "NEWS");
		addLocStr("MENU_STORE", "Store");
		addLocStr("MENU_STORE_CAPS", "STORE");
		addLocStr("MENU_FRIENDS", "Friends");
		addLocStr("MENU_FRIENDS_CAPS", "FRIENDS");
		addLocStr("MENU_LAN", "LAN");
		addLocStr("MENU_SP_MAPS_C", "^3Singleplayer Maps");
		addLocStr("MENU_SP_MAPS", "Singleplayer Maps");
		addLocStr("MENU_JOIN", "Join");
		addLocStr("MENU_JOIN_CAPS", "JOIN");

		// mennu buttons with shortcuts
		addLocStr("PLATFORM_FRIENDS", "Friends ^0- ^3F^7");
		addLocStr("PLATFORM_FRIENDS_CAPS", "FRIENDS ^0- ^3F^7");
		addLocStr("PLATFORM_STORE", "Store ^0- ^3S^7");
		addLocStr("PLATFORM_STORE_CAPS", "STORE ^0- ^3S^7");
		addLocStr("PLATFORM_CONNECT_CAPS",  "CONNECT ^0- ^3C");
		addLocStr("PLATFROM_CHANGE_NAME", "Change Name ^0- ^3N");
		addLocStr("PLATFORM_CHANGE_NAME_CAPS", "CHANGE NAME ^0- ^3N");

		// map names
		addLocStr("MPUI_BURGERTOWN", "Burger Town");
		addLocStr("MPUI_GHILLIES", "Pripyat");
		addLocStr("MPUI_TESTMAP", "IW4 Test Map");
		addLocStr("MPUI_CONTINGENCY", "Contingency");
		addLocStr("MPUI_NUKED", "Nuketown");
		addLocStr("MPUI_BOG_SH", "Bog");
		addLocStr("MPUI_KILLHOUSE", "Killhouse");

		// map descriptions
		addLocStr("MPUI_DESC_MAP_OILRIG", "TODO: Write description for MPUI_DESC_MAP_OILRIG");
		addLocStr("MPUI_DESC_MAP_GULAG", "An open air fight in the Gulag, 40 miles east of Petropavlovsk, Russia.");
		addLocStr("MPUI_DESC_MAP_BURGERTOWN", "Eating a burger while killing enemies?\nForget it!");
		addLocStr("MPUI_DESC_MAP_GHILLIES", "Stay away of the radiation and survive in Pripyat, Ukraine.");
		addLocStr("MPUI_DESC_MAP_TESTMAP", "A small testing map.");
		addLocStr("MPUI_DESC_MAP_CONTINGENCY", "Survive in a biting cold battle.");
		addLocStr("MPUI_DESC_MAP_TRAINER", "Remember... switching to your pistol is always faster than reloading!");
		addLocStr("MPUI_DESC_MAP_FAVELA_SP", "A Slum in Brazil. Fast-paced action on a small map.");
		addLocStr("MPUI_DESC_MAP_NUKED", "A deserted nuke testing facility used in the Cold War.");
		addLocStr("MPUI_DESC_MAP_KILLHOUSE", "A very small symmetrical map with a looking post in the middle.");
		addLocStr("MPUI_DESC_SP_MAPS", "Ported maps from the Singleplayer.");

		// gamemodes
		addLocStr("MPUI_VIP_CAPS", "VIP");

		// misc
		addLocStr("MPUI_DESC_CHANGE_NAME", "Change your Nickname.");

		// weapons
		addLocStr("WEAPON_PEACEKEEPER", "Peacekeeper SMG");

		// playercard titles
		addLocStr("PLAYERCARDS_TITLE_FNG", "<3 IW4LAN"); // new text for the default playercard icon
	}
}