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
#include <Gwork/Controls.h>

#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Renderers/Irrlicht.h>
#include <Gwork/Input/Irrlicht.h>
#include <Gwork/Platform.h>

#include <Component.h>
#include <Nplicit.h>
#include <Root.h>

namespace Xplicit
{
	class GWENComponentManager final
	{
	public:
		explicit GWENComponentManager(const Vector<float>& dim) noexcept
		{
			Gwk::Platform::RelativeToExecutablePaths local;

			Renderer = new Gwk::Renderer::Irrlicht(local, RENDER);
			XPLICIT_ASSERT(Renderer);

			Skin = new Gwk::Skin::TexturedBase(Renderer);
			XPLICIT_ASSERT(Skin);

			Skin->Init("Res/default.png");
			Skin->SetDefaultFont("Res/urbanist.ttf", 11);

			Canvas = new Gwk::Controls::Canvas(Skin);
			XPLICIT_ASSERT(Canvas);

			Canvas->SetSize(dim.X, dim.Y);
			Canvas->SetDrawBackground(true);
			Canvas->SetBackgroundColor(Gwk::Color(255, 255, 255, 255));
		}

		~GWENComponentManager()
		{
			if (Skin)
				delete Skin;

			if (Renderer)
				delete Renderer;
			
			if (Canvas)
				delete Canvas;
		}

	public:
		XPLICIT_COPY_DEFAULT(GWENComponentManager);

	public:
		Gwk::Renderer::Irrlicht* Renderer;
		Gwk::Skin::TexturedBase* Skin;
		Gwk::Controls::Canvas* Canvas;

	};
}