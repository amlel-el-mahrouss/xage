/*
 * =====================================================================
 *
 *			XPXNgin (RigMan)
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: DriverD3D11.cpp
 *			Purpose: C++ Rendering Driver for Direct3D 11
 *
 * =====================================================================
 */

 /**
 @file DriverD3D11.cpp
 */

#include "DriverD3D11.h"
#include "Bites.h"

#include "HelperMacros.h"

#ifdef XPLICIT_WINDOWS

#include <d3dcompiler.h>
#include <comdef.h>

namespace XPX::Renderer::DX11
{
	namespace Details
	{
		void ThrowIfFailed(const HRESULT hr)
		{
			if (FAILED(hr))
				throw Win32Error("[ThrowIfFailed] Fatal DirectX error! Check HRESULT.");
		}
	}

	struct DxgiOutputResult
	{
		uint32_t NumOutputs;
		std::vector<IDXGIOutput*> Outputs;
	};

	static DxgiOutputResult xplicit_enum_outputs(IDXGIAdapter* adapter) noexcept
	{
		DxgiOutputResult result;
		IDXGIOutput* pOutput = nullptr;

		while (adapter->EnumOutputs(result.NumOutputs, &pOutput) != DXGI_ERROR_NOT_FOUND)
		{
			result.Outputs.push_back(pOutput);
			++result.NumOutputs;
		}

		return result;
	}

	static void xplicit_d3d11_make_swapchain(
		DXGI_SWAP_CHAIN_DESC& swapDesc, 
		DriverSystemD3D11::DriverTraits& privateData,
		DWORD numerator,
		DWORD denominator,
		DWORD width,
		DWORD height,
		DXGI_FORMAT format,
		DXGI_MODE_SCALING scaling,
		DXGI_MODE_SCANLINE_ORDER scanlineOrdering)
	{
		RtlZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapDesc.BufferCount = 1;
		swapDesc.BufferDesc.Format = format;
		swapDesc.BufferDesc.Width = width;
		swapDesc.BufferDesc.Height = height;

		swapDesc.BufferDesc.ScanlineOrdering = scanlineOrdering;
		swapDesc.BufferDesc.Scaling = scaling;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		swapDesc.BufferDesc.RefreshRate.Denominator = denominator;
		swapDesc.BufferDesc.RefreshRate.Numerator = numerator;

		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;

		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		
		swapDesc.Flags = 0;

		swapDesc.Windowed = true;
		swapDesc.OutputWindow = privateData.pWindowHandle;
	}

	DriverSystemD3D11::DriverSystemD3D11(
		HWND hwnd,
		const UINT width,
		const UINT height)
		: m_private()
	{
		IDXGIFactory* pFactory = nullptr;
		auto hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);

		Details::ThrowIfFailed(hr);

		IDXGIAdapter* pAdapter = nullptr;

		hr = pFactory->EnumAdapters(0, &pAdapter);

		Details::ThrowIfFailed(hr);

		IDXGIOutput* pOutput = nullptr;

		hr = pAdapter->EnumOutputs(0, &pOutput);

		Details::ThrowIfFailed(hr);

