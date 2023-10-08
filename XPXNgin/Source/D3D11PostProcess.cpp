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

			m_pPs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), 
				"PS", RENDERER);

			PString path_vertex = DIR;
			path_vertex += L"Shaders/Blur.vs.hlsl";

			m_pVs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), 
				"VS", RENDERER);

			break;
		}
		case FOCUS:
		{
			PString path_pixel = DIR;
			path_pixel += L"Shaders/Focus.ps.hlsl";

			m_pPs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), 
				"PS", RENDERER);

			PString path_vertex = DIR;
			path_vertex += L"Shaders/Focus.vs.hlsl";

			m_pVs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), 
				"VS", RENDERER);

			break;
		}
		case SHAKE:
		{
			PString path_pixel = DIR;
			path_pixel += L"Shaders/Shake.ps.hlsl";

			m_pPs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), 
				"PS", RENDERER);

			PString path_vertex = DIR;
			path_vertex += L"Shaders/Shake.vs.hlsl";

			m_pVs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(),
				"VS", RENDERER);

			break;
		}
		case BLOOM:
		{
			PString path_pixel = DIR;
			path_pixel += L"Shaders/Bloom.ps.hlsl";

			m_pPs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), "PS", RENDERER);

			PString path_vertex = DIR;
			path_vertex += L"Shaders/Bloom.vs.hlsl";

			m_pVs = D3D11ShaderHelper1::make_shader<XPLICIT_SHADER_TYPE::Pixel>(path_pixel.c_str(), "PS", RENDERER);

			break;
		}
		default:
			throw EngineError("This Post process effect does not exist.");
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

		D3D11_INPUT_ELEMENT_DESC input_layout[] = {
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // MESH POSITION
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // TEXTURE COORD.
					{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // AMBIENT COLOR
		};

		const auto layout_size = sizeof(input_layout) / sizeof(input_layout[0]);

		hr = RENDERER->get().pDevice->CreateInputLayout(input_layout, 
			layout_size, 
			m_pVs->get().pBlob->GetBufferPointer(),
			m_pVs->get().pBlob->GetBufferSize(), 
			m_pInputLayout.GetAddressOf());

		Details::ThrowIfFailed(hr);
	}

	PostProcessEffectD3D11::~PostProcessEffectD3D11()
	{
		delete m_pVs;
		delete m_pPs;
	}

	void PostProcessEffectD3D11::update() noexcept
	{
		RENDERER->get().pContext->IASetInputLayout(m_pInputLayout.Get());
		RENDERER->get().pContext->RSSetState(m_pPPState.Get());

		m_pVs->update();
		m_pPs->update();


	}
}