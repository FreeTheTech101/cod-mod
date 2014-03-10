// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: The will of a single man. Makarov's men, fighting
//          Shepard's men. That, or a beautiful bridge...
//          about to be destroyed by some weird warfare.
//
// Initial author: NTAuthority
// Started: 2012-10-20
// ==========================================================

#include "StdInc.h"

// New sound file 
const char* newSound = "hz_af_chase_boatride_lr_r1.mp3";

snd_alias_list_t* FindSoundAliasHookFunc(assetType_t type, const char* name)
{
	snd_alias_list_t* aliases = (snd_alias_list_t*)DB_FindXAssetHeader(type, name);

	if (!_stricmp(name, "music_opening"))
	{
		if(strcmp(aliases->aliases->stream->file, newSound))
		{
			// Replace sound file
			aliases->aliases->stream->file = newSound;
		}
	}

	return aliases;
}

void PatchMW2_MusicalTalent()
{
	call(findSoundAliasHookLoc, FindSoundAliasHookFunc, PATCH_CALL);
}