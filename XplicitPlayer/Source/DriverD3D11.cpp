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

	static DxgiOutputResult&& xplicit_enum_outputs(IDXGIAdapter* adapter) noexcept
	{
		DxgiOutputResult result;
		IDXGIOutput* pOutput = nullptr;

		while (adapter->EnumOutputs(result.NumOutputs, &pOutput) != DXGI_ERROR_NOT_FOUND)
		{
			result.Outputs.push_back(pOutput);
			++result.NumOutputs;
		}

		return std::move(result);
	}

	static void xplicit_d3d11_init_swapchain(DXGI_SWAP_CHAIN_DESC& swapDesc, DriverSystemD3D11::PrivateData& privateData)
	{
		RtlZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapDesc.BufferCount = 1;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.Windowed = true;
		swapDesc.OutputWindow = privateData.WindowHandle;
	}

	DriverSystemD3D11::DriverSystemD3D11(HWND hwnd)
		: m_swap_desc(), m_private()
	{
		m_private.WindowHandle = hwnd;
		xplicit_d3d11_init_swapchain(m_swap_desc, m_private);

		Microsoft::WRL::ComPtr<IDXGIFactory> factory;
		CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));

		while (FAILED(factory->EnumAdapters(0, m_private.Adapter.GetAddressOf())));

		if (m_private.Adapter)
		{
			auto outputs = xplicit_enum_outputs(m_private.Adapter.Get());
		
#ifdef XPLICIT_DEBUG
			for (auto* output : outputs.Outputs)
			{
				DXGI_OUTPUT_DESC desc;
				RtlZeroMemory(&desc, sizeof(DXGI_OUTPUT_DESC));
		
				output->GetDesc(&desc);

				XPLICIT_INFO("AttachedToDesktop: ", desc.AttachedToDesktop ? "true" : "false");
				// XPLICIT_INFO(desc.DeviceName);
			}
#endif
		}
	}

	DriverSystemD3D11::~DriverSystemD3D11() 
	{
		
	}

	const char* DriverSystemD3D11::name() noexcept { return ("DriverSystemD3D11"); }
	RENDER_SYSTEM DriverSystemD3D11::api() { return RENDER_SYSTEM::DIRECT3D11; }

	std::unique_ptr<DriverSystemD3D11> make_driver_system_d3d11(HWND hwnd) 
	{ 
		assert(hwnd);
		return std::make_unique<DriverSystemD3D11>(hwnd); 
	}
}

#endif // XPLICIT_WINDOWS