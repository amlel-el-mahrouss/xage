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

	void ShaderSystemD3D11::update_render_shader(RenderableComponentD3D11* component)
	{
		if (!component)
			return;

		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource{};

		Details::CBUFFER* cBuffer = nullptr;

		unsigned int cBufferCnt = 0U;
		
		XMMATRIX transltateMatrix = XMMatrixTranslation(component->m_vPosition.X,
			component->m_vPosition.Y,
			component->m_vPosition.Z);

		XMMATRIX rotationMatrix = XMMatrixRotationY(component->m_vRotation.Y);

		rotationMatrix *= XMMatrixRotationX(component->m_vRotation.X);

		rotationMatrix *= XMMatrixRotationZ(component->m_vRotation.Z);

		component->m_pDriver->get().WorldMatrix = XMMatrixMultiply(rotationMatrix, transltateMatrix);

		auto transPoseWorldMatrix = XMMatrixTranspose(component->m_pDriver->get().WorldMatrix);
		auto transPoseViewMatrix = XMMatrixTranspose(component->m_pDriver->get().pCamera->m_viewMatrix);
		auto transPoseProjectionMatrix = XMMatrixTranspose(component->m_pDriver->get().ProjectionMatrix);

		HRESULT hr = component->m_pDriver->get().pContext->Map(component->m_pMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		Details::ThrowIfFailed(hr);

		cBuffer = (Details::CBUFFER*)mappedResource.pData;

		cBufferCnt = (sizeof(Details::CBUFFER) / sizeof(cBuffer[0]));

		cBuffer->PROJECTION = transPoseProjectionMatrix;
		cBuffer->WORLD = transPoseWorldMatrix;
		cBuffer->VIEW = transPoseViewMatrix;

		cBufferCnt = 0U;

		component->m_pDriver->get().pContext->Unmap(component->m_pMatrixBuffer.Get(), 0);
		component->m_pDriver->get().pContext->VSSetConstantBuffers(cBufferCnt, 1, component->m_pMatrixBuffer.GetAddressOf());
	}

	void ShaderSystemD3D11::update_light_shader(LightSystemD3D11* component)
	{
		if (!component)
			return;

		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource{};

		Details::CBUFFER* cBuffer = nullptr;

		unsigned int cBufferCnt = 0U;

		auto transPoseWorldMatrix = XMMatrixTranspose(RENDERER->get().WorldMatrix);
		auto transPoseViewMatrix = XMMatrixTranspose(RENDERER->get().pCamera->m_viewMatrix);
		auto transPoseProjectionMatrix = XMMatrixTranspose(RENDERER->get().ProjectionMatrix);

		HRESULT hr = RENDERER->get().pContext->Map(component->m_pMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		Details::ThrowIfFailed(hr);

		cBuffer = (Details::CBUFFER*)mappedResource.pData;

		cBufferCnt = (sizeof(Details::CBUFFER) / sizeof(cBuffer[0]));

		cBuffer->PROJECTION = transPoseProjectionMatrix;
		cBuffer->WORLD = transPoseWorldMatrix;
		cBuffer->VIEW = transPoseViewMatrix;

		cBufferCnt = 0U;

		RENDERER->get().pContext->Unmap(component->m_pMatrixBuffer.Get(), 0);
		RENDERER->get().pContext->VSSetConstantBuffers(cBufferCnt, 1, component->m_pMatrixBuffer.GetAddressOf());

		hr = RENDERER->get().pContext->Map(component->m_pLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		Details::ThrowIfFailed(hr);

		Details::LIGHT* light = (Details::LIGHT*)mappedResource.pData;

		light->COLOR.x = component->f_cColour.R;
		light->COLOR.y = component->f_cColour.G;
		light->COLOR.z = component->f_cColour.B;

		light->DIRECTION.x = component->f_vDirection.X;
		light->DIRECTION.y = component->f_vDirection.Y;
		light->DIRECTION.z = component->f_vDirection.Z;

		light->PADDING = 0.0f;

		cBufferCnt = 0U;

		RENDERER->get().pContext->Unmap(component->m_pLightBuffer.Get(), 0);
		RENDERER->get().pContext->PSSetConstantBuffers(cBufferCnt, 1, component->m_pLightBuffer.GetAddressOf());
	}

	void ShaderSystemD3D11::update(RenderableComponentD3D11* component)
	{
		if (!component)
			return;

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

	void ShaderSystemD3D11::update(LightSystemD3D11* component)
	{
		if (!component)
			return;

		switch ((XPLICIT_SHADER_TYPE)m_type)
		{
		case XPLICIT_SHADER_TYPE::Vertex:
		{
			RENDERER->get().pContext->VSSetShader(this->m_data.pVertex.Get(), nullptr, 0u);
			break;
		}
		case XPLICIT_SHADER_TYPE::Pixel:
		{
			RENDERER->get().pContext->PSSetShader(this->m_data.pPixel.Get(), nullptr, 0u);
			break;
		}
		case XPLICIT_SHADER_TYPE::Hull:
		{
			RENDERER->get().pContext->HSSetShader(this->m_data.pHull.Get(), nullptr, 0u);
			break;
		}
		}
	}
}

#endif