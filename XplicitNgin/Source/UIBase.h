/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 *			File: UIFrameworkBase.h
 *			Purpose: Core UI Component and logic.
 * 
 * =====================================================================
 */

#pragma once

#include "Root.h"
#include "Xplicit.h"

#define __RENDERER_IRR__ 1

#define slots public

namespace Xplicit
{
	namespace ImGUI
	{
		class XPLICIT_API UIFrame final
		{
		public:
			UIFrame() = default;
			~UIFrame() = default;

		public:
			XPLICIT_COPY_DEFAULT(UIFrame);

		slots:
			std::uint32_t WhiteScheme{ 0xFFFFFF };
			std::uint32_t DarkScheme{ 0x282121 };
			std::uint32_t Scheme{ WhiteScheme };

		slots:
			std::uint32_t X{ 0 };
			std::uint32_t Y{ 0 };
			std::uint32_t W{ 0 };
			std::uint32_t H{ 0 };

		public:
			virtual void update() noexcept
			{
#ifdef __RENDERER_IRR__
				RENDER->getVideoDriver()->draw2DRectangle(irr::video::SColor(this->Scheme),
					irr::core::recti(X, Y, W, H),
					nullptr);
#endif
			}

			virtual bool in_region() noexcept
			{
#ifdef __RENDERER_IRR__
				return RENDER->getCursorControl()->getPosition().X > X &&
					RENDER->getCursorControl()->getPosition().Y < Y;
#endif
			}

			virtual bool in_region(irr::core::vector2di& dim2d) noexcept
			{
#ifdef __RENDERER_IRR__
				return dim2d.X > X &&
					dim2d.Y < Y;
#endif
			}

		};

		class XPLICIT_API UIFont final
		{
		public:
#ifdef __RENDERER_IRR__
			typedef irr::gui::IGUIFont* FontPtr;

			FontPtr get_font(const char* path) { return RENDER->getGUIEnvironment()->getFont(path); }
#endif

		};
	}
}