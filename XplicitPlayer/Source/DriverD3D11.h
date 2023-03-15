/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright XPX, all rights reserved.
 *
 *			File: DriverD3D11.h
 *			Purpose: C++ Rendering Driver for Direct3D 11
 *
 * =====================================================================
 */

#pragma once

#include "DriverSystem.h"

#ifdef XPLICIT_WINDOWS

#include <wrl.h> /* Microsoft::WRL::ComPtr */
#include <Avx.h>

#include <dxgi.h>
#include <d3d11.h>

#include <nuklear/nuklear.h>
#include <nuklear/nuklear_d3d11.h>
#include <nuklear/nuklear_d3d11_pixel_shader.h>
#include <nuklear/nuklear_d3d11_vertex_shader.h>

#include <nuklear/nuklear.h>
#include <Instance.h>

namespace Xplicit::Renderer
{
	class DriverSystemD3D11 : public DriverSystem
	{
	public:
		DriverSystemD3D11() = delete;

	public:
		DriverSystemD3D11(HWND hwnd);
		virtual ~DriverSystemD3D11();

		DriverSystemD3D11& operator=(const DriverSystemD3D11&) = default;
		DriverSystemD3D11(const DriverSystemD3D11&) = default;

		virtual const char* name() noexcept override;
		virtual RENDER_SYSTEM api() override;

		struct PrivateData
		{
			HWND WindowHandle;

			Microsoft::WRL::ComPtr<ID3D11Device> Device;
			Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceCtx;
		};

	private:
		DXGI_SWAP_CHAIN_DESC m_swap_desc;
		PrivateData m_private;

	};

	std::unique_ptr<DriverSystemD3D11> make_driver_system_d3d11(HWND hwnd);
}

#else

#error You need Vulkan support!

#endif
