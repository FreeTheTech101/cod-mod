// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: UI file loading
//
// Initial author: NTAuthority
// Started: 2011-10-21
// ==========================================================

#include "StdInc.h"

std::vector<menuDef_t*> UI_ParseMenu(const char *menuFile);
bool Menu_Parse(int handle, menuDef_t *menu);
int PC_LoadSource(const char *filename);
int PC_FreeSource(int handle);

// 'shared' code
int KeywordHash_Key(char *keyword) {
	int register hash, i;

	hash = 0;
	for (i = 0; keyword[i] != '\0'; i++) {
		if (keyword[i] >= 'A' && keyword[i] <= 'Z')
			hash += (keyword[i] + ('a' - 'A')) * (119 + i);
		else
			hash += keyword[i] * (119 + i);
	}
	hash = (hash ^ (hash >> 10) ^ (hash >> 20)) & (KEYWORDHASH_SIZE-1);
	return hash;
}

void FreeScript(script_t *script)
{
#ifdef PUNCTABLE
	if (script->punctuationtable) FreeMemory(script->punctuationtable);
#endif //PUNCTABLE
	FreeMemory(script);

}

void StripDoubleQuotes(char *string)
{
	if (*string == '\"')
	{
		strcpy(string, string+1);
	} //end if
	if (string[strlen(string)-1] == '\"')
	{
		string[strlen(string)-1] = '\0';
	} //end if
} 

source_t* LoadSourceString(const char* string);

source_t *LoadSourceFile(const char *filename)
{
	int fh;
	int length = FS_FOpenFileRead(filename, &fh, 0);

	if (length >= 0)
	{
		char* buffer = new char[length + 1];
		FS_Read(buffer, length, fh);
		FS_FCloseFile(fh);

		buffer[length] = '\0';

		source_t* retval = LoadSourceString(buffer);
		delete[] buffer;

		return retval;
	}

	return NULL;
}

script_t* LoadScriptString(const char* string)
{
	static_assert(sizeof(script_t) == 1200, "script_t != 1200");

	int length = strlen(string);
	script_t* script = Script_Alloc(sizeof(script_t) + 1 + length);

	strcpy_s(script->filename, sizeof(script->filename), "script_t");
	script->buffer = (char*)(script + 1);
	
	*((char*)(script + 1) + length) = '\0';

	script->script_p = script->buffer;
	script->lastscript_p = script->buffer;
	script->length = length;
	script->end_p = &script->buffer[length];
	script->line = 1;
	script->lastline = 1;
	script->tokenavailable = 0;
	
	if(version == 159)
	{
		Script_SetupTokens(script, (void*)0x7320E0);
		script->punctuations = (punctuation_t*)0x7320E0;
	}
	else if(version == 184)
	{
		Script_SetupTokens(script, (void*)0x72F0E0);
		script->punctuations = (punctuation_t*)0x72F0E0;
	}

	strcpy(script->buffer, string);

	script->length = Script_CleanString(script->buffer);

	return script;
}

source_t* LoadSourceString(const char* string)
{
	source_t *source;
	script_t *script;

	script = LoadScriptString(string);
	if (!script) return NULL;

	script->next = NULL;

	source = (source_t *) malloc(sizeof(source_t));
	memset(source, 0, sizeof(source_t));

	strncpy(source->filename, "string", 64);
	source->scriptstack = script;
	source->tokens = NULL;
	source->defines = NULL;
	source->indentstack = NULL;
	source->skip = 0;
	source->definehash = (define_t**)malloc(4096);
	memset(source->definehash, 0, 4096);

	return source;
} 

