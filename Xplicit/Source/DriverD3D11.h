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
#include "Component.h"

#ifdef XPLICIT_WINDOWS

#include "Xplicit.h"
#include "Avx.h"

#include <wrl.h> /* Microsoft::WRL::ComPtr */
#include <dxgi.h>

#include <../um/d3d11.h>
#include <../um/d3dcommon.h>

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

namespace Xplicit::Renderer::DX11
{
	class DriverSystemD3D11;
	class D3D11ShaderSystem;
	class D3D11RenderComponent;

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
			bool VSync{ false };
			char CardDesc[128];
			bool EndRendering{ false };
			HWND WindowHandle{ nullptr };

			DXGI_SWAP_CHAIN_DESC SwapDesc;
			Microsoft::WRL::ComPtr<ID3D11Device> Device;
			Microsoft::WRL::ComPtr<IDXGIAdapter> Adapter;
			Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
			Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthTexture;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> Ctx;
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterState;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencil;
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState;
		};

	private:
		void setup(); // internal directx setup.

	public:
		void begin_scene(const float a, const float r, const float g, const float b);
		bool end_scene();

		PrivateData& get() noexcept;

		void close() noexcept { m_private.EndRendering = true; }

		const bool& is_closed() noexcept { return m_private.EndRendering; }
		operator bool() { return is_closed(); }

	private:
		void handle_device_removed();
		bool check_device_removed(HRESULT hr);

	private:
		PrivateData m_private;

	};

	XPLICIT_API std::unique_ptr<DriverSystemD3D11> make_driver_system_d3d11(HWND hwnd);

	class XPLICIT_API D3D11ShaderSystem final : public virtual ShaderSystem
	{
	public:
		D3D11ShaderSystem() = delete;

	public:
		explicit D3D11ShaderSystem(const pchar* filename, uint8_t format)
			: ShaderSystem(filename, format), m_data(std::make_unique<ShaderData>())
		{
			XPLICIT_ASSERT(format == FORMAT_HLSL);
		}

		virtual ~D3D11ShaderSystem() = default;

		D3D11ShaderSystem& operator=(const D3D11ShaderSystem&) = default;
		D3D11ShaderSystem(const D3D11ShaderSystem&) = default;

	public:
		class XPLICIT_API ShaderData
		{
		public:
			char* entrypoint;
			char* shader_type;
			ID3D10Blob* blob;
			ID3D10Blob* error_blob;
			uint32_t flags1, flags2;

			Microsoft::WRL::ComPtr<ID3D11HullShader> hull;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel;
			Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex;

		};

		ShaderData* get() const;

	public:
		virtual int compile() noexcept override;

	private:
		void update(D3D11RenderComponent* component);

	private:
		std::unique_ptr<ShaderData> m_data;

		friend D3D11RenderComponent;

	};

	class XPLICIT_API D3D11RenderComponent final : public Component
	{
	public:
		class XPLICIT_API Vertex
		{
		public:
			struct
			{
				float X, Y, Z;
			} pos;

			struct
			{
				float R, G, B, A;
			} clr;

		};

	public:
		D3D11RenderComponent();
		~D3D11RenderComponent();

		D3D11RenderComponent& operator=(const D3D11RenderComponent&) = default;
		D3D11RenderComponent(const D3D11RenderComponent&) = default;
		
		void push_back(const Nplicit::Vector<float>& vert, const Nplicit::Color<float>& clr);
		void create(std::unique_ptr<DriverSystemD3D11>& driver);

		void set(DriverSystemD3D11* dx11);
		void set(D3D11ShaderSystem* shader);

		virtual void update() override;

	public:
		virtual bool should_update() noexcept override { return true; }
		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_RENDER; }
		virtual const char* name() noexcept override { return ("D3D11RenderComponent"); }

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertex_buffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_index_buffer;

	private:
		std::map<Nplicit::Vector<float>, Nplicit::Color<float>> m_coord;

		D3D11_SUBRESOURCE_DATA m_vertex_data;
		D3D11_SUBRESOURCE_DATA m_index_data;
		D3D11_BUFFER_DESC m_vertex_buf_desc;
		D3D11_BUFFER_DESC m_index_buf_desc;
		D3D11ShaderSystem* m_shader;
		DriverSystemD3D11* m_driver;
		Vertex* m_vertex_arr;
		int64_t* m_index_arr;
		HRESULT m_hr;

		friend D3D11ShaderSystem;

	};
}

#endif
