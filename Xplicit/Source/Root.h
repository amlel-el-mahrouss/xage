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

#include <Ogre.h>
#include <lua/CLua.hpp>
#include <OgreRTShaderSystem.h>
#include <OgreApplicationContext.h>

/* common engine macros for Root */
#define RENDER Xplicit::Root::get_singleton_ptr()->Ogre3D

/* resource group for XplicitPlayer */
#define XPLICIT_RES_GROUP "Ngine"

namespace Xplicit
{
	class OgreInputListener : public OgreBites::InputListener
	{
	private:
		bool keyPressed(const OgreBites::KeyboardEvent& evt) override
		{
			KeyboardEvent = evt;
			return true;
		}

		bool keyReleased(const OgreBites::KeyboardEvent& evt) override
		{
			KeyboardEvent = evt;
			return true;
		}

		bool touchMoved(const OgreBites::TouchFingerEvent& evt)
		{
			FingerEvent = evt;
			return true;
		}

		bool touchPressed(const OgreBites::TouchFingerEvent& evt)
		{
			FingerEvent = evt;
			return true;
		}

		bool touchReleased(const OgreBites::TouchFingerEvent& evt)
		{
			FingerEvent = evt;
			return true;
		}

		bool mouseMoved(const OgreBites::MouseMotionEvent& evt)
		{
			Lua::CLuaStateManager::get_singleton_ptr()->run_string("Game:MouseMove()");

			MouseMotionEvent = evt;
			return true;
		}

		bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt)
		{
			MouseWheelEvent = evt;
			return true;
		}

		bool mousePressed(const OgreBites::MouseButtonEvent& evt)
		{
			// TODO: do not hardcode that, make something much more scalable, as we are planning to work
			// on our own renderer.
			if (evt.type == OgreBites::BUTTON_LEFT)
				Lua::CLuaStateManager::get_singleton_ptr()->run_string("Game:LeftClick()");

			if (evt.type == OgreBites::BUTTON_RIGHT)
				Lua::CLuaStateManager::get_singleton_ptr()->run_string("Game:RightClick()");

			MouseButtonEvent = evt;
			return true;
		}

		bool mouseReleased(const OgreBites::MouseButtonEvent& evt)
		{
			MouseButtonEvent = evt;
			return true;
		}

		bool textInput(const OgreBites::TextInputEvent& evt) 
		{
			TextInputEvent = evt;
			return true;
		}

		bool axisMoved(const OgreBites::AxisEvent& evt) 
		{
			AxisEvent = evt;
			return true;
		}

		bool buttonPressed(const OgreBites::ButtonEvent& evt) 
		{
			ButtonEvent = evt;
			return true;
		}

		bool buttonReleased(const OgreBites::ButtonEvent& evt) 
		{
			ButtonEvent = evt;
			return true;
		}

	public:
		OgreBites::MouseButtonEvent MouseButtonEvent;
		OgreBites::MouseMotionEvent MouseMotionEvent;
		OgreBites::MouseWheelEvent MouseWheelEvent;
		OgreBites::TextInputEvent TextInputEvent;
		OgreBites::TouchFingerEvent FingerEvent;
		OgreBites::KeyboardEvent KeyboardEvent;
		OgreBites::ButtonEvent ButtonEvent;
		OgreBites::AxisEvent AxisEvent;

	};

	class XPLICIT_API Root final : public OgreBites::ApplicationContext, public OgreInputListener
	{
	private:
		explicit Root()
			:
			Ogre3D(nullptr),
			Ogre3D_RTSS(nullptr),
			Ogre3D_Scene(nullptr),
			Ogre3D_Window(nullptr),
			Ogre3D_Light(nullptr),
			OgreBites::ApplicationContext("XplicitNgin [ Place1 ]")
		{}

	public:
		~Root() noexcept
		{
			if (Ogre3D)
				delete Ogre3D;
		}

		Root& operator=(const Root&) = default;
		Root(const Root&) = default;

		static Root* get_singleton_ptr() noexcept
		{
			static Root* root = nullptr;

			if (!root)
				root = new Root();

			return root;
		}

	public:
		void setup() override
		{
			OgreBites::ApplicationContext::setup();
		}

		bool set_title(const char* title) noexcept
		{
			if (!getRenderWindow())
				return false;

			if (title == nullptr)
				title = "XplicitNgine [ Place1 ]";

			std::int64_t hwnd = 0;

			getRenderWindow()->getCustomAttribute("WINDOW", &hwnd);
			::SetWindowTextA((HWND)hwnd, title);

			return true;
		}

	public:
		Ogre::RTShader::ShaderGenerator* Ogre3D_RTSS;
		Ogre::SceneNode* Ogre3D_Light;
		Ogre::RenderWindow* Ogre3D_Window;
		Ogre::SceneManager* Ogre3D_Scene;
		Ogre::Root* Ogre3D;

	};
}
