/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "SoundComponent.h"
#include <XHTTPManager.h>
#include <Nplicit.h>
#include <NginCore.h>
#include <Root.h>

inline void PlaySound(const XPX::String& path, const XPX::Vector<float>& pos)
{
	if (auto snd = XPX::ComponentSystem::get_singleton_ptr()->get<XPX::SoundComponent>("SoundComponent");
		snd)
	{
		snd->set_position(XPX::Vector<float>(pos.X, pos.Y, pos.Z));
		snd->set_volume(1);

		snd->play(path);
	}
}

namespace XPX
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