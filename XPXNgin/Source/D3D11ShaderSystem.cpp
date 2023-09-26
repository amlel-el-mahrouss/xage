/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
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

#ifdef XPLICIT_WINDOWS

#include <d3dcompiler.h>

namespace XPX::Renderer::DX11
{
	ShaderSystemD3D11::~ShaderSystemD3D11()
	{
		if (m_data.pBlob &&
			IsValidHeapPtr(m_data.pBlob))
			m_data.pBlob->Release();

		if (m_data.pErrorBlob &&
			IsValidHeapPtr(m_data.pErrorBlob))
			m_data.pErrorBlob->Release();
	}

	int ShaderSystemD3D11::compile() noexcept
	{
		HRESULT hr = D3DCompileFromFile(m_shader.c_str(), nullptr, nullptr, m_data.entrypoint.c_str(), m_data.shader_type.c_str(),
			m_data.iFlags1,
			m_data.iFlags2,
			&m_data.pBlob,
			&m_data.pErrorBlob);

		return SUCCEEDED(hr) ? 0 : 1;
	}

	ShaderSystemD3D11::ShaderTraits& ShaderSystemD3D11::get() { return m_data; }

	void ShaderSystemD3D11::update(RenderComponentD3D11* component)
	{
		if (!component)
			return;

		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Details::CBUFFER* dataPtr;
		unsigned int bufferNumber = 0U;

		auto viewMatrix = component->m_viewMatrix;

		component->m_pDriver->get().WorldMatrix = XMMatrixTranspose(component->m_pDriver->get().WorldMatrix);
		viewMatrix = XMMatrixTranspose(viewMatrix);
		component->m_pDriver->get().ProjectionMatrix = XMMatrixTranspose(component->m_pDriver->get().ProjectionMatrix);

		result = component->m_pDriver->get().pCtx->Map(component->m_pMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		if (FAILED(result))
			return;

		dataPtr = (Details::CBUFFER*)mappedResource.pData;

		dataPtr->world = component->m_pDriver->get().WorldMatrix;
		dataPtr->view = viewMatrix;
		dataPtr->projection = component->m_pDriver->get().ProjectionMatrix;

		component->m_pDriver->get().pCtx->Unmap(component->m_pMatrixBuffer.Get(), 0);

		component->m_pDriver->get().pCtx->VSSetConstantBuffers(bufferNumber, 1, component->m_pMatrixBuffer.GetAddressOf());
	
		if (this->m_data.pVertex)
			component->m_pDriver->get().pCtx->VSSetShader(this->m_data.pVertex.Get(), nullptr, 0);

		if (this->m_data.pPixel)
			component->m_pDriver->get().pCtx->PSSetShader(this->m_data.pPixel.Get(), nullptr, 0);

		if (this->m_data.pHull)
			component->m_pDriver->get().pCtx->HSSetShader(this->m_data.pHull.Get(), nullptr, 0);

	}

	HRESULT ShaderSystemD3D11::ShaderTraits::create_input_layout(ID3D11Device* device)
	{
		HRESULT hr = S_OK;

		if (!device)
			return hr;

		hr = device->CreateInputLayout(vInputLayouts.data(), vInputLayouts.size(), pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(), pInputLayout.GetAddressOf());

		XPLICIT_ASSERT(SUCCEEDED(hr));
		
		pBlob->Release();

		return hr;
	}
}

#endif