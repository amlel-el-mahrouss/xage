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
#include <NplicitNgine.h>
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

inline auto LoadSkybox(XPX::String skybox_prefix)
{
	XPLICIT_GET_DATA_DIR(DIR);

	DIR += "Textures/";
	DIR += skybox_prefix;

	XPX::String up = DIR + "_up.png";
	XPX::String dn = DIR + "_dn.png";
	XPX::String lf = DIR + "_lf.png";
	XPX::String rt = DIR + "_rt.png";
	XPX::String ft = DIR + "_ft.png";
	XPX::String bk = DIR + "_bk.png";

	auto skybox = CAD->getSceneManager()->addSkyBoxSceneNode(CAD->getVideoDriver()->getTexture(up.c_str()),
		CAD->getVideoDriver()->getTexture(dn.c_str()),
		CAD->getVideoDriver()->getTexture(lf.c_str()),
		CAD->getVideoDriver()->getTexture(rt.c_str()),
		CAD->getVideoDriver()->getTexture(ft.c_str()),
		CAD->getVideoDriver()->getTexture(bk.c_str()));

	return skybox;
}

namespace XPX
{
	class ExplosionFX final
	{
	public:
		ExplosionFX(vector3df& pos, vector3df& scale);
		~ExplosionFX();

	public:
		XPLICIT_COPY_DELETE(ExplosionFX);

	};
}