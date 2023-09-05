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
		: mBillboard(nullptr), mExplodeTex(nullptr)
	{
		XPLICIT_GET_DATA_DIR(dir);
		String explosion = dir;
		dir += "Contents/fire.bmp";

		mExplodeTex = RENDER->getVideoDriver()->getTexture(dir.c_str());

		if (mExplodeTex)
		{
			mBillboard = RENDER->getSceneManager()->addBillboardSceneNode();

			mBillboard->setScale(scale);
			mBillboard->setPosition(pos);
			mBillboard->setMaterialTexture(0, mExplodeTex);

			XPLICIT_GET_DATA_DIR(full_path);

			Xplicit::String full_sound_path;

			full_sound_path += full_path;
			full_sound_path += "Contents/";
			full_sound_path += "Explode.mp3";

			if (auto snd = ComponentSystem::get_singleton_ptr()->get<Xplicit::Player::SoundComponent>("SoundComponent"))
				snd->play(full_sound_path);
		}
	}

	Explosion::~Explosion()
	{
		if (mBillboard)
			mBillboard->drop();

		if (mExplodeTex)
			mExplodeTex->drop();
	}
}