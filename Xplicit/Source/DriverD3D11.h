/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright XPX, all rights reserved.
 *
 *			File: DriverD3D11.h
 *			Purpose: C++ Rendering Driver for Direct3D 11
 *			Very Low Level by the way.
 *
 * =====================================================================
 */

#pragma once

#include "DriverSystem.h"

#ifdef XPLICIT_WINDOWS

#include "Xplicit.h"
#include "Avx.h"

#include <wrl.h> /* Microsoft::WRL::ComPtr */
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcommon.h>

/* Include nuklear, for debugging */

#ifdef XPLICIT_DEBUG

#include <nuklear/nuklear.h>
#include <nuklear/nuklear_d3d11.h>
#include <nuklear/nuklear_d3d11_pixel_shader.h>
#include <nuklear/nuklear_d3d11_vertex_shader.h>

#include <nuklear/nuklear.h>

#endif

#define XPLICIT_VERTEX_SHADER "vs_5_0"
#define XPLICIT_PIXEL_SHADER "ps_5_0"
#define XPLICIT_HULL_SHADER "hs_5_0"
#define XPLICIT_COMPUTE_SHADER "ds_5_0"
#define XPLICIT_DOMAIN_SHADER "ds_5_0"
#define XPLICIT_GEOMETRY_SHADER "gs_5_0"

namespace Xplicit::Renderer
{
	class XPLICIT_API DriverSystemD3D11 : public DriverSystem
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

		struct XPLICIT_API PrivateData
		{
			bool VSync;
			bool EndRendering;
			HWND WindowHandle;

			Microsoft::WRL::ComPtr<ID3D11Device> Device;
			Microsoft::WRL::ComPtr<IDXGIAdapter> Adapter;
			Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceCtx;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencil;
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget;
		};

	public:
		PrivateData& get() noexcept;

	public:
		void begin_scene(const float a, const float r, const float g, const float b) const;
		bool end_scene() const;

	public:
		void close_device() noexcept { m_private.EndRendering = true; }
		const bool& should_end() noexcept { return m_private.EndRendering; }

	private:
		DXGI_SWAP_CHAIN_DESC m_swap_desc;
		PrivateData m_private;

	};

	XPLICIT_API std::unique_ptr<DriverSystemD3D11> make_driver_system_d3d11(HWND hwnd);

	class XPLICIT_API D3D11ShaderSystem final : public ShaderSystem
	{
	public:
		D3D11ShaderSystem() = delete;

	public:
		explicit D3D11ShaderSystem(const pchar* filename, uint8_t format)
			: ShaderSystem(filename, format), m_data(nullptr)
		{
			XPLICIT_ASSERT(format != FORMAT_HLSL);
		}

		virtual ~D3D11ShaderSystem() = default;

		D3D11ShaderSystem& operator=(const D3D11ShaderSystem&) = default;
		D3D11ShaderSystem(const D3D11ShaderSystem&) = default;

		const string& shader() noexcept { return m_shader; }
		const uint8_t& type() noexcept { return m_type; }

	public:
		class ShaderData
		{
		public:
			char* entrypoint;
			char* shader_type;
			uint32_t flags1, flags2;
			ID3D10Blob** blobs;
			ID3D10Blob** error_blobs;

		};

	public:
		virtual int operator()() override;
		void set(std::shared_ptr<ShaderData> data) noexcept;

	private:
		std::shared_ptr<ShaderData> m_data;

};
}

#else



#endif
