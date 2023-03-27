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
#include <d3d11.h>
#include <d3dcommon.h>

#include <DirectXMath.h>

#ifdef XPLICIT_DEBUG

#include <nuklear/nuklear.h>
#include <nuklear/nuklear_d3d11.h>
#include <nuklear/nuklear_d3d11_pixel_shader.h>
#include <nuklear/nuklear_d3d11_vertex_shader.h>

#include <nuklear/nuklear.h>

#endif // XPLICIT_DEBUG

#define XPLICIT_VERTEX_SHADER "vs_5_0"
#define XPLICIT_PIXEL_SHADER "ps_5_0"
#define XPLICIT_HULL_SHADER "hs_5_0"
#define XPLICIT_COMPUTE_SHADER "ds_5_0"
#define XPLICIT_DOMAIN_SHADER "ds_5_0"
#define XPLICIT_GEOMETRY_SHADER "gs_5_0"

using namespace DirectX;

namespace Xplicit::Renderer::DX11
{
	namespace Details
	{
		__declspec(align(16)) struct VERTEX
		{
			XMVECTOR position;
			XMVECTOR color;
		};

		__declspec(align(16)) struct CBUFFER
		{
			XMMATRIX view;
			XMMATRIX world;
			XMMATRIX projection;
		};

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

		class XPLICIT_API DriverTraits
		{
		public:
			bool bVSync{ false };
			char szCardDesc[128];
			bool bEndRendering{ false };
			HWND pWindowHandle{ nullptr };

		public:
			D3D11_VIEWPORT Viewport;
			DXGI_SWAP_CHAIN_DESC SwapDesc;

		public:
			Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
			Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> pCtx;
			Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
			Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthTexture;
			Microsoft::WRL::ComPtr<ID3D11Texture2D> pRenderTexture;
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterState;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencil;
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;

		};

	private:
		void setup();

	public:
		void begin_scene(const float a, const float r, const float g, const float b);
		bool end_scene();

	public:
		const bool& is_closed() noexcept;
		DriverTraits& get() noexcept;
		void close() noexcept;
		operator bool();

	private:
		void handle_device_removed();
		bool check_device_removed(HRESULT hr);

	private:
		DriverTraits m_private;

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
		class XPLICIT_API ShaderTraits
		{
		public:
			std::string entrypoint{};
			std::string shader_type{};

			uint32_t iFlags1{ 0 };
			uint32_t iFlags2{ 0 };
			ID3D10Blob* pBlob{ nullptr };
			ID3D10Blob* pErrorBlob{ nullptr };
			D3D11_BUFFER_DESC matrixBufferDesc{};

			HRESULT create_input_layout(ID3D11Device* device);

			template <typename StructSz>
			HRESULT create_matrix_buffer(ID3D11Device* device);

		public:
			Microsoft::WRL::ComPtr<ID3D11HullShader> pHull;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixel;
			Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertex;
			Microsoft::WRL::ComPtr<ID3D11Buffer> pMatrixBuffer;
			Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

		public:
			std::vector<D3D11_INPUT_ELEMENT_DESC> input_layouts;

		};

		ShaderTraits& get();

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
		ShaderTraits m_data;

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
		void push(D3D11ShaderSystem* system) noexcept;

		void set(DriverSystemD3D11* dx11) noexcept;

		size_t size() noexcept;
		void create();

	public:
		virtual void update() override;

	public:
		virtual bool should_update() noexcept override;
		virtual INSTANCE_TYPE type() noexcept override;
		virtual const char* name() noexcept override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
		std::vector<Nplicit::Vector<float>> m_arrayVerts;

		D3D11_SUBRESOURCE_DATA m_vertexData;
		D3D11_SUBRESOURCE_DATA m_indexData;
		D3D11_BUFFER_DESC m_vertexBufferDesc;
		D3D11_BUFFER_DESC m_indexBufDesc;

	private:
		std::vector<D3D11ShaderSystem*> m_pShader;
		DriverSystemD3D11* m_pDriver;
		Details::VERTEX* m_pVertex;
		size_t m_iVertexCnt;
		HRESULT m_hResult;

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
#include "D3D11ShaderSystem.inl"

#endif
