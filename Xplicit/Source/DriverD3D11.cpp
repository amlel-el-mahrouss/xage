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

#ifdef XPLICIT_WINDOWS

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
		XPLICIT_INFO("[DIRECTX] xplicit_d3d11_make_swapchain was a success!");
#endif
	}

	DriverSystemD3D11::DriverSystemD3D11(HWND hwnd)
		: m_swap_desc(), m_private()
	{
		m_private.WindowHandle = hwnd;
		xplicit_d3d11_make_swapchain(m_swap_desc, m_private);

		Microsoft::WRL::ComPtr<IDXGIFactory> factory;
		CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));

		if (FAILED(factory->EnumAdapters(0, m_private.Adapter.GetAddressOf())))
			throw EngineError();

		if (m_private.Adapter)
		{
			DxgiOutputResult outputs = xplicit_enum_outputs(m_private.Adapter.Get());
		
#ifdef XPLICIT_DEBUG
			DXGI_OUTPUT_DESC desc;

			for (auto* output : outputs.Outputs)
			{
				RtlZeroMemory(&desc, sizeof(DXGI_OUTPUT_DESC));
		
				output->GetDesc(&desc);

				XPLICIT_INFO("AttachedToDesktop: ", desc.AttachedToDesktop ? "true" : "false");
				XPLICIT_INFO(desc.DeviceName);
			}

#ifdef XPLICIT_DEBUG
			XPLICIT_INFO("[DIRECTX] DriverSystemD3D11::DriverSystemD3D11(HWND hwnd) was a success!");
#endif
#endif
		}
	}

	DriverSystemD3D11::~DriverSystemD3D11() 
	{
		
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
		assert(hwnd);
		return std::make_unique<DriverSystemD3D11>(hwnd); 
	}
}

#endif // XPLICIT_WINDOWS