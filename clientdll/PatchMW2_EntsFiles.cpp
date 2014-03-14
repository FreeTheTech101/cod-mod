// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: d3dbsp.ents file loading and dumping
//
// Initial author: NTAuthority
// Started: 2011-05-25
// ==========================================================

#include "StdInc.h"
#include <vector>

// entity dumping, needs to be split?
struct MapEnts
{
	const char* name;
	const char* entitystring;
};

void DumpMapEntities(MapEnts* entities)
{
	char filename[255];

	CreateDirectory("raw/maps", NULL);
	CreateDirectory(va("raw/maps/%s", CURRENT_ZONE_NAME_159), NULL);

	_snprintf(filename, sizeof(filename), "raw/%s.ents", entities->name);

	FILE* file = fopen(filename, "w");
	if (file)
	{
		fwrite(entities->entitystring, 1, strlen(entities->entitystring), file);
		fclose(file);
	}
}

static char mapEntities[1024 * 1024];

void LoadMapEntities(MapEnts* entry)
{
	char* buffer;
	char filename[255];
	_snprintf(filename, sizeof(filename), "%s.ents", entry->name);

	// why the weird casts?
	if (FS_ReadFile((const char*)(&filename), (char**)&buffer) >= 0)
	{
		strcpy(mapEntities, buffer);
		entry->entitystring = mapEntities;

		FS_FreeFile(buffer);
	}
}

// more code
static DWORD gameWorldSP;

void AssetRestrict_PreLoadFromMaps(assetType_t type, void* entry, const char* zone)
{
	if (type == ASSET_TYPE_MAP_ENTS)
	{
		if (true || GAME_FLAG(GAME_FLAG_DUMPDATA))
		{
			DumpMapEntities((MapEnts*)entry);
		}

		LoadMapEntities((MapEnts*)entry);
	}
}

void PatchMW2_EntsFiles()
{

}