/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
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
	class GWENManager final
	{
	public:
		explicit GWENManager(const Vector<float>& dim) noexcept;
		~GWENManager();

	public:
		XPLICIT_COPY_DEFAULT(GWENManager);

	public:
		Gwk::Renderer::Irrlicht* Renderer;
		Gwk::Skin::TexturedBase* Skin;
		Gwk::Controls::Canvas* Canvas;

	};
}