void FreeSource(source_t *source)
{
	script_t *script;
	token_t *token;
	define_t *define;
	indent_t *indent;
	//->int i;

	//PC_PrintDefineHashTable(source->definehash);
	//free all the scripts
	while(source->scriptstack)
	{
		script = source->scriptstack;
		source->scriptstack = source->scriptstack->next;
		FreeScript(script);
	} //end for
	//free all the tokens
	while(source->tokens)
	{
		token = source->tokens;
		source->tokens = source->tokens->next;
		//PC_FreeToken(token);
	} //end for
#if DEFINEHASHING
	for (i = 0; i < DEFINEHASHSIZE; i++)
	{
		while(source->definehash[i])
		{
			define = source->definehash[i];
			source->definehash[i] = source->definehash[i]->hashnext;
			PC_FreeDefine(define);
		} //end while
	} //end for
#else //DEFINEHASHING
	//free all defines
	while(source->defines)
	{
		define = source->defines;
		source->defines = source->defines->next;
		//PC_FreeDefine(define);
	} //end for
#endif //DEFINEHASHING
	//free all indents
	while(source->indentstack)
	{
		indent = source->indentstack;
		source->indentstack = source->indentstack->next;
		free(indent);
	} //end for
//#if DEFINEHASHING
	//
	if (source->definehash) free(source->definehash);
//#endif //DEFINEHASHING
	//free the source itself
	free(source);
}

#define MAX_SOURCEFILES		64

source_t* lastSource;

int PC_LoadSourceString(const char* string)
{
	source_t *source;
	int i;

	for (i = 1; i < MAX_SOURCEFILES; i++)
	{
		if (!sourceFiles[i])
			break;
	} //end for
	if (i >= MAX_SOURCEFILES)
		return 0;
	source = LoadSourceString(string);
	if (!source)
		return 0;
	lastSource = source;
	sourceFiles[i] = source;
	return i;
}

int PC_LoadSource(const char *filename)
{
	source_t *source;
	int i;

	for (i = 1; i < MAX_SOURCEFILES; i++)
	{
		if (!sourceFiles[i])
			break;
	} //end for
	if (i >= MAX_SOURCEFILES)
		return 0;
	source = LoadSourceFile(filename);
	if (!source)
		return 0;
	lastSource = source;
	sourceFiles[i] = source;
	return i;
}

int PC_FreeSource(int handle)
{
	if (handle < 1 || handle >= MAX_SOURCEFILES)
		return false;
	if (!sourceFiles[handle])
		return false;

	FreeSource(sourceFiles[handle]);
	sourceFiles[handle] = NULL;
	return true;
}

int PC_ReadTokenHandle(int handle, pc_token_t *pc_token)
{
	token_t token;
	int ret;

	if (handle < 1 || handle >= MAX_SOURCEFILES)
		return 0;
	if (!sourceFiles[handle])
		return 0;

	ret = PC_ReadToken(sourceFiles[handle], &token);
	strcpy(pc_token->string, token.string);
	pc_token->type = token.type;
	pc_token->subtype = token.subtype;
	pc_token->intvalue = token.intvalue;
	pc_token->floatvalue = (float)token.floatvalue;
	if (pc_token->type == TT_STRING)
		StripDoubleQuotes(pc_token->string);
	return ret;
}

void Menu_PostParse(menuDef_t *menu) {
	return;
	/*if (menu == NULL) {
		return;
	}
	if (menu->fullScreen) {
		menu->window.rect.x = 0;
		menu->window.rect.y = 0;
		menu->window.rect.w = 640;
		menu->window.rect.h = 480;
	}
	Menu_UpdatePosition(menu);*/
}

void Menu_Init(menuDef_t *menu) {
	memset(menu, 0, 2048);
	menu->items = (itemDef_t**)malloc(sizeof(itemDef_t*) * 512);
	//Window_Init(&menu->window);
}

int KeywordHash_Data(char* key)
{
	// patch this function on-the-fly, as it's some ugly C.
	DWORD func = 0x62DDF0;

	if(version == 184)
		func = 0x62B590;
		
	int retval = 0;

	if(version == 159)
	{
		*(DWORD*)0x62DDFE = 3523;
		*(DWORD*)0x62DE2B = 0x7F;
	}
	else if(version == 184)
	{
		*(DWORD*)0x62B59E = 3523;
		*(DWORD*)0x62B5CB = 0x7F;
	}
	
	__asm
	{
		mov eax, key
		call func
		mov retval, eax
	}

	if(version == 159)
	{
		*(DWORD*)0x62DDFE = 531;
		*(DWORD*)0x62DE2B = 0x1FF;
	}
	else if(version == 184)
	{
		*(DWORD*)0x62B59E = 531;
		*(DWORD*)0x62B5CB = 0x1FF;
	}

	return retval;
}

