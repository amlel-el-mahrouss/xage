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
	int D3D11ShaderSystem::compile() noexcept
	{
		if (!m_data)
			return 1;

		HRESULT hr = D3DCompileFromFile(m_shader.c_str(), nullptr, nullptr, m_data->entrypoint.c_str(), m_data->shader_type.c_str(),
			m_data->flags1, 
			m_data->flags2, 
			&m_data->blob, &m_data->error_blob);

		return SUCCEEDED(hr) ? 0 : -1;
	}

	D3D11ShaderSystem::ShaderData* D3D11ShaderSystem::get() const { return m_data.get(); }

	void D3D11ShaderSystem::update(D3D11RenderComponent* component)
	{
		if (!component)
			return;

		if (this->m_data->vertex)
		{
			component->m_driver->get().Ctx->VSSetShader(this->m_data->vertex.Get(), nullptr, 0);
			return;
		}
		else if (this->m_data->pixel)
		{
			component->m_driver->get().Ctx->PSSetShader(this->m_data->pixel.Get(), nullptr, 0);
			return;
		}
		else if (this->m_data->hull)
		{
			component->m_driver->get().Ctx->HSSetShader(this->m_data->hull.Get(), nullptr, 0);
			return;
		}
	}
}

#endif