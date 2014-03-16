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

// entity dumping, needs to be split?
struct MapEnts
{
	const char* name;
	const char* entitystring;
};

void DumpMapEntities(MapEnts* entities)
{
	char filename[255];

	CreateDirectory("data/maps", NULL);

	_snprintf(filename, sizeof(filename), "data/%s.ents", entities->name);

	errno = 0;
	FILE* file = fopen(filename, "w");
	if (file)
	{
		Com_Printf(0, "Dumping '%s'\n", filename);
		fwrite(entities->entitystring, 1, strlen(entities->entitystring), file);
		fclose(file);
	}
	else
	{
		Com_Printf(0, "Dumping '%s' failed: %s\n", filename, strerror(errno));
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
		if (GAME_FLAG(GAME_FLAG_DUMPDATA))
		{
			DumpMapEntities((MapEnts*)entry);
		}

		LoadMapEntities((MapEnts*)entry);
	}
}