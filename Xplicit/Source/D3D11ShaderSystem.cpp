/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: DriverD3D11.cpp
 *			Purpose: C++ Shader System for Direct3D 11
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "DriverD3D11.h"
#include <d3dcompiler.h>

#ifdef XPLICIT_WINDOWS

namespace Xplicit::Renderer::DX11
{
	D3D11ShaderSystem::~D3D11ShaderSystem()
	{
		if (m_data.pBlob)
			m_data.pBlob->Release();

		if (m_data.pErrorBlob)
			m_data.pErrorBlob->Release();
	}

	int D3D11ShaderSystem::compile() noexcept
	{
		HRESULT hr = D3DCompileFromFile(m_shader.c_str(), nullptr, nullptr, m_data.entrypoint.c_str(), m_data.shader_type.c_str(),
			m_data.iFlags1,
			m_data.iFlags2,
			&m_data.pBlob,
			&m_data.pErrorBlob);

		return SUCCEEDED(hr) ? 0 : 1;
	}

	D3D11ShaderSystem::ShaderTraits& D3D11ShaderSystem::get() { return m_data; }

	void D3D11ShaderSystem::update(D3D11RenderComponent* component)
	{
		if (!component)
			return;

		if (this->m_data.pInputLayout)
			component->m_pDriver->get().pCtx->IASetInputLayout(this->m_data.pInputLayout.Get());
	
		if (this->m_data.pVertex)
			component->m_pDriver->get().pCtx->VSSetShader(this->m_data.pVertex.Get(), nullptr, 0);

		if (this->m_data.pPixel)
			component->m_pDriver->get().pCtx->PSSetShader(this->m_data.pPixel.Get(), nullptr, 0);

		if (this->m_data.pHull)
			component->m_pDriver->get().pCtx->HSSetShader(this->m_data.pHull.Get(), nullptr, 0);
	}

	HRESULT D3D11ShaderSystem::ShaderTraits::create_input_layout(ID3D11Device* device)
	{
		HRESULT hr = S_OK;

		if (!device)
			return hr;

		hr = device->CreateInputLayout(input_layouts.data(), input_layouts.size(), pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(), pInputLayout.GetAddressOf());

		XPLICIT_ASSERT(SUCCEEDED(hr));
		return hr;
	}
}

#endif