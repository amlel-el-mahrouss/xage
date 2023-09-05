/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "FX.h"
#include "SoundComponent.h"

namespace Xplicit
{
	static XHTTPManager XPLICIT_HTTP;

	Explosion::Explosion(vector3df& pos, vector3df& scale)
	{
		XPLICIT_GET_DATA_DIR(full_path);

		String full_sound_path;

		full_sound_path += full_path;
		full_sound_path += "Contents/";

		full_sound_path += "Explosion.wav";

		if (auto snd = ComponentSystem::get_singleton_ptr()->get<Xplicit::Player::SoundComponent>("SoundComponent");
			snd)
		{
			snd->set_position(Xplicit::Vector<float>(pos.X, pos.Y, pos.Z));
			snd->set_volume(1);

			snd->play(full_sound_path);
		}
	}

	Explosion::~Explosion() = default;
}