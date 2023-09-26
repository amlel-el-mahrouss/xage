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

		
		if (m_data.shader_type == XPLICIT_VERTEX_SHADER)
		{
			ID3D11VertexShader* pShader = nullptr;

			m_pDriver->get().pDevice->CreateVertexShader(m_data.pBlob->GetBufferPointer(),
				m_data.pBlob->GetBufferSize(), nullptr, &pShader);

			m_pDriver->get().pCtx->VSSetShader(pShader, nullptr, 0);

			m_type = (uint8_t)XPLICIT_SHADER_TYPE::Vertex;

			pShader->Release();
		}
		else if (m_data.shader_type == XPLICIT_PIXEL_SHADER)
		{
			ID3D11PixelShader* pShader;

			m_pDriver->get().pDevice->CreatePixelShader(m_data.pBlob->GetBufferPointer(),
				m_data.pBlob->GetBufferSize(), nullptr, &pShader);

			m_pDriver->get().pCtx->PSSetShader(pShader, nullptr, 0);

			m_type = (uint8_t)XPLICIT_SHADER_TYPE::Pixel;

			pShader->Release();
		}

		return SUCCEEDED(hr) ? 0 : 1;
	}

	ShaderSystemD3D11::ShaderTraits& ShaderSystemD3D11::get() { return m_data; }

	void ShaderSystemD3D11::update(RenderComponentD3D11* component)
	{
		if (!component)
			return;

		switch ((XPLICIT_SHADER_TYPE)m_type)
		{
		case XPLICIT_SHADER_TYPE::Vertex:
		{
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			Details::CBUFFER* dataPtr;
			unsigned int bufferNumber = 0U;

			auto viewMatrix = component->m_viewMatrix;

			component->m_pDriver->get().WorldMatrix = XMMatrixTranspose(component->m_pDriver->get().WorldMatrix);
			viewMatrix = XMMatrixTranspose(viewMatrix);
			component->m_pDriver->get().ProjectionMatrix = XMMatrixTranspose(component->m_pDriver->get().ProjectionMatrix);

			result = component->m_pDriver->get().pCtx->Map(component->m_pMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			XPLICIT_ASSERT(SUCCEEDED(result));

			dataPtr = (Details::CBUFFER*)mappedResource.pData;

			dataPtr->world = component->m_pDriver->get().WorldMatrix;
			dataPtr->view = viewMatrix;
			dataPtr->projection = component->m_pDriver->get().ProjectionMatrix;

			component->m_pDriver->get().pCtx->Unmap(component->m_pMatrixBuffer.Get(), 0);

			component->m_pDriver->get().pCtx->VSSetConstantBuffers(bufferNumber, 1, component->m_pMatrixBuffer.GetAddressOf());

			component->m_pDriver->get().pCtx->Unmap(component->m_pMatrixBuffer.Get(), 0);

			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

			D3D11_BUFFER_DESC desc;
			component->m_pVertexBuffer->GetDesc(&desc);

			desc.Usage = D3D11_USAGE_STAGING;
			desc.BindFlags = 0;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

			ID3D11Buffer* pBuffer = nullptr;

			component->m_pDriver->get().pDevice->CreateBuffer(&desc, nullptr, &pBuffer);
			component->m_pDriver->get().pCtx->CopyResource(pBuffer, component->m_pVertexBuffer.Get());

			result = component->m_pDriver->get().pCtx->Map(pBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);

			Details::VERTEX** vert = (Details::VERTEX**)&mappedResource.pData;

			for (size_t i = 0; i < (sizeof(vert) / sizeof(*vert)); ++i)
			{
				vert[i]->COLOR = XMVectorSet(component->m_colorVectors[i].A, component->m_colorVectors[i].R, component->m_colorVectors[i].G, component->m_colorVectors[i].B);

				vert[i]->X = component->m_arrayVerts[i].X;
				vert[i]->Y = component->m_arrayVerts[i].Y;
				vert[i]->Z = component->m_arrayVerts[i].Z;
			}

			pBuffer->Release();

			break;
		}
		case XPLICIT_SHADER_TYPE::Pixel:
		{
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			Details::CBUFFER* dataPtr;
			unsigned int bufferNumber = 0U;

			auto viewMatrix = component->m_viewMatrix;

			component->m_pDriver->get().WorldMatrix = XMMatrixTranspose(component->m_pDriver->get().WorldMatrix);
			viewMatrix = XMMatrixTranspose(viewMatrix);
			component->m_pDriver->get().ProjectionMatrix = XMMatrixTranspose(component->m_pDriver->get().ProjectionMatrix);

			result = component->m_pDriver->get().pCtx->Map(component->m_pMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			XPLICIT_ASSERT(SUCCEEDED(result));

			dataPtr = (Details::CBUFFER*)mappedResource.pData;

			dataPtr->world = component->m_pDriver->get().WorldMatrix;
			dataPtr->view = viewMatrix;
			dataPtr->projection = component->m_pDriver->get().ProjectionMatrix;

			component->m_pDriver->get().pCtx->Unmap(component->m_pMatrixBuffer.Get(), 0);

			component->m_pDriver->get().pCtx->PSSetConstantBuffers(bufferNumber, 1, component->m_pMatrixBuffer.GetAddressOf());

			component->m_pDriver->get().pCtx->Unmap(component->m_pMatrixBuffer.Get(), 0);

			break;
		}
		}
	}
}

#endif