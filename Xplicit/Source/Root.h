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
	class InputReceiver;

	class XPLICIT_API Root final
	{
	private:
		explicit Root()
			: 
			Keyboard(nullptr), 
			Ogre3D(nullptr)
		{
			Ogre3D = new Ogre::Root("plugins.cfg");
		}

		~Root() noexcept
		{
			if (Ogre3D)
				delete Ogre3D;

			if (Keyboard)
				delete Keyboard;

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
		InputReceiver* Keyboard;
		Ogre::Root* Ogre3D;

	};
}

#include "HelperMacros.h"

