/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright XPX, all rights reserved.
 *
 *			File: DriverD3D11.h
 *			Purpose: C++ Rendering Driver for Direct3D 11
 *
 * =====================================================================
 */

namespace Xplicit
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
		Renderer::DX11::D3D11ShaderSystem* shader_system)
	{
		shader_system->get().shader_type += XPLICIT_VERTEX_SHADER;

		if (shader_system->compile())
		{
			print_error(shader_system->get().error_blob);
			delete shader_system;

			return false;
		}

		if (FAILED(driver->get().Device->CreateVertexShader(shader_system->get().blob->GetBufferPointer(),
			shader_system->get().blob->GetBufferSize(),
			nullptr,
			shader_system->get().vertex.GetAddressOf())))
		{
			delete shader_system;

			return false;
		}

		shader_system->get().matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		shader_system->get().matrix_buffer_desc.ByteWidth = sizeof(Renderer::DX11::Details::VERTEX);
		shader_system->get().matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		shader_system->get().matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		shader_system->get().matrix_buffer_desc.MiscFlags = 0;
		shader_system->get().matrix_buffer_desc.StructureByteStride = 0;

		if (FAILED(driver->get().Device->CreateBuffer(&shader_system->get().matrix_buffer_desc,
			nullptr,
			shader_system->get().matrix_buffer_ptr.GetAddressOf())))
		{
			delete shader_system;
			return false;
		}

		return true;
	}

	static inline bool create_pixel_shader(Renderer::DX11::DriverSystemD3D11* driver,
		Renderer::DX11::D3D11ShaderSystem* shader_system)
	{
		shader_system->get().shader_type += XPLICIT_PIXEL_SHADER;

		if (shader_system->compile())
		{
			print_error(shader_system->get().error_blob);
			delete shader_system;

			return false;
		}

		if (FAILED(driver->get().Device->CreatePixelShader(shader_system->get().blob->GetBufferPointer(),
			shader_system->get().blob->GetBufferSize(),
			nullptr,
			shader_system->get().pixel.GetAddressOf())))
		{
			delete shader_system;
			return false;
		}

		return true;
	}

	static inline bool create_hull_shader(Renderer::DX11::DriverSystemD3D11* driver,
		Renderer::DX11::D3D11ShaderSystem* shader_system)
	{
		shader_system->get().shader_type += XPLICIT_HULL_SHADER;

		if (shader_system->compile())
		{
			print_error(shader_system->get().error_blob);
			delete shader_system;

			return false;
		}

		if (FAILED(driver->get().Device->CreateHullShader(shader_system->get().blob->GetBufferPointer(),
			shader_system->get().blob->GetBufferSize(),
			nullptr,
			shader_system->get().hull.GetAddressOf())))
		{
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
template <Xplicit::Renderer::DX11::XPLICIT_SHADER_TYPE ShaderType>
Xplicit::Renderer::DX11::D3D11ShaderSystem* Xplicit::Renderer::DX11::D3D11ShaderHelper1::make_shader(
	const PChar* filename,
	const char* entrypoint,
	std::unique_ptr<Xplicit::Renderer::DX11::DriverSystemD3D11>& driver
)
{
	if (!entrypoint ||
		*entrypoint == 0)
		return nullptr;

	Xplicit::Renderer::DX11::D3D11ShaderSystem* shader_system = new Xplicit::Renderer::DX11::D3D11ShaderSystem(filename);

	if (shader_system)
	{
		shader_system->get().flags1 |= D3D10_SHADER_ENABLE_STRICTNESS;
		shader_system->get().entrypoint += entrypoint;

		switch (ShaderType)
		{
		case XPLICIT_SHADER_TYPE::Vertex:
		{
			if (create_vertex_shader(driver.get(), shader_system))
				return shader_system;

			break;
		}
		case XPLICIT_SHADER_TYPE::Pixel:
		{
			if (create_pixel_shader(driver.get(), shader_system))
				return shader_system;

			break;
		}
		case XPLICIT_SHADER_TYPE::Hull:
		{
			if (create_hull_shader(driver.get(), shader_system))
				return shader_system;

			break;
		}
		}

		return shader_system;
	}

	return nullptr;
}

#undef XPLICIT_DX11_STRING_SZ