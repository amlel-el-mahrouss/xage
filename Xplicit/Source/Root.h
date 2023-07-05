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
#include "HelperMacros.h"

/* common engine macros for Root */
#define RENDER Xplicit::Root::get_singleton_ptr()->Ogre3D

/* resource group for XplicitPlayer */
#define XPLICIT_RES_GROUP "Ngine"

namespace Xplicit
{
	class XPLICIT_API Root final
	{
	private:
		explicit Root()
			:
			Ogre3D(nullptr),
			Ogre3D_Scene(nullptr),
			Ogre3D_Window(nullptr)
		{
			Ogre3D = new Ogre::Root("Plugins.cfg", "XplicitNgine.cfg", "XplicitNgine.log");

			Ogre3D->loadPlugin("RenderSystem_Direct3D11.dll");
			Ogre3D->loadPlugin("Plugin_ParticleFX.dll");

			Ogre::RenderSystemList renderSystems = Ogre3D->getAvailableRenderers();
			Ogre::RenderSystemList::iterator itr = renderSystems.begin();

			Ogre3D->setRenderSystem((*itr));

			Ogre3D_Window = Ogre3D->initialise(true);

			Ogre3D_Scene = Ogre3D->createSceneManager();
			Ogre3D->_setCurrentSceneManager(Ogre3D_Scene);

			this->set_title("XplicitNgin [ Place1 ]");
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
		bool set_title(const char* title) noexcept
		{
			if (!Ogre3D_Window)
				return false;

			if (title == nullptr)
				title = "XplicitNgine";

			int64_t hwnd = 0;
			Ogre3D_Window->getCustomAttribute("WINDOW", &hwnd);
			::SetWindowTextA((HWND)hwnd, title);

			return true;
		}

	public:
		Ogre::RenderWindow* Ogre3D_Window;
		Ogre::SceneManager* Ogre3D_Scene;
		Ogre::Root* Ogre3D;

	};
}
