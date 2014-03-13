// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Modern Warfare 2 patches: experimental dev code
//
// Initial author: NTAuthority
// Started: 2011-05-09
// ==========================================================

#include "stdinc.h"
#include "direct.h"

typedef void* (__cdecl * LoadRawFile_t)(const char* filename, void* buffer, size_t buflen);
LoadRawFile_t LoadRawFile = (LoadRawFile_t)0x46DA60;

#define RAWFILE_BUFSIZE 2097152
static char rawFileBuffer[RAWFILE_BUFSIZE];

typedef struct weaponEntry_s
{
	const char* name;
	int offset;
	int type;
} weaponEntry_t;

#define NUM_ENTRIES 672
#define NUM_VEHICLE_ENTRIES 143

#define WEAPON_DO_ARRAY(ar, c) \
{ \
	for (int _l_1 = 0; _l_1 < c; _l_1++) \
	{ \
		if (*(int*)data == _l_1) \
		{ \
			fprintf(file, "%s", ((char**)ar)[_l_1]); /* why do I have to explicitly define ar as being a char**? */ \
		} \
	} \
}

weaponEntry_t* weaponEntries = (weaponEntry_t*)0x739C68;
weaponEntry_t* vehicleEntries = (weaponEntry_t*)0x73BEA8;

const char* SL_ConvertToString(unsigned int sl)
{
	__asm
	{
		push sl
		mov eax, 40E990h
		call eax
		add esp, 4h
		mov sl, eax
	}

	return (const char*)sl;
}

void DumpNoteTrackEntry(FILE* file, int type, char* data, char* data2)
{
	switch (type)
	{
		case 39: // notetrack-to-sound
		case 40: // notetrack-to-rumble
			{
				// as seen last time, SL is usable now.
				short* keys = *(short**)data;
				short* values = *(short**)data2;

				for (int i = 0; i < 16; i++)
				{
					short key = keys[i];
					short value = values[i];

					if (key)
					{
						fprintf(file, "%s %s%s", SL_ConvertToString(key), SL_ConvertToString(value), (i == 15 || keys[i + 1] == 0) ? "" : "\n"); // it seems like a newline is needed for all but the last tag
					}
				}
			}
			break;
	}
}

bool thisIsWeaponfile = true;

