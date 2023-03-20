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
		err += "[SHADER]";

		for (size_t i = 0; i < error->GetBufferSize(); i++)
		{
			err += ((char*)(error->GetBufferPointer()))[i];
		}

		XPLICIT_CRITICAL(err);

#ifdef XPLICIT_DEBUG
		XPLICIT_ASSERT(false);
#endif
	}
}

template <Xplicit::Renderer::DX11::D3D11_SHADER_TYPE ShaderType>
Xplicit::Renderer::DX11::D3D11ShaderSystem* Xplicit::Renderer::DX11::D3D11ShaderHelper1::make_shader(
	const pchar* filename,
	const pchar entrypoint[XPLICIT_DX11_STRING_SZ],
	Xplicit::Renderer::DX11::DriverSystemD3D11* driver
)
{
	if (!entrypoint ||
		*entrypoint == 0)
		return nullptr;

	Xplicit::Renderer::DX11::D3D11ShaderSystem* shader_system = new Xplicit::Renderer::DX11::D3D11ShaderSystem(filename);

	if (shader_system)
	{
		shader_system->get()->flags1 |= D3D10_SHADER_ENABLE_STRICTNESS;
		memcpy(shader_system->get()->entrypoint, entrypoint, XPLICIT_DX11_STRING_SZ);

		switch (ShaderType)
		{
		case D3D11_SHADER_TYPE::Vertex:
		{
			memcpy(shader_system->get()->shader_type, XPLICIT_VERTEX_SHADER, XPLICIT_DX11_STRING_SZ);

			if ((*shader_system)())
			{
				print_error(shader_system->get()->error_blob);
				delete shader_system;

				return nullptr;
			}

			driver->get().Device->CreateVertexShader(shader_system->get()->blob->GetBufferPointer(),
				shader_system->get()->blob->GetBufferSize(),
				nullptr,
				shader_system->get()->vertex);

			break;
		}
		case D3D11_SHADER_TYPE::Pixel:
		{
			memcpy(shader_system->get()->shader_type, XPLICIT_PIXEL_SHADER, XPLICIT_DX11_STRING_SZ);

			if ((*shader_system)())
			{
				print_error(shader_system->get()->error_blob);
				delete shader_system;

				return nullptr;
			}

			driver->get().Device->CreatePixelShader(shader_system->get()->blob->GetBufferPointer(),
				shader_system->get()->blob->GetBufferSize(),
				nullptr,
				shader_system->get()->pixel);

			break;
		}
		case D3D11_SHADER_TYPE::Hull:
		{
			memcpy(shader_system->get()->shader_type, XPLICIT_HULL_SHADER, XPLICIT_DX11_STRING_SZ);

			if ((*shader_system)())
			{
				print_error(shader_system->get()->error_blob);
				delete shader_system;

				return nullptr;
			}

			driver->get().Device->CreateHullShader(shader_system->get()->blob->GetBufferPointer(),
				shader_system->get()->blob->GetBufferSize(),
				nullptr,
				shader_system->get()->hull);

			break;
		}
		}

		return shader_system;
	}

	return nullptr;
}

#undef XPLICIT_DX11_STRING_SZ