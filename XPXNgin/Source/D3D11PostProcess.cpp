/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: D3D11PostProcess.cpp
 *			Purpose: C++ Post process API.
 *
 * =====================================================================
 */

#include "DriverD3D11.h"

namespace XPX::Renderer::DX11
{
	PostProcessEffectD3D11::PostProcessEffectD3D11(int effect_type)
		: m_rasterDesc()
	{
		XPLICIT_GET_DATA_DIR_W(DIR);


		switch (effect_type)
		{
		case BLUR:
		{
			PString path_pixel = DIR;
			path_pixel += L"Shaders/Blur.ps.hlsl";

			m_pPs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), "PS", RENDERER);

			PString path_vertex = DIR;
			path_vertex += L"Shaders/Blur.vs.hlsl";

			m_pVs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), "PS", RENDERER);

			break;
		}
		case FOCUS:
		{
			PString path_pixel = DIR;
			path_pixel += L"Shaders/Focus.ps.hlsl";

			m_pPs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), "PS", RENDERER);

			PString path_vertex = DIR;
			path_vertex += L"Shaders/Focus.vs.hlsl";

			m_pVs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), "PS", RENDERER);

			break;
		}
		case SHAKE:
		{
			PString path_pixel = DIR;
			path_pixel += L"Shaders/Shake.ps.hlsl";

			m_pPs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), "PS", RENDERER);

			PString path_vertex = DIR;
			path_vertex += L"Shaders/Shake.vs.hlsl";

			m_pVs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), "PS", RENDERER);

			break;
		}
		default:
			throw EngineError("This PostProcess effect does not exist.");
		}

		m_rasterDesc.AntialiasedLineEnable = true;
		m_rasterDesc.CullMode = D3D11_CULL_NONE;
		m_rasterDesc.ScissorEnable = true;
		m_rasterDesc.MultisampleEnable = false;
		m_rasterDesc.SlopeScaledDepthBias = 0.0f;
		m_rasterDesc.DepthBias = 0.0f;
		m_rasterDesc.DepthBiasClamp = 0.0f;

		HRESULT hr = RENDERER->get().pDevice->CreateRasterizerState(&m_rasterDesc, m_pPPState.GetAddressOf());
		Details::ThrowIfFailed(hr);


	}

	PostProcessEffectD3D11::~PostProcessEffectD3D11()
	{
		delete m_pVs;
		delete m_pPs;
	}
}