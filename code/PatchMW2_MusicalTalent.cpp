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

snd_alias_list_t* aliases;

void replaceMusic(const char* assetName, const char* sound)
{
	if (!_stricmp(aliases->name, assetName))
	{
		if(strcmp(aliases->aliases->stream->file, sound))
		{
			aliases->aliases->stream->file = sound;
		}
	}
}

snd_alias_list_t* FindSoundAliasHookFunc(assetType_t type, const char* name)
{
	aliases = (snd_alias_list_t*)DB_FindXAssetHeader(type, name);

	// Random replacements. Some don't fit, but I like them though. Change whatever you want :D
	replaceMusic("music_challenge_factory", "hz_estate_betrayal_LR_1.mp3");
	replaceMusic("music_mainmenu_mp", "hz_dc_burning_intropeak_LR_1.mp3");
	replaceMusic("music_opening", "hz_af_chase_boatride_lr_r1.mp3");

	//interesting sound: so_defeat_desert (only ingame)

	return aliases;
}

void PatchMW2_MusicalTalent()
{
	call(findSoundAliasHookLoc, FindSoundAliasHookFunc, PATCH_CALL);
}