void DumpEntry(FILE* file, weaponEntry_t* entry, char* data)
{
	int type = entry->type;

	switch (type)
	{
	case 0: // string
		{
			char* str = (char*)(*(DWORD_PTR*)data);

			if (str && (int)str != 0xFFFFFFFF && (int)str > 16)
			{
				fprintf(file, "%s", str);
			}
			break;
		}
	case 1: // char[1024]
	case 2: // char[64]
	case 3: // char[256]
		{
			fprintf(file, "%s", *(char**)data);
			break;
		}
	case 4:
	case 5: // bool-as-int
		{
			int number = *(int*)data;

			fprintf(file, "%d", number);
			break;
		}
	case 6:
		{
			bool boolean = *(bool*)data;

			fprintf(file, "%d", (boolean) ? 1 : 0);
			break;
		}
	case 7:
		{
			float number = *(float*)data;

			fprintf(file, "%g", number);
			break;
		}
	case 8:
		{
			float number = *(float*)data;

			fprintf(file, "%g", (number / 17.6f)); // miles/hour - inch/sec
			break;
		}
	case 9:
		{
			int number = *(int*)data;

			fprintf(file, "%g", (number / 1000.0f));
			break;
		}
	case 16:
		if (thisIsWeaponfile)
		{
			WEAPON_DO_ARRAY(0x739B08, 4) //bullet
		}
		else
		{
			WEAPON_DO_ARRAY(0x73BE80, 7) //4 wheel
		}
		break;
	case 17:
		if (thisIsWeaponfile)
		{
			WEAPON_DO_ARRAY(0x739B18, 12) //rifle
		}
		else
		{
			WEAPON_DO_ARRAY(0x73BE9C, 3) //front
		}
		break;
	case 18:
		if (thisIsWeaponfile)
		{
			WEAPON_DO_ARRAY(0x739BD0, 2) //none (crosshair...)
		}
		else
		{
			WEAPON_DO_ARRAY(0x73BE9C, 3) //front
		}
		break;
	case 19:
		if (thisIsWeaponfile)
		{
			WEAPON_DO_ARRAY(0x739B78, 4) //none (small, medium, large...)
		}
		else
		{
			WEAPON_DO_ARRAY(0x73BE9C, 3) //front
		}
		break;
	case 20:
		WEAPON_DO_ARRAY(0x739B88, 11) //none (bullet_small...)
		break;
	case 21:
		WEAPON_DO_ARRAY(0x739BD8, 3) //stand
		break;
	case 22:

		// Hotfix for explosion type bug.
		if(!strcmp(entry->name, "projExplosionType"))
		{
			fprintf(file, "grenade");
			break;
		}

		WEAPON_DO_ARRAY(0x739C2C, 7) //knife
		break;
	case 23:
		WEAPON_DO_ARRAY(0x739BE4, 6) //none (Frag Grenade)
		break;
	case 24:
		WEAPON_DO_ARRAY(0x1A6CAB0, *(int*)0x1A6BF64) //unknown
		break;
	case 25:
		WEAPON_DO_ARRAY(0x739BFC, 3) //none (Pip-On-A-Stick)
		break;
	case 26:
		WEAPON_DO_ARRAY(0x739C08, 4) //none (below bouncing diamond)
		break;
	case 28:
		WEAPON_DO_ARRAY(0x739C18, 6) //don't stick
		break;
	case 29:
		WEAPON_DO_ARRAY(0x739C30, 3) //none (javelin, turret scope...)
		break;
	case 30:
		WEAPON_DO_ARRAY(0x739B48, 6) //primary
		break;
	case 31:
		WEAPON_DO_ARRAY(0x739B60, 6) //Full Auto
		break;
	case 32:
		WEAPON_DO_ARRAY(0x739C3C, 7) //none (magazine, short magazine)
		break;
	case 33:
		WEAPON_DO_ARRAY(0x739C58, 3) //1:1, 2:1, 4:1
		break;
	//case 34:
		//WEAPON_DO_ARRAY(0x739C68, 1) //Displayname
		//break;
	case 34:
		WEAPON_DO_ARRAY(0x739C58, 3) // same as 33
		break;
	case 35:
		WEAPON_DO_ARRAY(0x739C58, 3) // same as 33
		break;
	case 36:
		WEAPON_DO_ARRAY(0x739C58, 3) // same as 33
		break;
	case 37:
		WEAPON_DO_ARRAY(0x739C58, 3) // same as 33
		break;
	case 13: // phys collmap
		break;
	case 10: // fx
	case 11: // xmodel
	case 12: // material
	case 14: // soundalias
	case 15: // tracer
		{
			BYTE* model = (BYTE*)(*(DWORD_PTR*)data);

			if (model)
			{
				char* str = (char*)(*(DWORD_PTR*)model);

				if (str)
				{
					fprintf(file, "%s", str);
				}
			}
			break;
		}
	case 38: // hideTags
		{
			// hopefully we'll have useful shit here, as if we don't, we're screwed
			// for some reason this code made me go deja vu - seems like I've done SL_ConvertToString before.
			short* tags = (short*)data;
			for (int i = 0; i < 32; i++)
			{
				short tag = tags[i];

				if (tag)
				{
					fprintf(file, "%s%s", SL_ConvertToString(tag), (i == 31 || tags[i + 1] == 0) ? "" : "\n"); // it seems like a newline is needed for all but the last tag
				}
			}
			break;
		}
	case 27: // bounceSound; surface sound
		{
			char*** dat = *(char****)data;
			if (dat != 0)
			{
				char bounceName[128];
				strcpy(bounceName, **dat);
				strrchr(bounceName, '_')[0] = '\0';

				fprintf(file, "%s", bounceName);
			}
			break;
		}
	default:
		fprintf(file, "lol no type %d", type);
		break;
	}
}

typedef struct 
{
	int offsetStart;
	int pointerOrigin;
} rtOffsetMap_t;

#define NUM_OFFSET_MAPS 14

static rtOffsetMap_t offsetMap[] =
{
	{ 116, 4 },
	{ 1784, 12 },
	{ 1848, 16 },
	{ 1996, 120 },
	{ 2060, 128 },
	{ 2208, 132 },
	{ 2356, 140 },
	{ 2388, 144 },
	{ 2420, 148 },
	{ 2452, 152 },
	{ 2484, 588 },
	{ 2548, 1208 },
	{ 2672, 1212 },
	{ 2796, 1576 },
	{ 0, 0 }
};

char* MapOffsetToPointer(char* origin, int offset)
{
	if (!thisIsWeaponfile)
	{
		return (origin + offset);
	}

	for (int i = (NUM_OFFSET_MAPS - 1); i >= 0; i--)
	{
		rtOffsetMap_t* current = &offsetMap[i];
		rtOffsetMap_t* next = &offsetMap[i + 1];

		int max = next->offsetStart;
		if (max == 0) max = 0xFFFFFF;

		if (offset >= current->offsetStart && offset < max)
		{
			char* pointer = *(char**)MapOffsetToPointer(origin, current->pointerOrigin);
			return (pointer + (offset - current->offsetStart));
		}
	}

	return (origin + offset);
}

