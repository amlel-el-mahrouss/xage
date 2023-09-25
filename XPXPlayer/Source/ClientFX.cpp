/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "ClientFX.h"
#include "SoundComponent.h"

namespace XPX
{
	ExplosionFX::ExplosionFX(vector3df& pos, vector3df& scale)
	{
		XPLICIT_GET_DATA_DIR(full_path);

		String full_sound_path;

		full_sound_path = full_path;
		full_sound_path += "Contents/ExplosionFX.wav";

		if (DownloadURL("xasset://Library/ExplosionFX.wav", full_path))
			PlaySound(full_sound_path, Vector<float>(pos.X, pos.Y, pos.Z));
	}

	ExplosionFX::~ExplosionFX() = default;
}