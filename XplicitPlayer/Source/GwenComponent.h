/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Gwork/Gwork.h>

#include <Gwork/Controls/Canvas.h>

#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Renderers/Irrlicht.h>
#include <Gwork/Input/Irrlicht.h>
#include <Gwork/Platform.h>

#include <Component.h>
#include <Root.h>

#include "Application.h"

namespace Xplicit::Player::GWEN
{
	class GWENComponent final : public Component
	{
	public:
		explicit GWENComponent() noexcept
		{
			Gwk::Platform::RelativeToExecutablePaths local("./");
			mRenderer = new Gwk::Renderer::Irrlicht(local, RENDER);

			mSkin = new Gwk::Skin::TexturedBase(mRenderer);

			XPLICIT_ASSERT(mRenderer);
			XPLICIT_ASSERT(mSkin);

			mSkin->Init("Res/default.png");
			mSkin->SetDefaultFont("Res/urbanist.ttf", 11);

			mCanvas = new Gwk::Controls::Canvas(mSkin);

			mCanvas->SetSize(XPLICIT_DIM.X, XPLICIT_DIM.Y);
			mCanvas->SetDrawBackground(false);
		}

		~GWENComponent() override
		{
			delete mSkin;
			delete mRenderer;
			delete mCanvas;
		}

	public:
		XPLICIT_COPY_DEFAULT(GWENComponent);

	public:
		virtual bool should_update() noexcept { return false; }
		virtual void update() {}

	public:
		virtual COMPONENT_TYPE type() noexcept { return COMPONENT_HUMANOID; }
		virtual const char* name() noexcept { return "GWENComponent"; }

	public:
		virtual PHYSICS_TYPE physics() noexcept { return PHYSICS_NONE; }
		virtual bool can_collide() noexcept { return false; }
		virtual bool has_physics() noexcept { return false; }

	private:
		Gwk::Renderer::Irrlicht* mRenderer;
		Gwk::Skin::TexturedBase* mSkin;
		Gwk::Controls::Canvas* mCanvas;

	};
}