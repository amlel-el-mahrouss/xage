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
#include <d3dcompiler.h>

#ifdef XPLICIT_WINDOWS

namespace Xplicit::Renderer::DX11
{
	int D3D11ShaderSystem::operator()()
	{
		if (!m_data)
			return 1;

		HRESULT hr = D3DCompileFromFile(m_shader.c_str(), nullptr, nullptr, m_data->entrypoint, m_data->shader_type,
			m_data->flags1, 
			m_data->flags2, 
			m_data->blobs, m_data->error_blobs);

		return SUCCEEDED(hr) ? 0 : -1;
	}

	D3D11ShaderSystem::ShaderData* D3D11ShaderSystem::get() const { return m_data.get(); }
}

#endif