//#define PC_SourceError(x, y, ...) Com_Printf(0, y, __VA_ARGS__)
#define PC_SourceError_159(x, y, ...) ((void (*)(int, const char*, ...))0x43A6D0)(x, y, __VA_ARGS__)
#define PC_SourceError_184(x, y, ...) ((void (*)(int, const char*, ...))0x46C8E0)(x, y, __VA_ARGS__)

bool Menu_Parse(int handle, menuDef_t *menu) {
	pc_token_t token;
	keywordHash_t *key;

	if (!PC_ReadTokenHandle(handle, &token))
		return false;
	if (*token.string != '{') {
		return false;
	}
    
	while ( 1 ) {

		memset(&token, 0, sizeof(pc_token_t));
		if (!PC_ReadTokenHandle(handle, &token)) 
		{
			if(version == 159)
				PC_SourceError_159(handle, "end of file inside menu\n");
			else if(version == 184)
				PC_SourceError_184(handle, "end of file inside menu\n");
			return false;
		}

		if (*token.string == '}') {
			return true;
		}

		//key = KeywordHash_Find(menuParseKeywordHash, token.string);
		int idx = KeywordHash_Data(token.string);

		key = menuParseKeywordHash[idx];
		if (!key) 
		{
			if(version == 159)
				PC_SourceError_159(handle, "unknown menu keyword %s", token.string);
			else if(version == 184)
				PC_SourceError_184(handle, "unknown menu keyword %s", token.string);
			continue;
		}
		if ( !key->func((itemDef_t*)menu, handle) ) 
		{
			if(version == 159)
				PC_SourceError_159(handle, "couldn't parse menu keyword %s", token.string);
			else if(version == 184)
				PC_SourceError_184(handle, "couldn't parse menu keyword %s", token.string);
			return false;
		}
	}
	return false; 	// bk001205 - LCC missing return value
}

menuDef_t* Menu_New(int handle) {
	menuDef_t *menu = (menuDef_t*)malloc(2048); // FIXME: tentative size

	Menu_Init(menu);
	if (Menu_Parse(handle, menu)) {
		Menu_PostParse(menu);
	}

	return menu;
}

std::vector<menuDef_t*> UI_ParseMenu(const char *menuFile) {
	int handle;
	pc_token_t token;
	std::vector<menuDef_t*> retval;

	//Com_Printf(0, "Parsing menu file: %s\n", menuFile);

	handle = PC_LoadSource(menuFile);
	if (!handle) {
		return retval;
	}

	while ( 1 ) {
		memset(&token, 0, sizeof(pc_token_t));
		if (!PC_ReadTokenHandle( handle, &token )) {
			break;
		}

		if ( token.string[0] == '}' ) {
			break;
		}

		if (_stricmp(token.string, "menudef") == 0) {
			// start a new menu
			retval.push_back(Menu_New(handle));
		}
	}
	
	PC_FreeSource(handle);

	return retval;
}

menuFile_t* UI_ParseScriptMenu(const char* filename)
{
	if (FS_ReadFile(filename, NULL) <= 0)
	{
		return (menuFile_t*)DB_FindXAssetHeader(ASSET_TYPE_MENUFILE, filename);
	}

	menuFile_t* newFile = (menuFile_t*)malloc(sizeof(menuFile_t));
	newFile->filename = (char*)filename;

	std::vector<menuDef_t*> menus = UI_ParseMenu(filename);

	newFile->count = menus.size();
	newFile->menuFiles = (menuDef_t**)malloc(sizeof(menuDef_t*) * menus.size());

	for (size_t i = 0; i < menus.size(); i++)
	{
		newFile->menuFiles[i] = menus[i];
	}

	return newFile;
}

menuFile_t* uimp_code;

