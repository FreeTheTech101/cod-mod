// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Material/image modification code
//
// Initial author: NTAuthority
// Started: 2011-06-09
// ==========================================================

#include "StdInc.h"
#include <google/dense_hash_map>
#include <string>
#include <xhash>
#include "cursor.h"

using google::dense_hash_map;

//static dense_hash_map<const char*, Material*, asset_hash<const char*>> materialTable;
static dense_hash_map<std::string, Material*, stdext::hash_compare<std::string>> materialTable;

unsigned int R_HashString(const char* string)
{
	unsigned int hash = 0;

	while (*string)
	{
		hash = (*string | 0x20) ^ (33 * hash);
		string++;
	}

	return hash;
}

void Image_Setup(GfxImage* image, short width, short height, short depth, unsigned int flags, int format)
{
	DWORD func = (version == 159 ? 0x5411D0 : 0x540970);

	__asm
	{
		push edi
		mov eax, image
		mov di, width
		push format
		push flags
		push 0
		push depth
		push height
		call func
		add esp, 14h
		pop edi
	}
}

// a3 is 'disable data needed for smoothing' or so, maybe 'do not auto-generate mipmaps'?
GfxImage* Image_Alloc(const char* name, char a2, char a3, char a4)
{
	GfxImage* memory = (GfxImage*)malloc(sizeof(GfxImage) + strlen(name) + 1);
	memset(memory, 0, sizeof(GfxImage));

	memory->a2 = a2;
	memory->a3 = a3;
	memory->a4 = a4;
	memory->name = ((char*)memory) + sizeof(GfxImage);
	strcpy(memory->name, name);

	return memory;
}

GfxImage* Image_Load(const char* name)
{
	GfxImage* image = Image_Alloc(name, 3, 0, 0);
	if(version == 159)
	{
		Image_LoadFromFileWithReader(image, (_Reader_t)0x48DD10);
	}
	else
	{
		Image_LoadFromFileWithReader(image, (_Reader_t)0x442130);
	}
	return image;
}

static unsigned char stateMapCopy[8];
static bool stateMapCopied = false;

static void Material_InitializeStateMap()
{
	if (stateMapCopied) return;

	Material* base = (Material*)DB_FindXAssetHeader(ASSET_TYPE_MATERIAL, "ui_cursor");
	memcpy(stateMapCopy, base->stateMap, 8);
	stateMapCopied = true;
}

Material* Material_CreateWithImage(const char* name, GfxImage* image)
{
	Material* material = new Material;
	memset(material, 0, sizeof(Material));

	material->maps = new MaterialTextureDef;
	memset(material->maps, 0, sizeof(MaterialTextureDef));

	Material_InitializeStateMap();

	material->name = name;
	//material->flags = 0x2F00;
	material->flags = 0x2200;
	material->animationX = 1;
	material->animationY = 1;
	material->unknown2 = 0xFFFFFFFF;
	material->unknown3 = 0xFFFFFF00;
	memset(material->unknown4, 0xFF, sizeof(material->unknown4));
	material->numMaps = 1;
	material->unknownCount2 = 1;
	material->unknown6 = 3;
	material->unknown7 = 4;
	material->techniqueSet = (MaterialTechniqueSet*)DB_FindXAssetHeader(ASSET_TYPE_TECHSET, "2d");

	material->maps->typeHash = R_HashString("colorMap");
	material->maps->firstCharacter = 'c';
	material->maps->secondLastCharacter = 'p';
	material->maps->unknown = 0xE2; // causes some nice filtering
	material->maps->image = image;

	material->stateMap = stateMapCopy;

	return material;
}

Material* Material_Create(const char* name, const char* imageFile)
{
	return Material_CreateWithImage(name, Image_Load(imageFile));
}

Material* Material_Obtain(const char* name, const char* imageFile)
{
	if (!materialTable[name])
	{
		//Com_Printf(1, "Material %s: Loading now\n", name);
		materialTable[name] = Material_Create(name, imageFile);
	} else {
		//Com_Printf(1, "Material %s: Already loaded, reusing\n", name);
	}


	return materialTable[name];
}

Material* Material_ObtainImage(const char* name, GfxImage* image)
{
	if (!materialTable[name])
	{
		materialTable[name] = Material_CreateWithImage(name, image);
	}

	return materialTable[name];
}

void Material_Delete(const char* name, const char* imageCondition)
{
	if (materialTable[name])
	{
		if (!imageCondition)
		{
			materialTable[name] = NULL;
		}
		else
		{
			if (!_stricmp(materialTable[name]->maps[0].image->name, imageCondition))
			{
				Image_Release(materialTable[name]->maps[0].image);
				free(materialTable[name]->maps[0].image);
				materialTable[name] = NULL;
			}
		}
	}
}

Material* loadCustomCursor(const char* filename)
{
	if (!materialTable[filename])
	{
		CreateDirectory(BASEGAME, NULL);
		CreateDirectory(BASEGAME "/images", NULL);
		char* image_name = "iw4_cursor_temp";
		const char* full_name = va(BASEGAME "/images/%s.iwi", image_name);

		FILE* fp = fopen(full_name, "wb");
		if(fp)
		{
			fwrite(cursor, sizeof(char), sizeof(cursor), fp);
			fclose(fp);

			materialTable[filename] = Material_Create(filename, image_name);

			remove(full_name);
		}
		else
		{
			materialTable[filename] = Material_Create(filename, "3_cursor3");
		}
	}

	return Material_Obtain(filename, filename);
}

Material* Material_Register(const char* filename)
{
	if (!_stricmp(filename, "ui_cursor"))
	{
		return loadCustomCursor(filename);
	}

	return NULL;
}

StompHook materialLoadHook;

void* MaterialLoadHookFunc(assetType_t type, const char* filename)
{
	Material* material = Material_Register(filename);

	if (material)
	{
		return material;
	}

	return (Material*)DB_FindXAssetHeader(ASSET_TYPE_MATERIAL, filename);
}

void Material_TableDebug_f()
{
	if (Cmd_Argc() < 2)
	{
		Com_Printf(0, "Usage: %s [material name]\n", Cmd_Argv(0));
		return;
	}

	Material* material = materialTable[Cmd_Argv(1)];
	Com_Printf(0, "%s is actually %s\n", Cmd_Argv(1), material->name);
}

cmd_function_t tDebugCmd;

void PatchMW2_Materialism()
{
	materialTable.set_empty_key("");

	materialLoadHook.initialize(materialLoadHookLoc, MaterialLoadHookFunc);
	materialLoadHook.installHook();

	Cmd_AddCommand("materialTableDebug", Material_TableDebug_f, &tDebugCmd, 0);
}


void ForAllCustomImages(void (*handler)(void*))
{
	for (google::dense_hash_map<std::string, Material*>::const_iterator iter = materialTable.begin(); iter != materialTable.end(); iter++)
	{
		Material* material = (*iter).second;

		for (int i = 0; i < material->numMaps; i++)
		{
			handler(material->maps[i].image);
		}
	}
}

void ClearAllCustomMaterials()
{
	materialTable.clear();
}