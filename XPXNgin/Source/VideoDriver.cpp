/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: VideoDriver.cpp
 *			Purpose: Abstraction behind the rendering driver.
 *
 * =====================================================================
 */

#include "VideoDriver.h"

namespace XPX::Renderer
{
	VideoDriver::VideoDriver(HWND hwnd, UINT width, UINT height)
		: m_driver(DX11::make_driver_system_d3d11(hwnd, width, height)), m_hwnd(hwnd)
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("VideoDriver::VideoDriver");
#endif
	}

	VideoDriver::~VideoDriver()
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("VideoDriver::~VideoDriver");
#endif
	}

	BaseRenderableComponent* VideoDriver::get_texture(const char* path)
	{
		return nullptr;
	}

	BaseRenderableComponent* VideoDriver::get_mesh(const char* path)
	{
		return nullptr;
	}
}
