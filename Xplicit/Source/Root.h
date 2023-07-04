/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

/* common engine macros for Root */
#define RENDER Xplicit::Root::get_singleton_ptr()->Ogre3D

namespace Xplicit
{
	class XPLICIT_API Root final
	{
	private:
		explicit Root()
			:
			Ogre3D(nullptr)
		{
			Ogre3D = new Ogre::Root("plugins.cfg");

#ifdef XPLICIT_DEBUG
			Ogre3D->loadPlugin("RenderSystem_Direct3D11_d.dll");
#else
			Ogre3D->loadPlugin("RenderSystem_Direct3D11.dll");
#endif

			Ogre::RenderSystemList renderSystems = Ogre3D->getAvailableRenderers();
			Ogre::RenderSystemList::iterator itr = renderSystems.begin();

			Ogre3D->setRenderSystem((*itr));

			Ogre3D->initialise(false);
		}

		~Root() noexcept
		{
			if (Ogre3D)
				delete Ogre3D;
		}

		Root& operator=(const Root&) = default;
		Root(const Root&) = default;

	public:
		static Root* get_singleton_ptr() noexcept
		{
			static Root* root = nullptr;

			if (!root)
				root = new Root();

			return root;
		}

	public:
		Ogre::Root* Ogre3D;

	};
}

#include "HelperMacros.h"

#define XPLICIT_RES_GROUP "XplicitNgine"
