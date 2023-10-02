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

		if (m_data.pInputLayout)
			m_data.pInputLayout->Release();
	}

	int ShaderSystemD3D11::compile() noexcept
	{
		HRESULT hr = D3DCompileFromFile(m_shader.c_str(), nullptr, nullptr, m_data.entrypoint.c_str(), m_data.shader_type.c_str(),
			m_data.iFlags1,
			m_data.iFlags2,
			&m_data.pBlob,
			&m_data.pErrorBlob);

		if (FAILED(hr))
		{
			print_error(m_data.pErrorBlob);
			throw Win32Error("Could not compile shader from file!");
		}

		if (m_data.shader_type == XPLICIT_VERTEX_SHADER)
		{
			hr = m_pDriver->get().pDevice->CreateVertexShader(m_data.pBlob->GetBufferPointer(),
				m_data.pBlob->GetBufferSize(), nullptr, m_data.pVertex.GetAddressOf());

			Details::ThrowIfFailed(hr);
		}
		else if (m_data.shader_type == XPLICIT_PIXEL_SHADER)
		{
			hr = m_pDriver->get().pDevice->CreatePixelShader(m_data.pBlob->GetBufferPointer(),
				m_data.pBlob->GetBufferSize(), nullptr, m_data.pPixel.GetAddressOf());

			Details::ThrowIfFailed(hr);
		}

		return SUCCEEDED(hr) ? 0 : 1;
	}

	ShaderSystemD3D11::ShaderTraits& ShaderSystemD3D11::get() { return m_data; }

	void ShaderSystemD3D11::update_cbuf(RenderableComponentD3D11* component)
	{
		if (!component)
			return;

		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource{};

		Details::CBUFFER* cBuffer = nullptr;

		unsigned int cBufferCnt = 0U;
		
		XMMATRIX rotationMatrix{};

		XMMATRIX transltateMatrix = XMMatrixTranslation(component->m_vPosition.X,
			component->m_vPosition.Y,
			component->m_vPosition.Z);

		XMMATRIX scaleMatrix = XMMatrixScaling(component->m_vScale.X,
			component->m_vScale.Y, component->m_vScale.Z);

		if (component->m_vRotation.Z > 0)
		{
			rotationMatrix += XMMatrixRotationZ(component->m_vRotation.Z);
			component->m_vRotation.Z = 0;
		}

		if (component->m_vRotation.X > 0)
		{
			rotationMatrix += XMMatrixRotationX(component->m_vRotation.X);
			component->m_vRotation.X = 0;
		}

		if (component->m_vRotation.Y > 0)
		{
			rotationMatrix += XMMatrixRotationY(component->m_vRotation.Y);
			component->m_vRotation.Y = 0;
		}

		XMMATRIX scaleRotateMatrix = XMMatrixMultiply(scaleMatrix,
			rotationMatrix);

		component->m_pDriver->get().WorldMatrix = XMMatrixMultiply(component->m_pDriver->get().WorldMatrix, scaleRotateMatrix);

		auto transPoseWorldMatrix = XMMatrixTranspose(component->m_pDriver->get().WorldMatrix);
	
		auto transPoseViewMatrix = XMMatrixTranspose(component->m_pDriver->get().pCamera->m_viewMatrix);
		auto transPoseProjectionMatrix = XMMatrixTranspose(component->m_pDriver->get().ProjectionMatrix);

		HRESULT hr = component->m_pDriver->get().pContext->Map(component->m_pMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		Details::ThrowIfFailed(hr);

		cBuffer = (Details::CBUFFER*)mappedResource.pData;

		cBufferCnt = (sizeof(Details::CBUFFER) / sizeof(cBuffer[0]));

		for (size_t bufferIndex = 0; bufferIndex < cBufferCnt; ++bufferIndex)
		{
			cBuffer[bufferIndex].PROJECTION = transPoseProjectionMatrix;
			cBuffer[bufferIndex].WORLD = transPoseWorldMatrix;
			cBuffer[bufferIndex].VIEW = transPoseViewMatrix;
		}

		cBufferCnt = 0U;

		component->m_pDriver->get().pContext->Unmap(component->m_pMatrixBuffer.Get(), 0);
		component->m_pDriver->get().pContext->VSSetConstantBuffers(cBufferCnt, 1, component->m_pMatrixBuffer.GetAddressOf());
	}

	void ShaderSystemD3D11::update(RenderableComponentD3D11* component)
	{
		if (!component)
			return;

		if (m_data.pInputLayout)
			m_pDriver->get().pContext->IASetInputLayout(m_data.pInputLayout);

		switch ((XPLICIT_SHADER_TYPE)m_type)
		{
		case XPLICIT_SHADER_TYPE::Vertex:
		{
			component->m_pDriver->get().pContext->VSSetShader(this->m_data.pVertex.Get(), nullptr, 0u);
			break;
		}
		case XPLICIT_SHADER_TYPE::Pixel:
		{
			component->m_pDriver->get().pContext->PSSetShader(this->m_data.pPixel.Get(), nullptr, 0u);
			break;
		}
		case XPLICIT_SHADER_TYPE::Hull:
		{
			component->m_pDriver->get().pContext->HSSetShader(this->m_data.pHull.Get(), nullptr, 0u);
			break;
		}
		}
	}
}

#endif