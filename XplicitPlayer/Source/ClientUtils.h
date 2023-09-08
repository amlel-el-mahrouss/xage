/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "SoundComponent.h"
#include <XHTTPManager.h>
#include <Nplicit.h>
#include <Xplicit.h>
#include <Root.h>

inline void PlaySound(const Xplicit::String& path, const Xplicit::Vector<float>& pos)
{
	if (auto snd = Xplicit::ComponentSystem::get_singleton_ptr()->get<Xplicit::Player::SoundComponent>("SoundComponent");
		snd)
	{
		snd->set_position(Xplicit::Vector<float>(pos.X, pos.Y, pos.Z));
		snd->set_volume(1);

		snd->play(path);
	}
}

namespace Xplicit
{
	class Explosion final
	{
	public:
		Explosion(vector3df& pos, vector3df& scale);
		~Explosion();

	public:
		XPLICIT_COPY_DEFAULT(Explosion);

	};
}