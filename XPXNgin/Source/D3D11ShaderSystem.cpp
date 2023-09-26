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

		if (m_data.pPixel)
			m_data.pPixel->Release();

		if (m_data.pVertex)
			m_data.pVertex->Release();
	}

	int ShaderSystemD3D11::compile() noexcept
	{
		HRESULT hr = D3DCompileFromFile(m_shader.c_str(), nullptr, nullptr, m_data.entrypoint.c_str(), m_data.shader_type.c_str(),
			m_data.iFlags1,
			m_data.iFlags2,
			&m_data.pBlob,
			&m_data.pErrorBlob);

		if (m_data.shader_type == XPLICIT_VERTEX_SHADER)
		{
			m_pDriver->get().pDevice->CreateVertexShader(m_data.pBlob->GetBufferPointer(),
				m_data.pBlob->GetBufferSize(), nullptr, m_data.pVertex.GetAddressOf());
		}
		else if (m_data.shader_type == XPLICIT_PIXEL_SHADER)
		{

			m_pDriver->get().pDevice->CreatePixelShader(m_data.pBlob->GetBufferPointer(),
				m_data.pBlob->GetBufferSize(), nullptr, m_data.pPixel.GetAddressOf());
		}

		return SUCCEEDED(hr) ? 0 : 1;
	}

	ShaderSystemD3D11::ShaderTraits& ShaderSystemD3D11::get() { return m_data; }

	void ShaderSystemD3D11::update_cbuf(RenderComponentD3D11* component)
	{
		if (!component)
			return;

		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Details::CBUFFER* cBuffer;
		unsigned int cBufferCnt = 0U;

		auto transPoseWorldMatrix = XMMatrixTranspose(component->m_pDriver->get().WorldMatrix);
		auto transPoseOrthoMatrix = XMMatrixTranspose(component->m_pDriver->get().OrthoMatrix);
		auto transPoseProjectionMatrix = XMMatrixTranspose(component->m_pDriver->get().ProjectionMatrix);

		HRESULT hr = component->m_pDriver->get().pCtx->Map(component->m_pMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		Details::ThrowIfFailed(hr);

		cBuffer = (Details::CBUFFER*)mappedResource.pData;

		cBufferCnt = sizeof(cBuffer) / sizeof(cBuffer[0]);

		for (size_t bufferIndex = 0; bufferIndex < cBufferCnt; bufferIndex++)
		{
			cBuffer[bufferIndex].projection = transPoseProjectionMatrix;
			cBuffer[bufferIndex].world = transPoseWorldMatrix;
			cBuffer[bufferIndex].view = transPoseOrthoMatrix;
		}

		component->m_pDriver->get().pCtx->Unmap(component->m_pMatrixBuffer.Get(), 0);
		component->m_pDriver->get().pCtx->VSSetConstantBuffers(cBufferCnt, 1, component->m_pMatrixBuffer.GetAddressOf());
	}

	void ShaderSystemD3D11::update(RenderComponentD3D11* component)
	{
		if (!component)
			return;

		try
		{
			this->update_cbuf(component);
		}
		catch (...)
		{
			XPLICIT_INFO("WARNING: No CBuf attached to shader.");
		}

		switch ((XPLICIT_SHADER_TYPE)m_type)
		{
		case XPLICIT_SHADER_TYPE::Vertex:
		{
			component->m_pDriver->get().pCtx->VSSetShader(this->m_data.pVertex.Get(), nullptr, 0u);
			break;
		}
		case XPLICIT_SHADER_TYPE::Pixel:
		{
			component->m_pDriver->get().pCtx->PSSetShader(this->m_data.pPixel.Get(), nullptr, 0u);
			break;
		}
		case XPLICIT_SHADER_TYPE::Hull:
		{
			component->m_pDriver->get().pCtx->HSSetShader(this->m_data.pHull.Get(), nullptr, 0u);
			break;
		}
		}
	}
}

#endif