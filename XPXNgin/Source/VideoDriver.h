/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: VideoDriver.h
 *			Purpose: Generic Video driver.
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"

#ifdef XPLICIT_WINDOWS

#include "DriverD3D11.h"

#endif

namespace XPX
{
	namespace Renderer
	{
		class VideoDriver;

		class XPLICIT_API VideoDriver final
		{
		public:
			VideoDriver(HWND hwnd, UINT width, UINT height);
			~VideoDriver();

		public:
			VideoDriver& operator=(const VideoDriver&) = default;
			VideoDriver(const VideoDriver&) = default;

		public:
			BaseRenderableComponent* get_texture(const char* path);
			BaseRenderableComponent* get_mesh(const char* path);

		private:
			std::unique_ptr<DX11::DriverSystemD3D11> m_driver;
			HWND m_hwnd;

		};
	}
}
