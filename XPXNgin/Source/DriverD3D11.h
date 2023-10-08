/*
 * =====================================================================
 *
 *			XPXNgin (RigMan)
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: DriverD3D11.h
 *			Purpose: C++ Rendering Driver for Direct3D 11
 *
 * =====================================================================
 */

#pragma once

#include "DriverSystem.h"
#include "Component.h"
#include "NginCore.h"
#include "Avx.h"

#include "Bites.h"
#include "FileIO.h"

#ifdef XPLICIT_WINDOWS

#	include <wrl.h> /* Microsoft::WRL::ComPtr */
#	include <dxgi.h>
#	include <d3d11.h>
#	include <d3dcommon.h>
#	include <DirectXMath.h>

#ifdef XPLICIT_DEBUG

#	include <nuklear/nuklear.h>
#	include <nuklear/nuklear_d3d11.h>
#	include <nuklear/nuklear_d3d11_pixel_shader.h>
#	include <nuklear/nuklear_d3d11_vertex_shader.h>

#	include <nuklear/nuklear.h>

#endif // XPLICIT_DEBUG

#define XPLICIT_VERTEX_SHADER "vs_5_0"
#define XPLICIT_PIXEL_SHADER "ps_5_0"
#define XPLICIT_HULL_SHADER "hs_5_0"
#define XPLICIT_COMPUTE_SHADER "ds_5_0"
#define XPLICIT_DOMAIN_SHADER "ds_5_0"
#define XPLICIT_GEOMETRY_SHADER "gs_5_0"

using namespace DirectX;

#define XPLICIT_DX_ALIGN (16)

namespace XPX::Renderer::DX11
{
	class CameraSystemD3D11;
	class LightSystemD3D11;

	namespace Details
	{
		struct __declspec(align(XPLICIT_DX_ALIGN)) VERTEX
		{
			XMFLOAT4 POSITION;
			XMFLOAT2 TEXCOORD;
			XMFLOAT3 NORMAL;
			XMFLOAT3 AMBIENT;
		};

		struct __declspec(align(XPLICIT_DX_ALIGN)) CAMERA_POS
		{
			XMFLOAT3 POSITION;
			float32 PADDING;
		};

		struct __declspec(align(XPLICIT_DX_ALIGN)) LIGHT
		{
			XMFLOAT4 AMBIENT_COLOR;
			XMFLOAT4 DIFFUSE_COLOR;

			XMFLOAT3 DIRECTION;

			float32 SPECULAR_POWER;
			XMFLOAT3 SPECULAR_COLOR;
		};

		struct __declspec(align(XPLICIT_DX_ALIGN)) CBUFFER
		{
			XMMATRIX VIEW;
			XMMATRIX WORLD;
			XMMATRIX PROJECTION;
		};

		XPLICIT_API void ThrowIfFailed(HRESULT hr);
	}

	namespace WRL = Microsoft::WRL;

	class DriverSystemD3D11;
	class ShaderSystemD3D11;
	class RenderableComponentD3D11;

	class XPLICIT_API DriverSystemD3D11 : public DriverSystem
	{
	public:
		DriverSystemD3D11() = delete;
		
		DriverSystemD3D11(HWND hwnd, 
			const UINT width,
			const UINT height);

		~DriverSystemD3D11() override;

	public:
		const char* name() noexcept override;
		RENDER_SYSTEM api() noexcept override;

		class XPLICIT_API DriverTraits
		{
		public:
			bool bVSync{ false };
			char szCardDesc[128];
			UINT ViewportCnt{ 1 };
			HRESULT hResult{ S_OK };
			bool bEndRendering{ false };
			HWND pWindowHandle{ nullptr };

			UINT iWidth{ XPLICIT_DEFAULT_WIDTH };
			UINT iHeight{ XPLICIT_DEFAULT_HEIGHT };

		public:
			DXGI_SWAP_CHAIN_DESC SwapDesc;
			D3D11_VIEWPORT Viewport;
			XMMATRIX ProjectionMatrix;
			XMMATRIX WorldMatrix;
			XMMATRIX OrthoMatrix;

		public:
			WRL::ComPtr<ID3D11Device> pDevice;
			WRL::ComPtr<IDXGIAdapter> pAdapter;
			WRL::ComPtr<ID3D11DeviceContext> pContext;
			WRL::ComPtr<IDXGISwapChain> pSwapChain;
			WRL::ComPtr<ID3D11Texture2D> pDepthTexture;
			WRL::ComPtr<ID3D11Texture2D> pRenderTexture;
			WRL::ComPtr<ID3D11RasterizerState> pRasterState;
			WRL::ComPtr<ID3D11DepthStencilView> pDepthStencil;
			WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget;
			WRL::ComPtr<ID3D11InputLayout> pInputLayout;
			WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;

		public:
			std::unique_ptr<CameraSystemD3D11> pCamera;

		};

	private:
		void setup_rendering_system();

	public:
		void begin_scene(const float& A, 
			const float& R, 
			const float& G, 
			const float& B, 
			const bool zBuffer, 
			const bool depth);

		bool end_scene();

