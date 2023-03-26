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

#include "Xplicit.h"
#include "Avx.h"

#ifdef XPLICIT_WINDOWS

#include <wrl.h> /* Microsoft::WRL::ComPtr */
#include <dxgi.h>

#include <../um/d3d11.h>
#include <../um/d3dcommon.h>

#include <D3DX10math.h>

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

namespace Xplicit::Details
{
	__declspec(align(16))
		struct VERTEX
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

	struct MATRIX
	{
		D3DXMATRIX world_matrix;
		D3DXMATRIX view_matrix;
		D3DXMATRIX projection_matrix;
	};
}

namespace Xplicit::Renderer::DX11
{
	namespace Details
	{
		void ThrowIfFailed(HRESULT hr);
	}

	namespace WRL = Microsoft::WRL;

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

		class XPLICIT_API PrivateData
		{
		public:
			bool VSync{ false };
			char CardDesc[128];
			bool EndRendering{ false };
			HWND WindowHandle{ nullptr };

		public:
			D3D11_VIEWPORT Viewport;
			DXGI_SWAP_CHAIN_DESC SwapDesc;

		public:
			Microsoft::WRL::ComPtr<ID3D11Device> Device;
			Microsoft::WRL::ComPtr<IDXGIAdapter> Adapter;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> Ctx;
			Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
			Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthTexture;
			Microsoft::WRL::ComPtr<ID3D11Texture2D> RenderTexture;
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterState;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencil;
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState;

		};

	private:
		void setup();

	public:
		void begin_scene(const float a, const float r, const float g, const float b);
		bool end_scene();

	public:
		const bool& is_closed() noexcept;
		PrivateData& get() noexcept;
		void close() noexcept;
		operator bool();

	private:
		void handle_device_removed();
		bool check_device_removed(HRESULT hr);

	private:
		PrivateData m_private;

	};

	XPLICIT_API std::unique_ptr<DriverSystemD3D11> make_driver_system_d3d11(HWND hwnd);

	class XPLICIT_API D3D11ShaderSystem final : public ShaderSystem
	{
	public:
		D3D11ShaderSystem() = delete;

	public:
		explicit D3D11ShaderSystem(const PChar* filename)
			: ShaderSystem(filename, FORMAT_HLSL), m_data()
		{}

		virtual ~D3D11ShaderSystem();

		D3D11ShaderSystem& operator=(const D3D11ShaderSystem&) = default;
		D3D11ShaderSystem(const D3D11ShaderSystem&) = default;

	public:
		/// <summary>
		/// Everything related to shaders are located here
		/// Internal uses only.
		/// </summary>
		class XPLICIT_API ShaderData
		{
		public:
			std::string entrypoint{};
			std::string shader_type{};

			ID3D10Blob* blob{ nullptr };
			ID3D10Blob* error_blob{ nullptr };
			uint32_t flags1{ 0 };
			uint32_t flags2{ 0 };
			D3D11_BUFFER_DESC matrix_buffer_desc{};

			inline HRESULT set_matrixes(ID3D11DeviceContext* ctx,
				D3DXMATRIX worldMatrix,
				D3DXMATRIX viewMatrix,
				D3DXMATRIX projectionMatrix)
			{
				HRESULT hr = S_OK;

				if (!ctx)
					return hr;

				D3D11_MAPPED_SUBRESOURCE mapped_res;
				Xplicit::Details::MATRIX* matrix = nullptr;

				D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
				D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
				D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

				hr = ctx->Map(matrix_buffer_ptr.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
				
				if (FAILED(hr))
					return hr;

				matrix = reinterpret_cast<Xplicit::Details::MATRIX*>(mapped_res.pData);

				matrix->projection_matrix = projectionMatrix;
				matrix->view_matrix = viewMatrix;
				matrix->world_matrix = worldMatrix;

				ctx->Unmap(matrix_buffer_ptr.Get(), 0);
				ctx->VSSetConstantBuffers(0, 1, matrix_buffer_ptr.GetAddressOf());

				XPLICIT_ASSERT(SUCCEEDED(hr));

				return hr;
			}

			inline HRESULT create_input_layout(ID3D11Device* device)
			{
				HRESULT hr = S_OK;

				if (!device)
					return hr;

				hr = device->CreateInputLayout(input_layouts.data(), input_layouts.size(), blob->GetBufferPointer(),
					blob->GetBufferSize(), input_layout_ptr.GetAddressOf());

				XPLICIT_ASSERT(SUCCEEDED(hr));
				return hr;
			}

		public:
			Microsoft::WRL::ComPtr<ID3D11HullShader> hull;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel;
			Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex;
			Microsoft::WRL::ComPtr<ID3D11Buffer> matrix_buffer_ptr;
			Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout_ptr;

		public:
			std::vector<D3D11_INPUT_ELEMENT_DESC> input_layouts;

		};

		ShaderData& get();

	public:
		/// <summary>
		/// Compiles the HLSL shader
		/// </summary>
		virtual int compile() noexcept override;

		/// <summary>
		/// Updates the shader.
		/// </summary>
		void update(D3D11RenderComponent* component);

	private:
		ShaderData m_data;

	private:
		friend D3D11RenderComponent;

	};

	class XPLICIT_API D3D11RenderComponent final : public Component
	{
	public:
		D3D11RenderComponent();
		~D3D11RenderComponent();

		D3D11RenderComponent& operator=(const D3D11RenderComponent&) = default;
		D3D11RenderComponent(const D3D11RenderComponent&) = default;
		
		void push(const Nplicit::Vector<float>& vert);

		void set(DriverSystemD3D11* dx11) noexcept;
		void set(D3D11ShaderSystem* system) noexcept;

		size_t size() noexcept;
		void create();

	public:
		virtual void update() override;

	public:
		virtual bool should_update() noexcept override;
		virtual INSTANCE_TYPE type() noexcept override;
		virtual const char* name() noexcept override;

	private:
		std::vector<Nplicit::Vector<float>> m_verts;
		D3D11ShaderSystem* m_shader;
	
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertex_buffer;

	private:
		Xplicit::Details::VERTEX* m_vertex_arr;

	private:
		D3D11_SUBRESOURCE_DATA m_vertex_data;
		D3D11_SUBRESOURCE_DATA m_index_data;
		D3D11_BUFFER_DESC m_vertex_buf_desc;
		D3D11_BUFFER_DESC m_index_buf_desc;
		DriverSystemD3D11* m_driver;

	private:
		size_t m_vertex_cnt;
		HRESULT m_hr;

		friend D3D11ShaderSystem;

	};

	enum class XPLICIT_SHADER_TYPE
	{
		Vertex,
		Pixel,
		Hull,
	};

	class XPLICIT_API D3D11ShaderHelper1 final
	{
	public:
		template <XPLICIT_SHADER_TYPE ShaderType>
		static D3D11ShaderSystem* make_shader(
			const PChar* filename,
			const char* entrypoint,
			std::unique_ptr<DriverSystemD3D11>& driver
		);

	};
}

#include "DriverD3D11.inl"

#endif
