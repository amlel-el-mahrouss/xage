/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <cad-engine/IBillboardSceneNode.h>
#include <XHTTPManager.h>
#include <Xplicit.h>
#include <Root.h>

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