#define NUM_CUSTOM_MENUS 1

void* MenuFileHookFunc(const char* filename)
{
	menuFile_t* file = (menuFile_t*)DB_FindXAssetHeader(ASSET_TYPE_MENUFILE, filename);

	if (!strcmp(file->menuFiles[0]->window.name, "default_menu"))
	{
		Com_Printf(1, "MenuFileHookFunc: %s [parsing]\n", filename);
		return UI_ParseScriptMenu(filename);
	}

	if (!strcmp(filename, "ui/code.txt") &&  uimp_code)
	{
		Com_Printf(1, "MenuFileHookFunc: %s [cached]\n", filename);
		return uimp_code;
	}

	int numCustomMenus = 0;

	if (!strcmp(filename, "ui/menus.txt"))
	{
		numCustomMenus = NUM_CUSTOM_MENUS;
	}

	menuFile_t* newFile = (menuFile_t*)malloc(sizeof(menuFile_t));
	memcpy(newFile, file, sizeof(menuFile_t));

	newFile->menuFiles = (menuDef_t**)malloc(sizeof(menuDef_t*) * (newFile->count + numCustomMenus));
	memset(newFile->menuFiles, 0, sizeof(menuDef_t*) * (newFile->count + numCustomMenus));
	memcpy(newFile->menuFiles, file->menuFiles, sizeof(menuDef_t*) * newFile->count);

	newFile->count += numCustomMenus;

	bool replacedMenu = false;

	for (int i = 0; i < file->count; i++)
	{
		menuDef_t* menu = file->menuFiles[i];

		if (!menu)
		{
			continue;
		}

		char uifilename[512];
		sprintf(uifilename, "ui/%s.menu", menu->window.name);
		//Com_Printf(0, va("Parsing %s\n", uifilename));
		
		if (FS_ReadFile(uifilename, 0) >= 0)
		{
			std::vector<menuDef_t*> menus = UI_ParseMenu(uifilename);

			if (menus.size() > 0)
			{
				newFile->menuFiles[i] = menus[0];
			}

			if (menus.size() > 1)
			{
				Com_PrintError(0, "Menu load warning: more than 1 menu in %s\n", uifilename);
			}

			replacedMenu = true;
		}
	}

#define CUSTOM_MENU(name) { \
		std::vector<menuDef_t*> menu = UI_ParseMenu(name); \
		\
		if (menu.size() > 0) \
		{ \
			newFile->menuFiles[file->count] = menu[0]; \
		} \
		else \
		{ \
			file->count--; \
		} \
	}

	if (numCustomMenus)
	{
		//CUSTOM_MENU("ui/connect_coop.menu");
		CUSTOM_MENU("ui/acceptinvite_warning.menu");
	}

	if (!replacedMenu)
	{
		Com_Printf(1, "MenuFileHookFunc: %s [original]\n", filename);
		free(newFile->menuFiles);
		free(newFile);
	}
	else if (!strcmp(filename, "ui/code.txt"))
	{
		Com_Printf(1, "MenuFileHookFunc: %s [replaced]\n", filename);
		uimp_code = newFile;
	}
	else
	{
		Com_Printf(1, "MenuFileHookFunc: %s [replaced]\n", filename);
	}

	return (replacedMenu) ? newFile : file;
}

StompHook menuFileHook;

void __declspec(naked) MenuFileHookStub()
{
	__asm jmp MenuFileHookFunc
}

void PatchMW2_UILoading()
{
	menuFileHook.initialize(menuFileHookLoc, MenuFileHookStub);
	menuFileHook.installHook();

	if(version == 159)
	{
		*(BYTE*)0x62E049 = 0xEB;

		// don't load ASSET_TYPE_MENU assets for every menu (might cause patch menus to fail)
		memset((void*)0x4AE536, 0x90, 5);
	}
	else if(version == 184)
	{
		*(BYTE*)0x62B7E9 = 0xEB;

		// don't load ASSET_TYPE_MENU assets for every menu (might cause patch menus to fail)
		memset((void*)0x417686, 0x90, 5);
	}
	
}