/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: DriverD3D11.cpp
 *			Purpose: C++ Rendering Driver for Direct3D 11
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "DriverD3D11.h"
#include "Bites.h"

#ifdef XPLICIT_WINDOWS

namespace Xplicit::Renderer::DX11
{
	namespace Details
	{
		void ThrowIfFailed(HRESULT hr)
		{
			if (FAILED(hr))
				throw Win32Error("[ThrowIfFailed] Fatal DirectX error!");
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

	static void xplicit_d3d11_make_swapchain(DXGI_SWAP_CHAIN_DESC& swapDesc, DriverSystemD3D11::DriverTraits& privateData)
	{
		RtlZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapDesc.BufferCount = 1;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.Width = XPLICIT_DEFAULT_WIDTH;
		swapDesc.BufferDesc.Height = XPLICIT_DEFAULT_HEIGHT;

		swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapDesc.BufferDesc.RefreshRate.Numerator = 0;

		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;

		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		
		swapDesc.Flags = 0;

		swapDesc.Windowed = true;
		swapDesc.OutputWindow = privateData.pWindowHandle;

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("[xplicit_d3d11_make_swapchain] was a success!");
#endif

	}

	DriverSystemD3D11::DriverSystemD3D11(HWND hwnd)
		: m_private()
	{
		m_private.pWindowHandle = hwnd;
		xplicit_d3d11_make_swapchain(m_private.SwapDesc, m_private);

		this->setup();
	}

	DriverSystemD3D11::~DriverSystemD3D11() 
	{
		if (m_private.pSwapChain)
			m_private.pSwapChain->SetFullscreenState(false, nullptr);
	}

	void DriverSystemD3D11::setup()
	{
		const D3D_FEATURE_LEVEL feature[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

		auto hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
#ifdef XPLICIT_DEBUG
			D3D11_CREATE_DEVICE_DEBUG,
#else
			0,
#endif
			feature,
			ARRAYSIZE(feature),
			D3D11_SDK_VERSION,
			&m_private.SwapDesc,
			m_private.pSwapChain.GetAddressOf(),
			m_private.pDevice.GetAddressOf(),
			nullptr,
			m_private.pCtx.GetAddressOf()
		);

		if (FAILED(hr))
			throw Win32Error("[D3D11CreateDeviceAndSwapChain] Failed to call function correctly!");

		hr = m_private.pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_private.pRenderTexture.GetAddressOf()));

		if (FAILED(hr))
			throw Win32Error("[GetBuffer] Failed to call function correctly!");

		hr = m_private.pDevice->CreateRenderTargetView(m_private.pRenderTexture.Get(), nullptr, m_private.pRenderTarget.GetAddressOf());

		if (FAILED(hr))
			throw Win32Error("[CreateRenderTargetView] Failed to call function correctly!");

		D3D11_TEXTURE2D_DESC depthBufferDesc;
		RtlZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

		depthBufferDesc.Width = XPLICIT_MIN_WIDTH;
		depthBufferDesc.Height = XPLICIT_MIN_HEIGHT;
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
			throw Win32Error("[CreateTexture2D] Failed to call function correctly!");

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
			throw Win32Error("[CreateDepthStencilState] Failed to call function correctly!");

		m_private.pCtx->OMSetDepthStencilState(m_private.pDepthStencilState.Get(), 1u);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		RtlZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		hr = m_private.pDevice->CreateDepthStencilView(m_private.pDepthTexture.Get(), 
			&depthStencilViewDesc,
			m_private.pDepthStencil.GetAddressOf());

		if (FAILED(hr))
			throw Win32Error("[CreateDepthStencilView] Failed to call function correctly!");

		D3D11_RASTERIZER_DESC rasterDesc;
		RtlZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		hr = m_private.pDevice->CreateRasterizerState(&rasterDesc, m_private.pRasterState.GetAddressOf());

