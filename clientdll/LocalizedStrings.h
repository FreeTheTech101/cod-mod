#include "stdinc.h"

#define MAX_LOCALZE_STRINGS 100

using namespace std;

int currentPos = 0;

const char* reference[MAX_LOCALZE_STRINGS];
const char* langEnglish[MAX_LOCALZE_STRINGS];

void addLocStr(const char* key, const char* value)
{
	reference[currentPos] = key;
	langEnglish[currentPos] = value;
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