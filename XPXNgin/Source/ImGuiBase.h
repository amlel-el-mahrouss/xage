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
#include "DriverD2D.h"

#include <SpriteFont.h>

#define imgui_slots public

namespace XPX
{
	namespace ImGUI
	{
		typedef Color<float> ImColor;
		typedef RECT ImRect;

		class XPLICIT_API UIFrame final
		{
		public:
			UIFrame() = default;
			~UIFrame() = default;

		public:
			XPLICIT_COPY_DEFAULT(UIFrame);

		imgui_slots:
			ImColor BackgroundHoverColor{ 0xA7, 0x0C, 0x0C, 0x0C };
			ImColor BackgroundColor{ 0xA5, 0x26, 0x26, 0x26 };
			ImColor TextColor{ 0xFF, 0xFF, 0xFF, 0xFF };

		imgui_slots:
			std::uint32_t X{ 0 };
			std::uint32_t Y{ 0 };
			std::uint32_t W{ 0 };
			std::uint32_t H{ 0 };

		private:
			struct ImGUIUIView : Renderer::D2D::UIView
			{
				void operator()(Renderer::D2D::DriverSystemD2D* pD2dDriver) override
				{
					pD2dDriver->draw_rectangle()
				}

			};

		public:
			virtual void update(ImColor clr) noexcept
			{
				if (W < 1 ||
					H < 1)
					return;

				RENDERER->getVideoDriver()->draw2DRectangle(clr,
					irr::core::recti(position2di(X, Y), dimension2d(W, H)),
					nullptr);
			}

			virtual bool in_region() noexcept
			{
				POINT p;

				if (ScreenToClient(RENDERER->get().pWindowHandle, &p))
				{
					return ((p.x > X) &&
						(p.x < X + W)) &&
						(p.y > Y) &&
						(p.y < Y + H);
				}

				return false;
			}

		};

		class XPLICIT_API UIFontHelper final
		{
		public:
			typedef std::shared_ptr<DirectX::SpriteFont> FontPtr;

			static FontPtr get_font(const PChar* path) 
			{
				return std::make_unique<DirectX::SpriteFont>(RENDERER->get().pDevice, path, true);
			}

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
					XPLICIT_GET_DATA_DIR_W(dir);
					dir += L"UIFont14.spritefont";

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

			static PString get_title_path() noexcept
			{
				XPLICIT_GET_DATA_DIR_W(dir);
				dir += L"UIFont22.spritefont";

				return dir;
			}

			static PString get_label_path() noexcept
			{
				XPLICIT_GET_DATA_DIR_W(dir);
				dir += L"UIFont18.spritefont";

				return dir;
			}

		};
	}
}