	public:
		const bool& is_closed() noexcept;
		DriverTraits& get() noexcept;
		void close() noexcept;
		operator bool();

	private:
		bool check_device_removed(HRESULT hr);

	private:
		DriverTraits m_private;

	};

	enum class XPLICIT_SHADER_TYPE
	{
		Vertex,
		Pixel,
		Hull,
	};

	XPLICIT_API std::unique_ptr<DriverSystemD3D11> make_driver_system_d3d11(HWND hwnd, UINT width, UINT height);

	class XPLICIT_API ShaderSystemD3D11 final : public ShaderSystem
	{
	public:
		ShaderSystemD3D11() = delete;
		
		explicit ShaderSystemD3D11(const PChar* filename, XPLICIT_SHADER_TYPE fmt, DriverSystemD3D11* pDriver)
			: ShaderSystem(filename, (uint8_t)fmt, FORMAT_HLSL), m_data(), m_pDriver(pDriver)
		{}

		~ShaderSystemD3D11() override;

	public:
		ShaderSystemD3D11& operator=(const ShaderSystemD3D11&) = default;
		ShaderSystemD3D11(const ShaderSystemD3D11&) = default;
		
		class XPLICIT_API ShaderTraits
		{
		public:
			std::string entrypoint{ "VS" };
			std::string shader_type{ XPLICIT_VERTEX_SHADER };

			uint32_t iFlags1{ 0U };
			uint32_t iFlags2{ 0U };

		public:
			ID3D10Blob* pBlob{ nullptr };
			ID3D10Blob* pErrorBlob{ nullptr };
			D3D11_BUFFER_DESC matrixBufferDesc{};

		public:
			WRL::ComPtr<ID3D11HullShader> pHull;
			WRL::ComPtr<ID3D11PixelShader> pPixel;
			WRL::ComPtr<ID3D11VertexShader> pVertex;
			WRL::ComPtr<ID3D11Buffer> pMatrixBuffer;

		public:
			ID3D11InputLayout* pInputLayout;

		};

	public:
		ShaderTraits& get();

	public:
		int compile() noexcept override;
		void update() noexcept;

	public:
		void update_light_shader(LightSystemD3D11* component);
		void update_render_shader(RenderableComponentD3D11* component);

	private:
		ShaderTraits m_data;
		DriverSystemD3D11* m_pDriver;
		
		friend RenderableComponentD3D11;

	};

	typedef enum D3D_PRIMITIVE_TOPOLOGY XPLICIT_PRIMITIVE_TOPOLOGY;

	class XPLICIT_API LightSystemD3D11 final : public LightSystem
	{
	public:
		explicit LightSystemD3D11(const std::size_t& verts);
		~LightSystemD3D11() override;

	public:
		XPLICIT_COPY_DEFAULT(LightSystemD3D11);

	public:
		void update(const std::size_t indexCount) noexcept;

	private:
		WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
		WRL::ComPtr<ID3D11SamplerState> m_pSamplerState;
		WRL::ComPtr<ID3D11Buffer> m_pCameraBuffer;
		WRL::ComPtr<ID3D11Buffer> m_pMatrixBuffer;
		WRL::ComPtr<ID3D11Buffer> m_pLightBuffer;
		ShaderSystemD3D11* m_pLightVs;
		ShaderSystemD3D11* m_pLightPs;
		HRESULT m_hResult;

	private:
		friend ShaderSystemD3D11;

	};

	class XPLICIT_API RenderableComponentD3D11 final : public BaseRenderableComponent
	{
	public:
		explicit RenderableComponentD3D11() noexcept;
		~RenderableComponentD3D11() override;

	public:
		XPLICIT_COPY_DEFAULT(RenderableComponentD3D11);

	public:
		void push_specular(const Color<float>& clr) noexcept;
		void push_ambient(const Color<float>& clr) noexcept;
		void push_diffuse(const Color<float>& clr) noexcept;

	public:
		void push_normal(const Vector<float>& indice) noexcept;
		void push_vertice(const Vector<float>& vert) noexcept;
		void push_indice(const UINT& indice) noexcept;

	public:
		void set_driver(DriverSystemD3D11* the) noexcept;
		DriverSystemD3D11* driver() noexcept;

	public:
		const size_t& get_vertices_count() noexcept;
		const size_t& get_indices_count() noexcept;

	public:
		static bool should_update() noexcept;
		static void update(ClassPtr self);

	public:
		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

	public:
		void should_draw(const bool) noexcept;
		const bool& should_draw() noexcept;

		void set_position(const Vector<float>& pos) noexcept;
		const Vector<float>& position() noexcept;

		void set_rotation(const Vector<float>& rot) noexcept;
		const Vector<float>& rotation() noexcept;

		void set_scale(const Vector<float>& size) noexcept;
		const Vector<float>& scale() noexcept;

		void make_renderable(const std::vector<ImageDataParams>& params);

	public:
		auto get_topology() noexcept { return m_iTopology; }
		void set_topology(const XPLICIT_PRIMITIVE_TOPOLOGY& topology) { m_iTopology = topology; }

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pMatrixBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
		
