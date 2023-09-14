/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "ClientUtils.h"
#include "SoundComponent.h"

namespace XPX
{
	Explosion::Explosion(vector3df& pos, vector3df& scale)
	{
		XPLICIT_GET_DATA_DIR(full_path);

		String full_sound_path;

		full_sound_path = full_path;
		full_sound_path += "Contents/Explosion.wav";

		if (DownloadURL("xasset://Library/Explosion.wav", full_path))
			PlaySound(full_sound_path, Vector<float>(pos.X, pos.Y, pos.Z));
	}

	Explosion::~Explosion() = default;
}