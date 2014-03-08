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

#define MAX_LOCALZE_STRINGS 14000

int currentPos = 0;
std::string reference[MAX_LOCALZE_STRINGS];
std::string langEnglish[MAX_LOCALZE_STRINGS];

void addLocStr(const char* key, const char* value)
{
	for(int i = 0;i<currentPos;i++)
	{
		if(!strcmp(reference[i].c_str(), key))
		{
			langEnglish[i] = value;
			return;
		}
	}

	if(currentPos == MAX_LOCALZE_STRINGS - 1)
		return;

	reference[currentPos] = key;
	langEnglish[currentPos] = value;
	currentPos++;
}

bool isCustomLoc(const char* key)
{
	for(int i = 0;i<currentPos;i++)
	{
		if(!strcmp(reference[i].c_str(), key))
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
		if(!strcmp(reference[i].c_str(), key))
		{
			(*entry).value = langEnglish[i].c_str();
			break;
		}
	}
}