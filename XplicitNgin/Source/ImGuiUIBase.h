/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 *			File: UIFrameworkBase.h
 *			Purpose: Immediate UI Framework
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
		typedef SColor ImColor;

		class XPLICIT_API UIFrame final
		{
		public:
			UIFrame() = default;
			~UIFrame() = default;

		public:
			XPLICIT_COPY_DEFAULT(UIFrame);

		slots:
			ImColor BackgroundHoverColor{ 0xA7, 0x0C, 0x0C, 0x0C };
			ImColor BackgroundColor{ 0xA5, 0x26, 0x26, 0x26 };
			ImColor TextColor{ 0xFF, 0xFF, 0xFF, 0xFF };

		slots:
			std::uint32_t X{ 0 };
			std::uint32_t Y{ 0 };
			std::uint32_t W{ 0 };
			std::uint32_t H{ 0 };

		public:
			virtual void update(SColor clr) noexcept
			{
#ifdef __RENDERER_IRR__
				RENDER->getVideoDriver()->draw2DRectangle(clr,
					irr::core::recti(position2di(X, Y), dimension2d(W, H)),
					nullptr);
#endif // __RENDERER_IRR__
			}

			virtual bool in_region() noexcept
			{
#ifdef __RENDERER_IRR__
				return (RENDER->getCursorControl()->getPosition().X > X && RENDER->getCursorControl()->getPosition().X < (W + X)) &&
					(RENDER->getCursorControl()->getPosition().Y > Y && RENDER->getCursorControl()->getPosition().Y < (H + Y));
#endif // __RENDERER_IRR__
			}

			virtual bool in_region(irr::core::vector2di& dim2d) noexcept
			{
#ifdef __RENDERER_IRR__
				return (dim2d.X > X && dim2d.X < W) &&
					(dim2d.Y > Y && dim2d.Y < H);
#endif // __RENDERER_IRR__
			}

		};

		class XPLICIT_API UIFont final
		{
		public:
#ifdef __RENDERER_IRR__
			typedef irr::gui::IGUIFont* FontPtr;

			static FontPtr get_font(const char* path) { return RENDER->getGUIEnvironment()->getFont(path); }

			static FontPtr get_title_font() noexcept
			{
				static FontPtr fnt = nullptr;

				if (!fnt)
				{
					XPLICIT_GET_DATA_DIR(dir);
					dir += "GameFont22.bmp";

					fnt = get_font(dir.c_str());
				}

				return fnt;
			}

			static FontPtr get_label_font() noexcept
			{
				static FontPtr fnt = nullptr;

				if (!fnt)
				{
					XPLICIT_GET_DATA_DIR(dir);
					dir += "GameFont18.bmp";

					fnt = get_font(dir.c_str());
				}

				return fnt;
			}

			static String get_title_path() noexcept
			{
				XPLICIT_GET_DATA_DIR(dir);
				dir += "GameFont22.bmp";

				return dir;
			}

			static String get_label_path() noexcept
			{
				XPLICIT_GET_DATA_DIR(dir);
				dir += "GameFont18.bmp";

				return dir;
			}

#endif // __RENDERER_IRR__

		};
	}
}