void DumpWeaponFile(FILE* file, char* data)
{
	for (int i = 0; i < ((thisIsWeaponfile) ? NUM_ENTRIES : NUM_VEHICLE_ENTRIES); i++)
	{
		weaponEntry_t* entry = (thisIsWeaponfile) ? &weaponEntries[i] : &vehicleEntries[i];

		char* ptr = MapOffsetToPointer(data, entry->offset);//(data + entry->offset);

		// quick patch to not export empty models (causes too large weaponfiles)
		if (entry->type == 11 && *(int*)ptr == 0)
		{
			continue;
		}

		// same for empty strings
		if (entry->type == 0)
		{
			char* str = (char*)(*(DWORD_PTR*)ptr);

			if(!(str && (int)str != 0xFFFFFFFF && (int)str > 16 && *str != 0)) // It could have been that easy :D
			{
				continue;
			}
		}

		fprintf(file, "%s\\", entry->name);

		/*if (entry->offset > 116)
		{
			ptr = *(BYTE**)(data + 4);
			ptr += (entry->offset - 116);
		}*/

		if (entry->type == 39 || entry->type == 40)
		{
			DumpNoteTrackEntry(file, entry->type, MapOffsetToPointer(data, 140), MapOffsetToPointer(data, 144));
		}
		else
		{
			DumpEntry(file, entry, ptr);
		}

		if (i < (((thisIsWeaponfile) ? NUM_ENTRIES : NUM_VEHICLE_ENTRIES) - 1))
		{
			fprintf(file, "\\");
		}
	}
}

void DumpWeaponTypes(FILE* file)
{
	for (int i = 0; i < NUM_ENTRIES; i++)
	{
		weaponEntry_t* entry = &weaponEntries[i];

		//fprintf(file, "%s\\", entry->name);

		//DumpEntry(file, entry->type, (data + entry->offset));

		fprintf(file, "+0x%x:\t%s", entry->offset, entry->name);

		switch (entry->type)
		{
		case 0:
			fprintf(file, " (string)");
			break;
		case 7:
			fprintf(file, " (float)");
			break;
		case 8:
			fprintf(file, " (float / 17.6)");
			break;
		case 9:
			fprintf(file, " (float / 1000.0)");
			break;
		case 11:
			fprintf(file, " (xmodel)");
			break;
		case 0xD:
			fprintf(file, " (phys collmap)");
			break;
		default:
			fprintf(file, " (type %d)", entry->type);
			break;
		}

		fprintf(file, "\n");
	}
}

StompHook weaponFileHook;
DWORD weaponFileHookLoc = 0x659E00;

void* WeaponFileHookFunc(const char* filename)
{
	if (FS_ReadFile(va("weapons/sp/%s", filename), NULL) > 0)
	{
		return ((void* (*)(const char*))0x659DA0)(filename); // BG_LoadWeaponDef_LoadObj
	}

	char* file = (char*)DB_FindXAssetHeader(0x1C, filename);

	if (GAME_FLAG(GAME_FLAG_DUMPDATA))
	{
		_mkdir("raw");
		_mkdir("raw\\weapons");
		_mkdir("raw\\weapons\\sp");

		char dumpfile[512];
		strcpy(dumpfile, "raw\\weapons\\sp\\");
		strcat(dumpfile, filename);

		FILE* dump = fopen(dumpfile, "w");
		fprintf(dump, "WEAPONFILE\\");

		thisIsWeaponfile = true;
		DumpWeaponFile(dump, file);

		fclose(dump);
	}
	
	return file;
}

void __declspec(naked) WeaponFileHookStub()
{
	__asm jmp WeaponFileHookFunc
}

StompHook vehicleFileHook;
DWORD vehicleFileHookLoc = 0x667D90;

void* G_LoadVehicleDef(const char* filename);

void* VehicleFileHookFunc(const char* filename)
{
	if (FS_ReadFile(va("vehicles/%s", filename), NULL) > 0)
	{
		//return G_LoadVehicleDef(filename);
	}

	char* file = (char*)DB_FindXAssetHeader(ASSET_TYPE_VEHICLE, filename);

	if (GAME_FLAG(GAME_FLAG_DUMPDATA))
	{
		_mkdir("raw");
		_mkdir("raw\\vehicles");
		_mkdir("raw\\vehicles\\sp");

		char dumpfile[512];
		strcpy(dumpfile, "raw\\vehicles\\sp\\");
		strcat(dumpfile, filename);

		FILE* dump = fopen(dumpfile, "w");
		fprintf(dump, "VEHICLEFILE\\");

		thisIsWeaponfile = false;
		DumpWeaponFile(dump, file);

		fclose(dump);
	}

	return file;
}

void PatchMW2_Experimental()
{
	// weapon asset existence check
	nop(0x43E1D8, 5); // find asset header
	nop(0x43E1E0, 5); // is asset default
	nop(0x43E1EA, 2); // jump

	// Ignore missing default weapon
	*(BYTE*)0x659DBE = 0xEB;

	weaponFileHook.initialize(weaponFileHookLoc, WeaponFileHookStub);
	weaponFileHook.installHook();

	vehicleFileHook.initialize(vehicleFileHookLoc, VehicleFileHookFunc);
	vehicleFileHook.installHook();
}