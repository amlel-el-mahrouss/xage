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

#include <D3DX10math.h>

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

#ifdef XPLICIT_DEBUG
		XPLICIT_ASSERT(false);
#endif
	}
}

/// <summary>
/// Direct3D Shader creation helper.
/// </summary>
/// <typeparam name="ShaderType">Either Pixel, Vertex or Hull.</typeparam>
/// <param name="filename">The file to open.</param>
/// <param name="entrypoint">The entrypoint to look at.</param>
/// <param name="driver">The rendering driver.</param>
/// <returns>Pointer to a new ShaderSystem.</returns>
/// 
template <Xplicit::Renderer::DX11::D3D11_SHADER_TYPE ShaderType>
Xplicit::Renderer::DX11::D3D11ShaderSystem* Xplicit::Renderer::DX11::D3D11ShaderHelper1::make_shader(
	const pchar* filename,
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
		shader_system->get()->flags1 |= D3D10_SHADER_ENABLE_STRICTNESS;
		shader_system->get()->entrypoint += entrypoint;

		switch (ShaderType)
		{
		case D3D11_SHADER_TYPE::Vertex:
		{
			shader_system->get()->shader_type  += XPLICIT_VERTEX_SHADER;
			if (shader_system->compile() != 0)
			{
				print_error(shader_system->get()->error_blob);
				delete shader_system;

				return nullptr;
			}

			driver->get().Device->CreateVertexShader(shader_system->get()->blob->GetBufferPointer(),
				shader_system->get()->blob->GetBufferSize(),
				nullptr,
				shader_system->get()->vertex.GetAddressOf());

			shader_system->get()->matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
			shader_system->get()->matrix_buffer_desc.ByteWidth = sizeof(D3DXMATRIX);
			shader_system->get()->matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			shader_system->get()->matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			shader_system->get()->matrix_buffer_desc.MiscFlags = 0;
			shader_system->get()->matrix_buffer_desc.StructureByteStride = 0;

			driver->get().Device->CreateBuffer(&shader_system->get()->matrix_buffer_desc, 
				nullptr, 
				shader_system->get()->matrix_buffer_ptr.GetAddressOf());

			break;
		}
		case D3D11_SHADER_TYPE::Pixel:
		{
			shader_system->get()->shader_type += XPLICIT_PIXEL_SHADER;

			if (shader_system->compile() != 0)
			{
				print_error(shader_system->get()->error_blob);
				delete shader_system;

				return nullptr;
			}

			driver->get().Device->CreatePixelShader(shader_system->get()->blob->GetBufferPointer(),
				shader_system->get()->blob->GetBufferSize(),
				nullptr,
				shader_system->get()->pixel.GetAddressOf());

			shader_system->get()->polygon_layout.push_back(D3D11_INPUT_ELEMENT_DESC());
			shader_system->get()->polygon_layout.push_back(D3D11_INPUT_ELEMENT_DESC());

			shader_system->get()->polygon_layout[0].SemanticName = "POSITION";
			shader_system->get()->polygon_layout[0].SemanticIndex = 0;
			shader_system->get()->polygon_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			shader_system->get()->polygon_layout[0].InputSlot = 0;
			shader_system->get()->polygon_layout[0].AlignedByteOffset = 0;
			shader_system->get()->polygon_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			shader_system->get()->polygon_layout[0].InstanceDataStepRate = 0;

			shader_system->get()->polygon_layout[1].SemanticName = "COLOR";
			shader_system->get()->polygon_layout[1].SemanticIndex = 0;
			shader_system->get()->polygon_layout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			shader_system->get()->polygon_layout[1].InputSlot = 0;
			shader_system->get()->polygon_layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			shader_system->get()->polygon_layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			shader_system->get()->polygon_layout[1].InstanceDataStepRate = 0;

			size_t num = shader_system->get()->polygon_layout.size();

			XPLICIT_ASSERT(SUCCEEDED(driver->get().Device->CreateInputLayout(shader_system->get()->polygon_layout.data(),
				num,
				shader_system->get()->blob->GetBufferPointer(),
				shader_system->get()->blob->GetBufferSize(),
				shader_system->get()->input_layout.GetAddressOf())));

			break;
		}
		case D3D11_SHADER_TYPE::Hull:
		{
			shader_system->get()->shader_type += XPLICIT_HULL_SHADER;

			if (shader_system->compile() != 0)
			{
				print_error(shader_system->get()->error_blob);
				delete shader_system;

				return nullptr;
			}

			driver->get().Device->CreateHullShader(shader_system->get()->blob->GetBufferPointer(),
				shader_system->get()->blob->GetBufferSize(),
				nullptr,
				shader_system->get()->hull.GetAddressOf());

			break;
		}
		}

		return shader_system;
	}

	return nullptr;
}

#undef XPLICIT_DX11_STRING_SZ