/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "FX.h"

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

		if (!mExplodeTex)
		{
			if (XPLICIT_HTTP.download("xasset://Library/fire.bmp", dir))
			{
				mExplodeTex = RENDER->getVideoDriver()->getTexture(dir.c_str());
			}
		}

		if (mExplodeTex)
		{
			mBillboard = RENDER->getSceneManager()->addBillboardSceneNode();

			mBillboard->setScale(scale);
			mBillboard->setPosition(pos);
			mBillboard->setMaterialTexture(0, mExplodeTex);
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