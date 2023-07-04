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
			Ogre3D(nullptr),
			Ogre3D_Scene(nullptr),
			Ogre3D_Window(nullptr)
		{
			Ogre3D = new Ogre::Root();

#ifdef XPLICIT_DEBUG
			Ogre3D->loadPlugin("RenderSystem_Direct3D11_d.dll");
#else
			Ogre3D->loadPlugin("RenderSystem_Direct3D11.dll");
#endif

			Ogre::RenderSystemList renderSystems = Ogre3D->getAvailableRenderers();
			Ogre::RenderSystemList::iterator itr = renderSystems.begin();

			Ogre3D->setRenderSystem((*itr));

			Ogre3D_Scene = Ogre3D->createSceneManager();
			Ogre3D->_setCurrentSceneManager(Ogre3D_Scene);

			Ogre3D_Window = Ogre3D->initialise(true);
		
			Ogre::SceneManager* sceneManager = RENDER->createSceneManager();

			Root::get_singleton_ptr()->Ogre3D_Window->setVSyncEnabled(false);

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

#include "HelperMacros.h"

#define XPLICIT_RES_GROUP "XplicitNgine"