	public:
		void push_texture_coord(const Vector<float>& coord) noexcept { m_arrayTextures.push_back(coord); }

	public:
		/// <summary>
		/// Texture system for DirectX renderable.
		/// </summary>
		struct XPLICIT_API TextureSystemD3D11 final
		{
		private:
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;
			Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture;
			DriverSystemD3D11* m_pDriver{ nullptr };

			void make_texture(ImageDataParams params) noexcept
			{
				if (!m_pDriver)
					return;

				D3D11_TEXTURE2D_DESC textureDesc{};

				textureDesc.Height = params.iHeight;
				textureDesc.Width = params.iWidth;
				textureDesc.MipLevels = 0;
				textureDesc.ArraySize = 1;
				textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				textureDesc.SampleDesc.Count = 1;
				textureDesc.SampleDesc.Quality = 0;
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
				textureDesc.CPUAccessFlags = 0;
				textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

				Details::ThrowIfFailed(m_pDriver->get().pDevice->CreateTexture2D(&textureDesc, nullptr, m_pTexture.GetAddressOf()));

				float rowPitch = (params.iStride * params.iWidth) * sizeof(BYTE);

				m_pDriver->get().pContext->UpdateSubresource(m_pTexture.Get(), 0, nullptr, params.pImage, rowPitch, 0);

				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
				srvDesc.Format = textureDesc.Format;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = -1;

				// Create the shader resource view for the texture.
				Details::ThrowIfFailed(m_pDriver->get().pDevice->CreateShaderResourceView(m_pTexture.Get(), &srvDesc, m_pTextureView.GetAddressOf()));

				m_pDriver->get().pContext->GenerateMips(m_pTextureView.Get());
			}

			friend RenderableComponentD3D11;

		};

	private:
		std::vector<TextureSystemD3D11*> m_vTextures;

	private:
		WRL::ComPtr<ID3D11SamplerState> m_pSamplerState;
		std::vector<Vector<float32>> m_arrayTextures;

	private:
		UINT m_iSamplerCnt;

	private:
		D3D11_SUBRESOURCE_DATA m_vertexData;
		D3D11_SUBRESOURCE_DATA m_indexData;
		D3D11_BUFFER_DESC m_vertexBufferDesc;
		D3D11_BUFFER_DESC m_indexBufDesc;

	private:
		XPLICIT_PRIMITIVE_TOPOLOGY m_iTopology;

	private:
		ShaderSystemD3D11* m_pVertexShader;
		ShaderSystemD3D11* m_pTextureShader;
		DriverSystemD3D11* m_pDriver;

	private:
		Details::VERTEX* m_pVertex;

	private:
		Vector<float32> m_vRotation;
		Vector<float32> m_vPosition;
		Vector<float32> m_vScale;

	private:
		bool m_bDraw;

	private:
		size_t m_iIndices;
		size_t m_iVertexCnt;
		HRESULT m_hResult;

		friend ShaderSystemD3D11;

	public:
		LightSystemD3D11* f_pSourceLight;

	};

	class XPLICIT_API D3D11ShaderHelper1 final
	{
	public:
		template <XPLICIT_SHADER_TYPE ShaderType>
		static ShaderSystemD3D11* make_shader(
			const PChar* filename,
			const char* entrypoint,
			DriverSystemD3D11* driver
		);

	};

	class XPLICIT_API CameraSystemD3D11 : public DriverCameraSystem
	{
	public:
		explicit CameraSystemD3D11() noexcept : DriverCameraSystem(), m_viewMatrix(), m_rotationMatrix() {}
		~CameraSystemD3D11() override {}

	private:
		void render_camera() noexcept;

	public:
		XPLICIT_COPY_DEFAULT(CameraSystemD3D11);

	private:
		XMMATRIX m_rotationMatrix;
		XMMATRIX m_viewMatrix;

		friend ShaderSystemD3D11;
		friend DriverSystemD3D11;

	};

	class XPLICIT_API PostProcessEffectD3D11 final
	{
		PostProcessEffectD3D11() = delete;

	public:
		enum
		{
			BLUR,
			BLOOM,
			SHAKE,
			FOCUS,
		};

		PostProcessEffectD3D11(int effect_type);
		~PostProcessEffectD3D11();

		XPLICIT_COPY_DEFAULT(PostProcessEffectD3D11);

	public:
		void update() noexcept;

	private:
		ShaderSystemD3D11* m_pVs{ nullptr };
		ShaderSystemD3D11* m_pPs{ nullptr };

	private:
		WRL::ComPtr<ID3D11InputLayout> m_pInputLayout{ nullptr };

	private:
		WRL::ComPtr<ID3D11RasterizerState> m_pPPState{ nullptr };
		D3D11_RASTERIZER_DESC m_rasterDesc;

	};
}

#include "DriverD3D11.inl"
#include "D3D11ShaderSystem.inl"

namespace XPX::Renderer
{
	typedef DX11::CameraSystemD3D11 CameraSystem;
	typedef DX11::RenderableComponentD3D11 RenderableComponent;
}

#endif
