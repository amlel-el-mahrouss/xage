/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright XPX, all rights reserved.
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
		static void ThrowIfFailed(HRESULT hr)
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
		XPLICIT_INFO("[xplicit_d3d11_make_swapchain] was a success!");
#endif

	}

	DriverSystemD3D11::DriverSystemD3D11(HWND hwnd)
		: m_private()
	{
		m_private.WindowHandle = hwnd;
		xplicit_d3d11_make_swapchain(m_private.SwapDesc, m_private);

		this->setup();
	}

	DriverSystemD3D11::~DriverSystemD3D11() 
	{
		// ComPtr will drop everything.. called after m_swap_desc, see class.
	}

	void DriverSystemD3D11::setup()
	{
		const D3D_FEATURE_LEVEL feature[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_11_1 };

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
			m_private.SwapChain.GetAddressOf(),
			m_private.Device.GetAddressOf(),
			nullptr,
			m_private.Ctx.GetAddressOf()
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

		m_private.Ctx->OMSetDepthStencilState(m_private.DepthStencilState.Get(), 1);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		RtlZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		hr = m_private.Device->CreateDepthStencilView(m_private.DepthTexture.Get(), &depthStencilViewDesc, m_private.DepthStencil.GetAddressOf());

		if (FAILED(hr))
			throw Win32Error("[CreateDepthStencilView] Failed to call function correctly!");

		m_private.Ctx->OMSetRenderTargets(1, m_private.RenderTarget.GetAddressOf(), m_private.DepthStencil.Get());

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

		m_private.Ctx->RSSetState(m_private.RasterState.Get());

		D3D11_VIEWPORT viewport;
		RtlZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.Width = static_cast<float>(XPLICIT_MIN_WIDTH);
		viewport.Height = static_cast<float>(XPLICIT_MIN_HEIGHT);

		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		m_private.Ctx->RSSetViewports(1, &viewport);

		// we are dore here, phew!

		XPLICIT_INFO("[DriverSystemD3D11::DriverSystemD3D11] Driver has been created.");
	}

	const char* DriverSystemD3D11::name() noexcept { return ("DriverSystemD3D11"); }

	RENDER_SYSTEM DriverSystemD3D11::api() { return RENDER_SYSTEM::DIRECT3D11; }

	DriverSystemD3D11::PrivateData& DriverSystemD3D11::get() noexcept { return m_private; }

	void DriverSystemD3D11::begin_scene(const float a, const float r, const float g, const float b) 
	{
		XPLICIT_ASSERT(m_private.Ctx);
		XPLICIT_ASSERT(m_private.DepthStencil);
		XPLICIT_ASSERT(m_private.RenderTarget);

		float rgba[4]{ r, g, b, a };

		m_private.Ctx->ClearRenderTargetView(m_private.RenderTarget.Get(), rgba);
		m_private.Ctx->ClearDepthStencilView(m_private.DepthStencil.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	}

	void DriverSystemD3D11::handle_device_removed()
	{

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
		XPLICIT_ASSERT(m_private.SwapChain);
		HRESULT hr = m_private.SwapChain->Present(m_private.VSync, 0);

		return !this->check_device_removed(hr);
	}

	std::unique_ptr<DriverSystemD3D11> make_driver_system_d3d11(HWND hwnd) 
	{ 
		XPLICIT_ASSERT(hwnd);

		return std::make_unique<DriverSystemD3D11>(hwnd); 
	}

	D3D11RenderComponent::D3D11RenderComponent()
		: m_vertex_data(), m_hr(0), m_vertex_buf_desc(), m_index_buf_desc(), m_vertex_buffer(nullptr),
		m_index_buffer(nullptr), m_driver(nullptr), m_index_arr(nullptr), m_vertex_arr(nullptr)
	{
		
	}

	D3D11RenderComponent::~D3D11RenderComponent()
	{
		if (m_index_arr)
			delete[] m_index_arr;

		if (m_vertex_arr)
			delete[] m_vertex_arr;
	}

	void D3D11RenderComponent::push_back(const Nplicit::Vector<float>& vert, const Nplicit::Color<float>& clr)
	{
		auto pair = std::pair<Nplicit::Vector<float>, Nplicit::Color<float>>(vert, clr);
		this->m_coord.emplace(std::move(pair));
	}

	void D3D11RenderComponent::create(std::unique_ptr<DriverSystemD3D11>& driver)
	{
		if (m_coord.empty())
			return;

		RtlZeroMemory(&m_vertex_data, sizeof(D3D11_SUBRESOURCE_DATA));
		RtlZeroMemory(&m_vertex_buf_desc, sizeof(D3D11_BUFFER_DESC));
		RtlZeroMemory(&m_index_buf_desc, sizeof(D3D11_BUFFER_DESC));

		if (!m_vertex_arr)
			m_vertex_arr = new Vertex[m_coord.size()];

		XPLICIT_ASSERT(m_vertex_arr);

		size_t i = 0;

		for (auto it = m_coord.cbegin(); it != m_coord.cend(); ++it)
		{
			m_vertex_arr[i].pos.X = it->first.X;
			m_vertex_arr[i].pos.Y = it->first.Y;
			m_vertex_arr[i].pos.Z = it->first.Z;

			m_vertex_arr[i].clr.R = it->second.R;
			m_vertex_arr[i].clr.G = it->second.G;
			m_vertex_arr[i].clr.B = it->second.B;
			m_vertex_arr[i].clr.A = it->second.A;

			++i;
		}

		m_vertex_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		m_vertex_buf_desc.ByteWidth = sizeof(Xplicit::Details::VERTEX) * m_coord.size();
		m_vertex_buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_vertex_buf_desc.CPUAccessFlags = 0;
		m_vertex_buf_desc.MiscFlags = 0;
		m_vertex_buf_desc.StructureByteStride = 0;

		m_vertex_data.pSysMem = m_vertex_arr;
		m_vertex_data.SysMemPitch = 0;
		m_vertex_data.SysMemSlicePitch = 0;

		m_hr = driver->get().Device->CreateBuffer(&m_vertex_buf_desc, &m_vertex_data, m_vertex_buffer.GetAddressOf());
		
		if (FAILED(m_hr))
			throw Win32Error("DirectX Error (D3D11RenderComponent::create)");

		m_index_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		m_index_buf_desc.ByteWidth = sizeof(ULONG) * m_coord.size();
		m_index_buf_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		m_index_buf_desc.CPUAccessFlags = 0;
		m_index_buf_desc.MiscFlags = 0;
		m_index_buf_desc.StructureByteStride = 0;

		if (!m_index_arr)
			m_index_arr = new int64_t[m_coord.size()];

		XPLICIT_ASSERT(m_vertex_arr);

		for (size_t i = 0; i < m_coord.size(); ++i)
			m_index_arr[i] = i;

		m_index_data.pSysMem = m_index_arr;
		m_index_data.SysMemPitch = 0;
		m_index_data.SysMemSlicePitch = 0;

		m_hr = driver->get().Device->CreateBuffer(&m_index_buf_desc, &m_index_data, m_index_buffer.GetAddressOf());

		if (FAILED(m_hr))
			throw Win32Error("DirectX Error (D3D11RenderComponent::create) Index Buffer initialization failed!");
	}


	bool D3D11RenderComponent::should_update() noexcept { return true; }

	const char* D3D11RenderComponent::name() noexcept { return ("D3D11RenderComponent"); }

	D3D11RenderComponent::INSTANCE_TYPE D3D11RenderComponent::type() noexcept { return INSTANCE_RENDER; }

	void D3D11RenderComponent::set(DriverSystemD3D11* driver)
	{
		if (driver)
			m_driver = driver;
	}

	void D3D11RenderComponent::update()
	{
		if (m_driver && m_driver->get().Ctx && m_vertex_buffer)
		{
			static const uint32_t stride = sizeof(Xplicit::Details::VERTEX);
			static const uint32_t offset = 0;

			m_driver->get().Ctx->IASetVertexBuffers(0, 1, m_vertex_buffer.GetAddressOf(), &stride, &offset);
			m_driver->get().Ctx->IASetIndexBuffer(m_index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			m_driver->get().Ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			for (size_t i = 0; i < m_shaders.size(); i++)
			{
				m_shaders[i]->update(this);
			}
		}
	}

	void D3D11RenderComponent::add_shader(D3D11ShaderSystem* shader)
	{
		if (shader)
			m_shaders.push_back(shader);
	}
}

#endif // XPLICIT_WINDOWS