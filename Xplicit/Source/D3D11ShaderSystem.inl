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

namespace Xplicit::Renderer::DX11
{
	template <typename StructSz>
	HRESULT ShaderSystemD3D11::ShaderTraits::create_matrix_buffer(ID3D11Device* device)
	{
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(StructSz);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&matrixBufferDesc, nullptr, this->pMatrixBuffer.GetAddressOf());

		if (FAILED(hr))
			throw EngineError();

		return hr;
	}
}