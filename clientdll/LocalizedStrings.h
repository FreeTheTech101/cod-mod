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
#include <string>

std::map<std::string, std::string> localizedStrings;

void addLocStr(const char* key, const char* value)
{
	localizedStrings[key] = value;
}

bool isCustomLoc(const char* key)
{
	return (localizedStrings.find(key) != localizedStrings.end());
}

void buildCustomEntry(localizedEntry_s* entry, const char* key)
{
	(*entry).name = key;
	(*entry).value = key;

	if(localizedStrings.find(key) != localizedStrings.end())
		(*entry).value = localizedStrings[key].c_str();
}