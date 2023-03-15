/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: DriverD3D11.cpp
 *			Purpose: C++ Rendering Driver for Direct 3D 11
 *
 * =====================================================================
 */

#include "DriverD3D11.h"

#ifdef XPLICIT_WINDOWS

namespace Xplicit::Renderer
{
	DriverSystemD3D11::DriverSystemD3D11(HWND hwnd)
		: m_swap_desc(), m_private()
	{
		memset(&m_swap_desc, 0, sizeof(DXGI_SWAP_CHAIN_DESC));
		m_swap_desc.BufferCount = 1;
		m_swap_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_swap_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		m_private.WindowHandle = hwnd;
		
		m_swap_desc.SampleDesc.Count = 1;
		m_swap_desc.Windowed = true;

		m_swap_desc.OutputWindow = m_private.WindowHandle;
	}

	DriverSystemD3D11::~DriverSystemD3D11() 
	{
		
	}

	const char* DriverSystemD3D11::name() noexcept { return ("DriverSystemD3D11"); }
	RENDER_SYSTEM DriverSystemD3D11::api() { return RENDER_SYSTEM::DIRECT3D11; }

	std::unique_ptr<DriverSystemD3D11> make_driver_system_d3d11(HWND hwnd) 
	{ 
		assert(hwnd);
		return std::make_unique<DriverSystemD3D11>(hwnd); 
	}
}

#endif // XPLICIT_WINDOWS