/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: UIFrameworkBase.h
 *			Purpose: Immediate UI Framework
 * 
 * =====================================================================
 */

#pragma once

#include "Root.h"
#include "NginCore.h"

#define __RENDERER_IRR__ 1

#define slots public

namespace XPX
{
	namespace ImGUI
	{
		typedef ITexture ImTexture;

		typedef SColor ImColor;
		typedef recti ImRect;

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
				if (W < 1 ||
					H < 1)
					return;

#ifdef __RENDERER_IRR__
				CAD->getVideoDriver()->draw2DRectangle(clr,
					irr::core::recti(position2di(X, Y), dimension2d(W, H)),
					nullptr);
#endif // __RENDERER_IRR__
			}

			virtual bool in_region() noexcept
			{
#ifdef __RENDERER_IRR__
				auto pos = CAD->getCursorControl()->getPosition();

				return ((pos.X > X) &&
					(pos.X < X + W)) &&
					(pos.Y > Y) &&
					(pos.Y < Y + H);
#endif
			}

		};

		class XPLICIT_API UIFontHelper final
		{
		public:
#ifdef __RENDERER_IRR__
			typedef irr::gui::IGUIFont* FontPtr;

			static FontPtr get_font(const char* path) { return CAD->getGUIEnvironment()->getFont(path); }

		public:
			static FontPtr get_title_font() noexcept
			{
				static FontPtr fnt = nullptr;

				if (!fnt)
					fnt = get_font(get_title_path().c_str());

				return fnt;
			}

			static FontPtr get_properties_font() noexcept
			{
				static FontPtr fnt = nullptr;

				if (!fnt)
				{
					XPLICIT_GET_DATA_DIR(dir);
					dir += "UIFont14.png";

					fnt = get_font(dir.c_str());
				}

				return fnt;
			}

			static FontPtr get_label_font() noexcept
			{
				static FontPtr fnt = nullptr;

				if (!fnt)
					fnt = get_font(get_label_path().c_str());

				return fnt;
			}

			static String get_title_path() noexcept
			{
				XPLICIT_GET_DATA_DIR(dir);
				dir += "UIFont22.png";

				return dir;
			}

			static String get_label_path() noexcept
			{
				XPLICIT_GET_DATA_DIR(dir);
				dir += "UIFont18.png";

				return dir;
			}

#endif // __RENDERER_IRR__

		};
	}
}