		if (FAILED(hr))
			throw Win32Error("[CreateRasterizerState] Failed to call function correctly!");

		m_private.pCtx->RSSetState(m_private.pRasterState.Get());

		RtlZeroMemory(&m_private.Viewport, sizeof(D3D11_VIEWPORT));

		m_private.Viewport.Width = static_cast<float>(XPLICIT_MIN_WIDTH);
		m_private.Viewport.Height = static_cast<float>(XPLICIT_MIN_HEIGHT);

		m_private.Viewport.MinDepth = 0.0f;
		m_private.Viewport.MaxDepth = 1.0f;
		m_private.Viewport.TopLeftX = 0.0f;
		m_private.Viewport.TopLeftY = 0.0f;

		m_private.pCtx->RSSetViewports(1U, &m_private.Viewport);

		XPLICIT_INFO("[DriverSystemD3D11::DriverSystemD3D11] driver created.");
	}

	const char* DriverSystemD3D11::name() noexcept { return ("DriverSystemD3D11"); }

	RENDER_SYSTEM DriverSystemD3D11::api() noexcept { return RENDER_SYSTEM::DIRECT3D11; }

	DriverSystemD3D11::DriverTraits& DriverSystemD3D11::get() noexcept { return m_private; }

	void DriverSystemD3D11::begin_scene(const float a, const float r, const float g, const float b) 
	{
		XPLICIT_ASSERT(m_private.pCtx);
		XPLICIT_ASSERT(m_private.pRenderTarget);
		XPLICIT_ASSERT(m_private.pDepthStencil);

		float rgba[4]{ r, g, b, a };

		m_private.pCtx->OMSetRenderTargets(1, m_private.pRenderTarget.GetAddressOf(), m_private.pDepthStencil.Get());

		m_private.pCtx->ClearRenderTargetView(m_private.pRenderTarget.Get(), rgba);
		m_private.pCtx->ClearDepthStencilView(m_private.pDepthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	void DriverSystemD3D11::handle_device_removed()
	{
		Details::ThrowIfFailed(E_FAIL);
	}

	bool DriverSystemD3D11::check_device_removed(HRESULT hr)
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			this->handle_device_removed();
			return true;
		}
		else
		{
			Details::ThrowIfFailed(hr);
		}

		return false;
	}

	bool DriverSystemD3D11::end_scene() 
	{
		XPLICIT_ASSERT(m_private.pSwapChain);
		XPLICIT_ASSERT(m_private.pCtx);

		HRESULT hr = m_private.pSwapChain->Present(m_private.bVSync ? 1 : 0, 0);

		return !this->check_device_removed(hr);
	}

	void DriverSystemD3D11::close() noexcept { m_private.bEndRendering = true; }

	const bool& DriverSystemD3D11::is_closed() noexcept { return m_private.bEndRendering; }

	DriverSystemD3D11::operator bool() { return is_closed(); }

	std::unique_ptr<DriverSystemD3D11> make_driver_system_d3d11(HWND hwnd) 
	{ 
		XPLICIT_ASSERT(hwnd);

		return std::make_unique<DriverSystemD3D11>(hwnd); 
	}

	RenderComponentD3D11::RenderComponentD3D11()
		: m_vertexData(), m_hResult(0), m_vertexBufferDesc(), 
		m_indexBufDesc(), m_pVertexBuffer(nullptr),
		 m_pDriver(nullptr), m_pVertex(nullptr),
		m_indexData(), m_iVertexCnt(0)
	{
		
	}

	RenderComponentD3D11::~RenderComponentD3D11()
	{
		if (m_pVertex)
			delete[] m_pVertex;
	}

	void RenderComponentD3D11::push(const Vector<float>& vert)
	{
		this->m_arrayVerts.push_back(vert);
	}

	// this needs to be more generic.
	void RenderComponentD3D11::create()
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

