// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Stuff to handle custom localized strings
//
// Initial author: momo5502
// Started: 2014-03-07
// ==========================================================

#include "stdinc.h"

#define MAX_LOCALZE_STRINGS 100

int currentPos = 0;
const char* reference[MAX_LOCALZE_STRINGS];
const char* langEnglish[MAX_LOCALZE_STRINGS];

void addLocStr(const char* key, const char* value)
{
	const char* _key = va(key);
	const char* _value = va(value);

	for(int i = 0;i<currentPos;i++)
	{
		if(!strcmp(reference[i], _key))
		{
			langEnglish[i] = _value;
			return;
		}
	}

	reference[currentPos] = _key;
	langEnglish[currentPos] = _value;
	currentPos++;
}

bool isCustomLoc(const char* key)
{
	for(int i = 0;i<currentPos;i++)
	{
		if(!strcmp(reference[i], key))
		{
			return true;
		}
	}

	return false;
}

void buildCustomEntry(localizedEntry_s* entry, const char* key)
{
	(*entry).name = key;
	(*entry).value = "";

	for(int i = 0;i<currentPos;i++)
	{
		if(!strcmp(reference[i], key))
		{
			(*entry).value = langEnglish[i];
			break;
		}
	}
}