		UINT numModes = 0U;
		hr = pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);

		Details::ThrowIfFailed(hr);

		DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
		memset(displayModeList, 0, numModes);

		hr = pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);

		Details::ThrowIfFailed(hr);

		if (!displayModeList)
		{
			pOutput->Release();
			pAdapter->Release();
			pFactory->Release();

			throw EngineError("DriverD3D11: out of memory!");
		}

		UINT i = 0U;

		UINT numerator = 0U;
		UINT denominator = 0U;

		for (i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width >= width)
			{
				if (displayModeList[i].Height >= height)
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;

					m_private.pWindowHandle = hwnd;

					xplicit_d3d11_make_swapchain(m_private.SwapDesc,
						m_private,
						numerator,
						denominator,
						width,
						height,
						displayModeList[i].Format,
						displayModeList[i].Scaling,
						displayModeList[i].ScanlineOrdering);

					get().pCamera = std::make_unique<CameraSystemD3D11>();

					m_private.iWidth = width;
					m_private.iHeight = height;

					this->setup_rendering_system();

					delete[] displayModeList;

					pFactory->Release();
					pOutput->Release();
					pAdapter->Release();

					return;
				}
			}
		}

		Details::ThrowIfFailed(E_FAIL);
	}

	DriverSystemD3D11::~DriverSystemD3D11() 
	{
		if (m_private.pSwapChain)
			m_private.pSwapChain->SetFullscreenState(false, nullptr);
	}

	constexpr const float SCREEN_DEPTH = 1000.0f;
	constexpr const float SCREEN_NEAR = 0.3f;

	void DriverSystemD3D11::setup_rendering_system()
	{
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(XPLICIT_DEBUG)
		// If the project is in a debug build, enable the debug layer.
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&m_private.SwapDesc,
			m_private.pSwapChain.GetAddressOf(),
			m_private.pDevice.GetAddressOf(),
			nullptr,
			m_private.pContext.GetAddressOf()
		);

		if (FAILED(hr))
			throw Win32Error("[D3D11CreateDeviceAndSwapChain] hr != S_OK, check debug output.");

		hr = m_private.pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_private.pRenderTexture.GetAddressOf()));

		if (FAILED(hr))
			throw Win32Error("[GetBuffer] Buffer had incorrect arguments.");

		hr = m_private.pDevice->CreateRenderTargetView(m_private.pRenderTexture.Get(), nullptr, m_private.pRenderTarget.GetAddressOf());

		if (FAILED(hr))
			throw Win32Error("[CreateRenderTargetView] One or more arguments are invalid.");

		m_private.pRenderTexture->Release();

		D3D11_TEXTURE2D_DESC depthBufferDesc;
		RtlZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

		depthBufferDesc.Width = get().iWidth;
		depthBufferDesc.Height = get().iHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1U;
		depthBufferDesc.SampleDesc.Quality = 0U;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		hr = m_private.pDevice->CreateTexture2D(&depthBufferDesc, nullptr, m_private.pDepthTexture.GetAddressOf());

		if (FAILED(hr))
			throw Win32Error("[CreateTexture2D] Can't create Texture2D!");

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		hr = m_private.pDevice->CreateDepthStencilState(&depthStencilDesc, m_private.pDepthStencilState.GetAddressOf());

		if (FAILED(hr))
			throw Win32Error("[CreateDepthStencilState] Can't create a depth stencil!");

		m_private.pContext->OMSetDepthStencilState(m_private.pDepthStencilState.Get(), 1);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

		ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		hr = m_private.pDevice->CreateDepthStencilView(m_private.pDepthTexture.Get(), &depthStencilViewDesc, m_private.pDepthStencil.GetAddressOf());

		Details::ThrowIfFailed(hr);

		m_private.pContext->OMSetRenderTargets(get().ViewportCnt, m_private.pRenderTarget.GetAddressOf(), m_private.pDepthStencil.Get());

		D3D11_RASTERIZER_DESC rasterDesc;
		RtlZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.FrontCounterClockwise = true;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.SlopeScaledDepthBias = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.ScissorEnable = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.AntialiasedLineEnable = false;
	
		hr = m_private.pDevice->CreateRasterizerState(&rasterDesc, m_private.pRasterState.GetAddressOf());

		Details::ThrowIfFailed(hr);

		m_private.pContext->RSSetState(m_private.pRasterState.Get());

		RtlZeroMemory(&m_private.Viewport, sizeof(D3D11_VIEWPORT));

		m_private.Viewport.Width = static_cast<float>(get().iWidth);
		m_private.Viewport.Height = static_cast<float>(get().iHeight);

		m_private.Viewport.MinDepth = 0.0f;
		m_private.Viewport.MaxDepth = 1.0f;

		m_private.Viewport.TopLeftX = 0.0f;
		m_private.Viewport.TopLeftY = 0.0f;

		float fieldOfView = NPLICIT_PI / 4.0f;
		float screenAspect = (float)m_private.Viewport.Width / (float)m_private.Viewport.Height;

		// Create the projection matrix for 3D rendering.
		m_private.ProjectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);
		m_private.WorldMatrix = XMMatrixIdentity();
		m_private.OrthoMatrix = XMMatrixOrthographicLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);

		this->get().ViewportCnt = 1U;

		XPLICIT_INFO("[DriverSystemD3D11::DriverSystemD3D11] Driver is On-Line.");

		this->get().pContext->RSSetViewports(this->get().ViewportCnt, &this->get().Viewport);
	}

	const char* DriverSystemD3D11::name() noexcept { return ("DriverSystemD3D11"); }

	RENDER_SYSTEM DriverSystemD3D11::api() noexcept { return RENDER_SYSTEM::DIRECT3D11; }

	DriverSystemD3D11::DriverTraits& DriverSystemD3D11::get() noexcept { return m_private; }

	void DriverSystemD3D11::begin_scene(
		const float& a, 
		const float& r,
		const float& g, 
		const float& b, 
		const bool zBuffer, 
		const bool depth)
	{
		XPLICIT_ASSERT(m_private.pContext);
		XPLICIT_ASSERT(m_private.pCamera);
		XPLICIT_ASSERT(m_private.pRenderTarget);

		float rgba[4];

		rgba[0] = r;
		rgba[1] = g;
		rgba[2] = b;
		rgba[3] = a;

		m_private.pContext->ClearRenderTargetView(m_private.pRenderTarget.Get(), rgba);
		m_private.pContext->ClearDepthStencilView(m_private.pDepthStencil.Get(), depth ? (D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL) : 0, 1.0f, 0);
	
		m_private.pCamera->render_camera();
	}

	bool DriverSystemD3D11::check_device_removed(HRESULT hr)
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED || 
			hr == DXGI_ERROR_DEVICE_RESET)
		{
			HRESULT hr = get().pDevice->GetDeviceRemovedReason();

			_com_error com(hr);

			DialogHelper::message_box(L"DirectX Driver (XAGE)",
				L"Device removed!",
				com.ErrorMessage(), TD_ERROR_ICON,
				TDCBF_OK_BUTTON);

			return false;
		}
		else
		{
			Details::ThrowIfFailed(hr);
		}

		return true;
	}

	bool DriverSystemD3D11::end_scene() 
	{
		XPLICIT_ASSERT(m_private.pSwapChain);
		XPLICIT_ASSERT(m_private.pContext);

		m_private.hResult = m_private.pSwapChain->Present(m_private.bVSync, 0);

		return this->check_device_removed(m_private.hResult);
	}

	void DriverSystemD3D11::close() noexcept { m_private.bEndRendering = true; }

	const bool& DriverSystemD3D11::is_closed() noexcept { return m_private.bEndRendering; }

	DriverSystemD3D11::operator bool() { return is_closed(); }

	std::unique_ptr<DriverSystemD3D11> make_driver_system_d3d11(HWND hwnd, UINT width, UINT height) 
	{ 
		XPLICIT_ASSERT(hwnd);

		return std::make_unique<DriverSystemD3D11>(hwnd, width, height); 
	}

	LightSystemD3D11::LightSystemD3D11(const std::size_t& verts)
		: m_pLightPs(nullptr), m_pLightVs(nullptr), 
		m_pSamplerState(nullptr), m_hResult(S_OK),
		m_pMatrixBuffer(nullptr), m_pLightBuffer(nullptr),
		m_pCameraBuffer(nullptr)
	{
		XPLICIT_GET_DATA_DIR_W(DIR);

		PString path_pixel = DIR;
		path_pixel += L"Shaders/Light.ps.hlsl";

		m_pLightPs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), "PS", RENDERER);

		PString path_vertex = DIR;
		path_vertex += L"Shaders/Light.vs.hlsl";

		m_pLightVs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Vertex>(path_vertex.c_str(), "VS", RENDERER);

		D3D11_INPUT_ELEMENT_DESC input_layout[] = {
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		const auto layout_size = sizeof(input_layout) / sizeof(input_layout[0]);

		Details::ThrowIfFailed(RENDERER->get().pDevice->CreateInputLayout(
			input_layout, 
			layout_size,
			m_pLightVs->get().pBlob->GetBufferPointer(), 
			m_pLightVs->get().pBlob->GetBufferSize(),
			m_pInputLayout.GetAddressOf()));

		D3D11_SAMPLER_DESC samplerDesc{};

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_hResult = RENDERER->get().pDevice->CreateSamplerState(&samplerDesc, m_pSamplerState.GetAddressOf());

		Details::ThrowIfFailed(m_hResult);

		D3D11_BUFFER_DESC matrixBufferDesc{};

		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(Details::VERTEX) * verts;
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		m_hResult = RENDERER->get().pDevice->CreateBuffer(&matrixBufferDesc,
			nullptr,
			m_pMatrixBuffer.GetAddressOf());

		Details::ThrowIfFailed(m_hResult);

		D3D11_BUFFER_DESC lightBufferDesc{};
	
		lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		lightBufferDesc.ByteWidth = sizeof(Details::LIGHT) * verts;
		lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightBufferDesc.MiscFlags = 0;
		lightBufferDesc.StructureByteStride = 0;

		m_hResult = RENDERER->get().pDevice->CreateBuffer(&lightBufferDesc,
			nullptr,
			m_pLightBuffer.GetAddressOf());

		Details::ThrowIfFailed(m_hResult);

		D3D11_BUFFER_DESC cameraBufferDesc{};

		cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cameraBufferDesc.ByteWidth = sizeof(Details::CAMERA_POS) * verts;
		cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cameraBufferDesc.MiscFlags = 0;
		cameraBufferDesc.StructureByteStride = 0;

		m_hResult = RENDERER->get().pDevice->CreateBuffer(&cameraBufferDesc,
			nullptr,
			m_pCameraBuffer.GetAddressOf());

		Details::ThrowIfFailed(m_hResult);
	}

	LightSystemD3D11::~LightSystemD3D11()
	{
		if (m_pLightPs)
			delete m_pLightPs;

		if (m_pLightVs)
			delete m_pLightVs;
	}

	void LightSystemD3D11::update(const std::size_t indexCount) noexcept
	{
		m_pLightVs->update_light_shader(this);
		m_pLightPs->update_light_shader(this);

		RENDERER->get().pContext->IASetInputLayout(m_pInputLayout.Get());

		m_pLightVs->update();
		m_pLightPs->update();

		RENDERER->get().pContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());

		RENDERER->get().pContext->DrawIndexed(indexCount, 0, 0);
	}

	//! Render-able class
	//! Used to render mesh, triangles and more.

	RenderableComponentD3D11::RenderableComponentD3D11() noexcept
		: m_vertexData(), m_hResult(0), m_vertexBufferDesc(), 
		m_indexBufDesc(), m_pVertexBuffer(nullptr),
		 m_pDriver(nullptr), m_pVertex(nullptr),
		m_indexData(), m_iVertexCnt(0), m_iTopology(XPLICIT_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
		m_pMatrixBuffer(nullptr), m_iIndices(0),
		m_pVertexShader(nullptr), m_pTextureShader(nullptr),
		m_vPosition(0, 0, 0), m_vRotation(0, 0, 0), m_bDraw(true),
		m_vScale(1, 1, 1), f_pSourceLight(nullptr), m_iSamplerCnt(1),
		m_vTextures()
	{}

	RenderableComponentD3D11::~RenderableComponentD3D11()
	{
		if (m_pVertex)
			delete[] m_pVertex;

		for (auto* tex : m_vTextures)
		{
			delete tex;
		}
	}

	DriverSystemD3D11* RenderableComponentD3D11::driver() noexcept { return m_pDriver; }

	void RenderableComponentD3D11::push_ambient(const Color<float>& vert) noexcept { this->m_arrayColorsAmbient.push_back(vert); }

	void RenderableComponentD3D11::push_specular(const Color<float>& vert) noexcept { this->m_arrayColorsSpecular.push_back(vert); }

	void RenderableComponentD3D11::push_diffuse(const Color<float>& vert) noexcept { this->m_arrayColorsDiffuse.push_back(vert); }

	void RenderableComponentD3D11::push_normal(const Vector<float>& vert) noexcept { this->m_arrayNormal.push_back(vert); }

	void RenderableComponentD3D11::push_vertice(const Vector<float>& vert) noexcept { this->m_arrayVerts.push_back(vert); }

	void RenderableComponentD3D11::push_indice(const UINT& indice) noexcept { this->m_arrayIndices.push_back(indice);  }

	void RenderableComponentD3D11::should_draw(const bool enable) noexcept { m_bDraw = enable; }
	
	const bool& RenderableComponentD3D11::should_draw() noexcept { return m_bDraw; }

	void RenderableComponentD3D11::make_renderable(const std::vector<ImageDataParams>& params)
	{
		if (m_arrayVerts.empty())
			return;
		
		RtlZeroMemory(&m_vertexData, sizeof(D3D11_SUBRESOURCE_DATA));
		RtlZeroMemory(&m_vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		RtlZeroMemory(&m_indexBufDesc, sizeof(D3D11_BUFFER_DESC));

		if (!m_pVertex)
			m_pVertex = new Details::VERTEX[m_arrayVerts.size()];

		XPLICIT_ASSERT(m_pVertex);

		m_iVertexCnt = 0;

		for (size_t vertex_index = 0; 
			vertex_index < m_arrayVerts.size(); 
			++vertex_index)
		{
			m_pVertex[vertex_index].POSITION = XMFLOAT4(
				m_arrayVerts[vertex_index].X,
				m_arrayVerts[vertex_index].Y, 
				m_arrayVerts[vertex_index].Z, 
				1.0f);
		}

		for (size_t tex_index = 0;
			tex_index < m_arrayTextures.size(); 
			++tex_index)
		{
			m_pVertex[tex_index].TEXCOORD = XMFLOAT2(
				m_arrayTextures[tex_index].X,
				m_arrayTextures[tex_index].Y);
		}

		for (size_t ambient_index = 0;
			ambient_index < m_arrayColorsAmbient.size(); ++ambient_index)
		{
			m_pVertex[ambient_index].AMBIENT = XMFLOAT3(
				m_arrayColorsAmbient[ambient_index].R,
				m_arrayColorsAmbient[ambient_index].G,
				m_arrayColorsAmbient[ambient_index].B);
		}

		m_iVertexCnt = m_arrayVerts.size();

		m_vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_vertexBufferDesc.ByteWidth = sizeof(Details::VERTEX) * m_iVertexCnt;
		m_vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_vertexBufferDesc.CPUAccessFlags = 0;
		m_vertexBufferDesc.MiscFlags = 0;
		m_vertexBufferDesc.StructureByteStride = 0;
		
		m_vertexData.pSysMem = &m_pVertex[0];
		m_vertexData.SysMemPitch = 0;
		m_vertexData.SysMemSlicePitch = 0;

		m_hResult = m_pDriver->get().pDevice->CreateBuffer(&m_vertexBufferDesc, 
			&m_vertexData, 
			m_pVertexBuffer.GetAddressOf());
		
		if (FAILED(m_hResult))
		{
			delete[] m_pVertex;
			throw Win32Error("Driver error (RenderableComponentD3D11::make_renderable(CreateBuffer(m_vertex_buffer))");
		}

		delete[] m_pVertex;

		XPLICIT_GET_DATA_DIR_W(DIR);

		PString path_pixel = DIR;
		path_pixel += L"Shaders/Renderable.ps.hlsl";

		m_pTextureShader = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), "PS", this->m_pDriver);

		PString path_vertex = DIR;
		path_vertex += L"Shaders/Renderable.vs.hlsl";

		m_pVertexShader = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Vertex>(path_vertex.c_str(), "VS", this->m_pDriver);

		D3D11_INPUT_ELEMENT_DESC input_layout[] = {
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // MESH POSITION
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // TEXTURE COORD.
					{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // AMBIENT COLOR
		};

		const auto layout_size = sizeof(input_layout) / sizeof(input_layout[0]);

		m_hResult = m_pDriver->get().pDevice->CreateInputLayout(input_layout, layout_size,
			m_pVertexShader->m_data.pBlob->GetBufferPointer(), m_pVertexShader->m_data.pBlob->GetBufferSize(),
			&m_pVertexShader->m_data.pInputLayout);

		Details::ThrowIfFailed(m_hResult);

		UINT* indices = new UINT[m_arrayIndices.size()];
		memset(indices, 0, m_arrayIndices.size());

		std::size_t i = 0UL;

		for (size_t vertex_index = 0; vertex_index < m_arrayIndices.size(); ++vertex_index)
			indices[vertex_index] = m_arrayIndices[vertex_index];

		m_iIndices = m_arrayIndices.size();

		m_indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
		m_indexBufDesc.ByteWidth = sizeof(ULONG) * m_iIndices;
		m_indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		m_indexBufDesc.CPUAccessFlags = 0;
		m_indexBufDesc.MiscFlags = 0;
		m_indexBufDesc.StructureByteStride = 0;

		m_indexData.pSysMem = &indices[0];
		m_indexData.SysMemPitch = 0;
		m_indexData.SysMemSlicePitch = 0;

		m_hResult = m_pDriver->get().pDevice->CreateBuffer(&m_indexBufDesc, &m_indexData, m_pIndexBuffer.GetAddressOf());

		if (FAILED(m_hResult))
			throw Win32Error("DirectX Error (D3D11RenderComponent::create(CreateBuffer(m_pIndexBuffer))");

		delete[] indices;

		D3D11_BUFFER_DESC matrixBufferDesc{};

		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(Details::CBUFFER);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		m_hResult = m_pDriver->get().pDevice->CreateBuffer(&matrixBufferDesc, nullptr, m_pMatrixBuffer.GetAddressOf());

		Details::ThrowIfFailed(m_hResult);

		for (auto& tex : params)
		{
			auto tex_ptr = new TextureSystemD3D11();
			tex_ptr->m_pDriver = m_pDriver;

			m_vTextures.push_back(tex_ptr);
			tex_ptr->make_texture(tex);

			delete[] tex.pImage;
		}

		D3D11_SAMPLER_DESC samplerDesc{};

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_hResult = m_pDriver->get().pDevice->CreateSamplerState(&samplerDesc, m_pSamplerState.GetAddressOf());

		Details::ThrowIfFailed(m_hResult);
	}

	void RenderableComponentD3D11::set_scale(const Vector<float>& scale) noexcept { m_vScale = scale; }
	
	const Vector<float>& RenderableComponentD3D11::scale() noexcept { return m_vScale; }

	void RenderableComponentD3D11::set_rotation(const Vector<float>& rot) noexcept { m_vRotation = rot; }
	
	const Vector<float>& RenderableComponentD3D11::rotation() noexcept { return m_vRotation; }

	void RenderableComponentD3D11::set_position(const Vector<float>& pos) noexcept { m_vPosition = pos; }
	
	const Vector<float>& RenderableComponentD3D11::position() noexcept { return m_vPosition; }

	const char* RenderableComponentD3D11::name() noexcept { return ("RenderableComponentD3D11"); }

	COMPONENT_TYPE RenderableComponentD3D11::type() noexcept { return COMPONENT_RENDER; }

	void RenderableComponentD3D11::set_driver(DriverSystemD3D11* driver) noexcept
	{
		if (driver)
			m_pDriver = driver;
	}

	bool RenderableComponentD3D11::should_update() noexcept { return true; }

	void RenderableComponentD3D11::update(ClassPtr this_ptr) 
	{
		RenderableComponentD3D11* self = (RenderableComponentD3D11*)this_ptr;

		if (!IsValidHeapPtr(self) ||
			!self->m_bDraw ||
			!self->m_pDriver)
			return;

		const uint32_t stride = (uint32_t)(sizeof(Details::VERTEX));
		const uint32_t offset = { 0u };

		self->m_pDriver->get().pContext->RSSetState(self->m_pDriver->get().pRasterState.Get());

		self->m_pDriver->get().pContext->IASetPrimitiveTopology(self->m_iTopology);

		self->m_pDriver->get().pContext->OMSetRenderTargets(self->m_pDriver->get().ViewportCnt,
			self->m_pDriver->get().pRenderTarget.GetAddressOf(),
			self->m_pDriver->get().pDepthStencil.Get());

		self->m_pDriver->get().pContext->IASetInputLayout(self->m_pVertexShader->m_data.pInputLayout);

		self->m_pVertexShader->update_render_shader(self);

		std::vector<ID3D11ShaderResourceView*> textures;

		for (auto& tex : self->m_vTextures)
		{
			textures.push_back(tex->m_pTextureView.Get());
		}

		self->m_pDriver->get().pContext->IASetIndexBuffer(self->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		self->m_pDriver->get().pContext->IASetVertexBuffers(0, self->m_pDriver->get().ViewportCnt,
			self->m_pVertexBuffer.GetAddressOf(),
			&stride,
			&offset);
		

		if (self->f_pSourceLight)
		{
			self->m_pDriver->get().pContext->PSSetShaderResources(0, textures.size(), textures.data());
			self->f_pSourceLight->update(self->m_iIndices);
		}
		else
		{
			self->m_pTextureShader->update();

			self->m_pVertexShader->update();

			self->m_pDriver->get().pContext->PSSetShaderResources(0, textures.size(), textures.data());

			self->m_pDriver->get().pContext->PSSetSamplers(0, self->m_iSamplerCnt, self->m_pSamplerState.GetAddressOf());

			self->m_pDriver->get().pContext->DrawIndexed(self->m_iIndices, 0, 0);
		}
	}

	const size_t& RenderableComponentD3D11::get_vertices_count() noexcept { return m_iVertexCnt; }

	const size_t& RenderableComponentD3D11::get_indices_count() noexcept { return m_iIndices; }

	void CameraSystemD3D11::render_camera() noexcept
	{
		XMFLOAT3 lookAt{};
		XMFLOAT3 up{};

		float yaw, pitch, roll;
		XMMATRIX rotationMatrix{};

		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 1.0f;

		XMVECTOR upVector = XMLoadFloat3(&up);

		XMFLOAT3 position{};

		position.x = m_vPos.X;
		position.y = m_vPos.Y;
		position.z = m_vPos.Z;

		XMVECTOR positionVector = XMLoadFloat3(&position);

		lookAt.x = 0.0f;
		lookAt.y = 0.0f;
		lookAt.z = 1.0f;

		XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

		// radian shit
		pitch = m_vRot.X * 0.0174532925f;
		yaw = m_vRot.Y * 0.0174532925f;
		roll = m_vRot.Z * 0.0174532925f;

		m_rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

		lookAtVector = XMVector3TransformCoord(lookAtVector, m_rotationMatrix);
		upVector = XMVector3TransformCoord(upVector, m_rotationMatrix);

		lookAtVector = XMVectorAdd(positionVector, lookAtVector);

		m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
	}
}

#endif // XPLICIT_WINDOWS