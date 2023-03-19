/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Hazel.h
 *			Purpose: Hazel Direct3D renderer.
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
		class XPLICIT_API DeviceDriverWindows final
		{
		public:
			DeviceDriverWindows(HWND hwnd)
				: m_driver(DX11::make_driver_system_d3d11(hwnd)), m_hwnd(hwnd)
			{}

			~DeviceDriverWindows() 
			{}

			DeviceDriverWindows& operator=(const DeviceDriverWindows&) = default;
			DeviceDriverWindows(const DeviceDriverWindows&) = default;

		private:
			std::unique_ptr<DX11::DriverSystemD3D11> m_driver;
			HWND m_hwnd;

		};
	}


}

#endif
