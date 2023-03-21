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

// Stupid workaround...
#include <../um/d3dcompiler.h>

#ifdef XPLICIT_WINDOWS

namespace Xplicit::Renderer::DX11
{
	D3D11ShaderSystem::~D3D11ShaderSystem()
	{
		if (m_data->blob)
			m_data->blob->Release();

		if (m_data->error_blob)
			m_data->error_blob->Release();
	}

	int D3D11ShaderSystem::compile() noexcept
	{
		if (!m_data)
			return 1;

		HRESULT hr = D3DReadFileToBlob(m_shader.c_str(), &m_data->blob);

		if (FAILED(hr))
		{
			hr = D3DCompileFromFile(m_shader.c_str(), nullptr, nullptr, m_data->entrypoint.c_str(), m_data->shader_type.c_str(),
				m_data->flags1,
				m_data->flags2,
				&m_data->blob,
				&m_data->error_blob);

			return SUCCEEDED(hr) ? 0 : -1;
		}

		return 0;
	}

	D3D11ShaderSystem::ShaderData* D3D11ShaderSystem::get() const { return m_data.get(); }

	void D3D11ShaderSystem::make(D3D11RenderComponent* component)
	{
		if (!component)
			return;

		component->m_driver->get().Ctx->IASetInputLayout(this->m_data->input_layout_ptr.Get());

		component->m_driver->get().Ctx->VSSetShader(this->get()->vertex.Get(), nullptr, 0);
		component->m_driver->get().Ctx->PSSetShader(this->get()->pixel.Get(), nullptr, 0);
	}
}

#endif