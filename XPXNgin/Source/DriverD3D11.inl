/*
 * =====================================================================
 *
 *			XPXNgin (RigMan)
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: DriverD3D11.h
 *			Purpose: C++ Rendering Driver for Direct3D 11
 *
 * =====================================================================
 */

#ifdef XPLICIT_WINDOWS

namespace XPX
{
	static inline void print_error(ID3D10Blob* error)
	{
		if (!error)
			return;

		std::string err;
		err += "[ShaderSystem] ";

		for (size_t i = 0; i < error->GetBufferSize(); i++)
		{
			err += ((char*)(error->GetBufferPointer()))[i];
		}

		XPLICIT_CRITICAL(err);

		OutputDebugStringA(err.c_str());

#ifdef XPLICIT_DEBUG
		XPLICIT_ASSERT(false);
#endif
	}

	static inline bool create_vertex_shader(Renderer::DX11::DriverSystemD3D11* driver,
		Renderer::DX11::ShaderSystemD3D11* shader_system)
	{
		shader_system->get().shader_type += XPLICIT_VERTEX_SHADER;

		if (shader_system->compile())
		{
			print_error(shader_system->get().pErrorBlob);
			delete shader_system;

			return false;
		}

		return true;
	}

	static inline bool create_pixel_shader(Renderer::DX11::DriverSystemD3D11* driver,
		Renderer::DX11::ShaderSystemD3D11* shader_system)
	{
		shader_system->get().shader_type += XPLICIT_PIXEL_SHADER;

		if (shader_system->compile())
		{
			print_error(shader_system->get().pErrorBlob);
			delete shader_system;

			return false;
		}

		return true;
	}

	static inline bool create_hull_shader(Renderer::DX11::DriverSystemD3D11* driver,
		Renderer::DX11::ShaderSystemD3D11* shader_system)
	{
		shader_system->get().shader_type += XPLICIT_HULL_SHADER;

		if (shader_system->compile())
		{
			print_error(shader_system->get().pErrorBlob);
			delete shader_system;

			return false;
		}

		return true;
	}
}

/// <summary>
/// Direct3D Shader creation helper.
/// </summary>
/// <typeparam name="ShaderType">Either Pixel, Vertex or Hull</typeparam>
/// <param name="filename">The shader to open</param>
/// <param name="entrypoint">The entrypoint to look at</param>
/// <param name="driver">The rendering driver</param>
/// <returns>Pointer to a new ShaderSystem</returns>
/// 
template <XPX::Renderer::DX11::XPLICIT_SHADER_TYPE ShaderType>
XPX::Renderer::DX11::ShaderSystemD3D11* XPX::Renderer::DX11::D3D11ShaderHelper1::make_shader(
	const PChar* filename,
	const char* entrypoint,
	XPX::Renderer::DX11::DriverSystemD3D11* driver
)
{
	if (!entrypoint ||
		*entrypoint == 0)
		return nullptr;

	XPX::Renderer::DX11::ShaderSystemD3D11* shader_system = new XPX::Renderer::DX11::ShaderSystemD3D11(filename, ShaderType, driver);

	if (shader_system)
	{
		shader_system->get().iFlags1 |= D3D10_SHADER_ENABLE_STRICTNESS;
		shader_system->get().entrypoint += entrypoint;

		switch (ShaderType)
		{
		case XPLICIT_SHADER_TYPE::Vertex:
		{
			if (create_vertex_shader(driver, shader_system))
				return shader_system;

			break;
		}
		case XPLICIT_SHADER_TYPE::Pixel:
		{
			if (create_pixel_shader(driver, shader_system))
				return shader_system;

			break;
		}
		case XPLICIT_SHADER_TYPE::Hull:
		{
			if (create_hull_shader(driver, shader_system))
				return shader_system;

			break;
		}
		}

		delete shader_system;
	}

	return nullptr;
}

#undef XPLICIT_DX11_STRING_SZ

#endif