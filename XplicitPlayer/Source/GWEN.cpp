/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "GWEN.h"

namespace Xplicit
{
	GWENManager::GWENManager(const Vector<float>& dim) noexcept
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

	GWENManager::~GWENManager()
	{
		if (Skin)
			delete Skin;

		if (Renderer)
			delete Renderer;

		if (Canvas)
			delete Canvas;
	}
}