		for (size_t vertex_index = 0; vertex_index < m_arrayVerts.size(); ++vertex_index)
		{
			m_pVertex[vertex_index].position = XMVectorSet(m_arrayVerts[vertex_index].X, 
				m_arrayVerts[vertex_index].Y,
				m_arrayVerts[vertex_index].Z,
				0.f);
		
			m_pVertex[vertex_index].color = XMVectorSet(1.f,
				1.f,
				1.f,
				1.f);

			++m_iVertexCnt;
		}

		m_vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_vertexBufferDesc.ByteWidth = sizeof(Details::VERTEX) * m_iVertexCnt;
		m_vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_vertexBufferDesc.CPUAccessFlags = 0;
		m_vertexBufferDesc.MiscFlags = 0;
		m_vertexBufferDesc.StructureByteStride = 0;

		m_vertexData.pSysMem = m_pVertex;
		m_vertexData.SysMemPitch = 0;
		m_vertexData.SysMemSlicePitch = 0;

		m_hResult = m_pDriver->get().pDevice->CreateBuffer(&m_vertexBufferDesc, &m_vertexData, m_pVertexBuffer.GetAddressOf());
		
		if (FAILED(m_hResult))
			throw Win32Error("DirectX Error (D3D11RenderComponent::create(CreateBuffer(m_vertex_buffer))");

		m_indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
		m_indexBufDesc.ByteWidth = sizeof(ULONG) * m_iVertexCnt;
		m_indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		m_indexBufDesc.CPUAccessFlags = 0;
		m_indexBufDesc.MiscFlags = 0;
		m_indexBufDesc.StructureByteStride = 0;

		UINT* pIndices = new UINT[m_iVertexCnt];

		for (size_t i = 0; i < m_iVertexCnt; i++)
		{
			pIndices[i] = i;
		}

		m_indexData.pSysMem = pIndices;
		m_indexData.SysMemPitch = 0;
		m_indexData.SysMemSlicePitch = 0;

		m_hResult = m_pDriver->get().pDevice->CreateBuffer(&m_indexBufDesc, &m_indexData, m_pIndexBuffer.GetAddressOf());

		if (FAILED(m_hResult))
		{
			delete[] pIndices;
			throw Win32Error("DirectX Error (D3D11RenderComponent::create(CreateBuffer(m_pIndexBuffer))");
		}

		delete[] pIndices;
	}

	bool RenderComponentD3D11::should_update() noexcept { return true; }

	const char* RenderComponentD3D11::name() noexcept { return ("D3D11RenderComponent"); }

	COMPONENT_TYPE RenderComponentD3D11::type() noexcept { return COMPONENT_RENDER; }

	void RenderComponentD3D11::set_driver(DriverSystemD3D11* driver) noexcept
	{
		if (driver)
			m_pDriver = driver;
	}

	void RenderComponentD3D11::push_shader(ShaderSystemD3D11* shaderSystem) noexcept
	{
		if (shaderSystem)
			m_pShader.push_back(shaderSystem);
	}
	
	void RenderComponentD3D11::update() 
	{
		if (m_pShader.empty() ||
			!m_pDriver ||
			m_iVertexCnt < 1)
			return;

		static const uint32_t stride[] = { sizeof(Details::VERTEX) };
		static const uint32_t offset = 0;

		m_pDriver->get().pCtx->IASetVertexBuffers(1, 1, m_pVertexBuffer.GetAddressOf(), stride, &offset);
		m_pDriver->get().pCtx->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		m_pDriver->get().pCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (size_t shader_index = 0; shader_index < m_pShader.size(); ++shader_index)
		{
			m_pShader[shader_index]->update(this);
		}

		m_pDriver->get().pCtx->DrawIndexed(m_iVertexCnt, 0, 0);
	}

	size_t RenderComponentD3D11::size() noexcept
	{
		return m_iVertexCnt;
	}
}

#endif // XPLICIT_WINDOWS