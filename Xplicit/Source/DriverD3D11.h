/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright Xplicit Inc, all rights reserved.
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

#include <Nplicit.h>

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
	class ShaderSystemD3D11;
	class RenderComponentD3D11;

	class XPLICIT_API DriverSystemD3D11 : public DriverSystem
	{
	public:
		DriverSystemD3D11() = delete;
		
		explicit DriverSystemD3D11(HWND hwnd);
		~DriverSystemD3D11() override;

		DriverSystemD3D11& operator=(const DriverSystemD3D11&) = default;
		DriverSystemD3D11(const DriverSystemD3D11&) = default;

		const char* name() noexcept override;
		RENDER_SYSTEM api() noexcept override;

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
			WRL::ComPtr<ID3D11Device> pDevice;
			WRL::ComPtr<IDXGIAdapter> pAdapter;
			WRL::ComPtr<ID3D11DeviceContext> pCtx;
			WRL::ComPtr<IDXGISwapChain> pSwapChain;
			WRL::ComPtr<ID3D11Texture2D> pDepthTexture;
			WRL::ComPtr<ID3D11Texture2D> pRenderTexture;
			WRL::ComPtr<ID3D11RasterizerState> pRasterState;
			WRL::ComPtr<ID3D11DepthStencilView> pDepthStencil;
			WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget;
			WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;

		};

	private:
		void setup();

	public:
		void begin_scene(const float& a, const float& r, const float& g, const float& b, const bool zBuffer, const bool depth);
		bool end_scene();

	public:
		const bool& is_closed() noexcept;
		DriverTraits& get() noexcept;
		void close() noexcept;
		operator bool();

	private:
		static void handle_device_removed();
		static bool check_device_removed(HRESULT hr);

	private:
		DriverTraits m_private;

	};

	XPLICIT_API std::unique_ptr<DriverSystemD3D11> make_driver_system_d3d11(HWND hwnd);

	class XPLICIT_API ShaderSystemD3D11 final : public ShaderSystem
	{
	public:
		ShaderSystemD3D11() = delete;
		
		explicit ShaderSystemD3D11(const PChar* filename)
			: ShaderSystem(filename, FORMAT_HLSL), m_data()
		{}

		~ShaderSystemD3D11() override;

		ShaderSystemD3D11& operator=(const ShaderSystemD3D11&) = default;
		ShaderSystemD3D11(const ShaderSystemD3D11&) = default;
		
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
			
			WRL::ComPtr<ID3D11HullShader> pHull;
			WRL::ComPtr<ID3D11PixelShader> pPixel;
			WRL::ComPtr<ID3D11VertexShader> pVertex;
			WRL::ComPtr<ID3D11Buffer> pMatrixBuffer;
			WRL::ComPtr<ID3D11InputLayout> pInputLayout;
			
			std::vector<D3D11_INPUT_ELEMENT_DESC> input_layouts;

		};

		ShaderTraits& get();
		
		/// <summary>
		/// Compiles the HLSL shader
		/// </summary>
		int compile() noexcept override;

		/// <summary>
		/// Updates the shader.
		/// </summary>
		void update(RenderComponentD3D11* component);

	private:
		ShaderTraits m_data;
		
		friend RenderComponentD3D11;

	};

	/*
     *
	 * RenderComponentD3D11
	 * 
	 * @brief Rendering Component, takes care of rendering a specific component in the scene.
	 * NOTE: don't add this to the main ComponentSystem, use the SceneTree class instead.
	 * 
	 */

	class XPLICIT_API RenderComponentD3D11 final : public Component
	{
	public:
		RenderComponentD3D11();
		~RenderComponentD3D11() override;

	public:
		XPLICIT_COPY_DEFAULT(RenderComponentD3D11);

	public:
		void push(const Vector<float>& vert);
		void push_shader(ShaderSystemD3D11* system) noexcept;

	public:
		void set_driver(DriverSystemD3D11* dx11) noexcept;

	public:
		size_t size() noexcept;
		void create();

	public:
		static void update(void* _this);

	public:
		static bool should_update() noexcept { return true; }
		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
		std::vector<Vector<float>> m_arrayVerts;

		D3D11_SUBRESOURCE_DATA m_vertexData;
		D3D11_SUBRESOURCE_DATA m_indexData;
		D3D11_BUFFER_DESC m_vertexBufferDesc;
		D3D11_BUFFER_DESC m_indexBufDesc;

	private:
		std::vector<ShaderSystemD3D11*> m_pShader;
		DriverSystemD3D11* m_pDriver;
		Details::VERTEX* m_pVertex;
		size_t m_iVertexCnt;
		HRESULT m_hResult;

		friend ShaderSystemD3D11;

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
		static ShaderSystemD3D11* make_shader(
			const PChar* filename,
			const char* entrypoint,
			std::unique_ptr<DriverSystemD3D11>& driver
		);

	};
}

#include "DriverD3D11.inl"
#include "D3D11ShaderSystem.inl"

#endif
