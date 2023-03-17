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

 /**
 @file
 */

#include "DriverD3D11.h"
#include "Bites.h"

#ifdef XPLICIT_WINDOWS

#define XPLICIT_PI (3.1415926535897932384626)

namespace Xplicit::Renderer
{
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

	static void xplicit_d3d11_make_swapchain(DXGI_SWAP_CHAIN_DESC& swapDesc, DriverSystemD3D11::PrivateData& privateData)
	{
		RtlZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapDesc.BufferCount = 1;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.Windowed = true;
		swapDesc.OutputWindow = privateData.WindowHandle;

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("[DIRECTX Backend] xplicit_d3d11_make_swapchain was a success!");
#endif

	}

	DriverSystemD3D11::DriverSystemD3D11(HWND hwnd)
		: m_swap_desc(), m_private()
	{
		m_private.WindowHandle = hwnd;
		xplicit_d3d11_make_swapchain(m_swap_desc, m_private);

		this->setup();
	}

	DriverSystemD3D11::~DriverSystemD3D11() 
	{
		// ComPtr will drop everything.. called after m_swap_desc, see class.
	}

	void DriverSystemD3D11::setup()
	{
		const D3D_FEATURE_LEVEL feature[] = { D3D_FEATURE_LEVEL_11_0 , D3D_FEATURE_LEVEL_11_1 };

		auto hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			feature,
			ARRAYSIZE(feature),
			D3D11_SDK_VERSION,
			&m_swap_desc,
			m_private.SwapChain.GetAddressOf(),
			m_private.Device.GetAddressOf(),
			nullptr,
			m_private.DeviceCtx.GetAddressOf()
		);

		if (FAILED(hr))
			throw Win32Error("[D3D11CreateDeviceAndSwapChain] Failed to call function correctly!");

		Microsoft::WRL::ComPtr<ID3D11Resource> TextureRes;

		hr = m_private.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(TextureRes.GetAddressOf()));

		if (FAILED(hr))
			throw Win32Error("[GetBuffer] Failed to call function correctly!");

		hr = m_private.Device->CreateRenderTargetView(TextureRes.Get(), nullptr, m_private.RenderTarget.GetAddressOf());

		if (FAILED(hr))
			throw Win32Error("[CreateRenderTargetView] Failed to call function correctly!");

		D3D11_TEXTURE2D_DESC depthBufferDesc;
		RtlZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

		depthBufferDesc.Width = XPLICIT_MIN_WIDTH;
		depthBufferDesc.Height = XPLICIT_MIN_HEIGHT;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		hr = m_private.Device->CreateTexture2D(&depthBufferDesc, nullptr, m_private.DepthTexture.GetAddressOf());

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

		hr = m_private.Device->CreateDepthStencilState(&depthStencilDesc, m_private.DepthStencilState.GetAddressOf());

		if (FAILED(hr))
			throw Win32Error("[CreateDepthStencilState] Failed to call function correctly!");

		m_private.DeviceCtx->OMSetDepthStencilState(m_private.DepthStencilState.Get(), 1);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		RtlZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		hr = m_private.Device->CreateDepthStencilView(m_private.DepthTexture.Get(), &depthStencilViewDesc, m_private.DepthStencil.GetAddressOf());

		if (FAILED(hr))
			throw Win32Error("[CreateDepthStencilView] Failed to call function correctly!");

		m_private.DeviceCtx->OMSetRenderTargets(1, m_private.RenderTarget.GetAddressOf(), m_private.DepthStencil.Get());

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

		hr = m_private.Device->CreateRasterizerState(&rasterDesc, m_private.RasterState.GetAddressOf());

		if (FAILED(hr))
			throw Win32Error("[CreateRasterizerState] Failed to call function correctly!");

		m_private.DeviceCtx->RSSetState(m_private.RasterState.Get());

		D3D11_VIEWPORT viewport;
		RtlZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.Width = (float)XPLICIT_MIN_WIDTH;
		viewport.Height = (float)XPLICIT_MIN_HEIGHT;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		m_private.DeviceCtx->RSSetViewports(1, &viewport);

		// we are dore here, phew!

		XPLICIT_INFO("[DriverSystemD3D11::DriverSystemD3D11] Driver has been created.");
	}

	const char* DriverSystemD3D11::name() noexcept { return ("DriverSystemD3D11"); }

	RENDER_SYSTEM DriverSystemD3D11::api() { return RENDER_SYSTEM::DIRECT3D11; }

	DriverSystemD3D11::PrivateData& DriverSystemD3D11::get() noexcept { return m_private; }

	void DriverSystemD3D11::begin_scene(const float a, const float r, const float g, const float b) const 
	{
		XPLICIT_ASSERT(m_private.DeviceCtx);
		XPLICIT_ASSERT(m_private.DepthStencil);
		XPLICIT_ASSERT(m_private.RenderTarget);

		float rgba[4]{ a, r, g, b };

		m_private.DeviceCtx->ClearRenderTargetView(m_private.RenderTarget.Get(), rgba);
		m_private.DeviceCtx->ClearDepthStencilView(m_private.DepthStencil.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	}

	bool DriverSystemD3D11::end_scene() const 
	{
		XPLICIT_ASSERT(m_private.SwapChain);

		m_private.SwapChain->Present(m_private.VSync, 0);

		return true; 
	}

	std::unique_ptr<DriverSystemD3D11> make_driver_system_d3d11(HWND hwnd) 
	{ 
		XPLICIT_ASSERT(hwnd);

		return std::make_unique<DriverSystemD3D11>(hwnd); 
	}
}

#undef XPLICIT_PI

#endif // XPLICIT_WINDOWS