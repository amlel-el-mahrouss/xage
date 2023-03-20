/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: VideoDriver.h
 *			Purpose: Direct3D renderer.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include "DriverD3D11.h"

#ifdef XPLICIT_WINDOWS

namespace Xplicit
{
	namespace Renderer
	{
		class XPLICIT_API VideoDriver final
		{
		public:
			VideoDriver(HWND hwnd)
				: m_driver(DX11::make_driver_system_d3d11(hwnd)), m_hwnd(hwnd)
			{}

			~VideoDriver()
			{}

			VideoDriver& operator=(const VideoDriver&) = default;
			VideoDriver(const VideoDriver&) = default;

		private:
			std::unique_ptr<DX11::DriverSystemD3D11> m_driver;
			HWND m_hwnd;

		};
	}
}

#endif
