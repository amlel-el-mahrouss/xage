/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Editor.cpp
 *			Purpose: Xplicit Studio
 *
 * =====================================================================
 */

/**
* @file
*/

#include <DriverD3D11.h>
#include <Bites.h>

#ifdef XPLICIT_WINDOWS

namespace Xplicit::Studio
{
	// this is horrendous, but windows imposes us that.
	constexpr const wchar_t* XPLICIT_APP_NAME_WIDE = L"Xplicit Studio";
	constexpr const char* XPLICIT_APP_NAME = "Xplicit Studio";
	constexpr const char* XPLICIT_APP_CLASS = "StudioXplicit";
}

INT32 WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR pCmdLine, int nCmdShow)
{
	try
	{
#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif

		Xplicit::Bites::Win32_Window* window = new Xplicit::Bites::Win32_Window(Xplicit::Studio::XPLICIT_APP_NAME,
																				  Xplicit::Studio::XPLICIT_APP_CLASS, hInst);

		std::unique_ptr<Xplicit::Renderer::DX11::DriverSystemD3D11> renderer = Xplicit::Renderer::DX11::make_driver_system_d3d11(window->get().WindowHandle);
		XPLICIT_ASSERT(renderer);

		auto component = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::Renderer::DX11::D3D11RenderComponent>();

		Xplicit::Renderer::DX11::D3D11ShaderSystem vertex_shader(L"Vertex.cso", Xplicit::Renderer::FORMAT_HLSL);

		vertex_shader.get()->entrypoint = new char[strlen("main")];
		memcpy(vertex_shader.get()->entrypoint, "main", strlen("main"));
		vertex_shader.get()->entrypoint[strlen("main")] = 0;

		vertex_shader.get()->shader_type = new char[strlen(XPLICIT_VERTEX_SHADER)];
		memcpy(vertex_shader.get()->shader_type, XPLICIT_VERTEX_SHADER, strlen(XPLICIT_VERTEX_SHADER));
		vertex_shader.get()->shader_type[strlen(XPLICIT_VERTEX_SHADER)] = 0;

		Xplicit::Renderer::DX11::D3D11ShaderSystem color_shader(L"Color.cso", Xplicit::Renderer::FORMAT_HLSL);

		color_shader.get()->entrypoint = new char[strlen("main")];
		memcpy(color_shader.get()->entrypoint, "main", strlen("main"));
		color_shader.get()->entrypoint[strlen("main")] = 0;

		color_shader.get()->shader_type = new char[strlen(XPLICIT_PIXEL_SHADER)];
		memcpy(color_shader.get()->shader_type, XPLICIT_PIXEL_SHADER, strlen(XPLICIT_PIXEL_SHADER));
		color_shader.get()->shader_type[strlen(XPLICIT_PIXEL_SHADER)] = 0;

		XPLICIT_ASSERT(color_shader() == 0);
		XPLICIT_ASSERT(vertex_shader() == 0);

		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader_ptr;
		
		HRESULT hr = renderer->get().Device->CreateVertexShader(vertex_shader.get()->blobs[0]->GetBufferPointer(),
			vertex_shader.get()->blobs[0]->GetBufferSize(),
			nullptr, vertex_shader_ptr.GetAddressOf());

		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader_ptr;

		hr = renderer->get().Device->CreatePixelShader(color_shader.get()->blobs[0]->GetBufferPointer(),
			color_shader.get()->blobs[0]->GetBufferSize(),
			nullptr, pixel_shader_ptr.GetAddressOf());

		if (FAILED(hr))
			return 1;

		D3D11_INPUT_ELEMENT_DESC  polygonLayout[2];
		RtlZeroMemory(&polygonLayout, sizeof(D3D11_INPUT_ELEMENT_DESC));
		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

		polygonLayout[1].SemanticName = "COLOR";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;

		size_t numElements = numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

		Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout_ptr;

		hr = renderer->get().Device->CreateInputLayout(polygonLayout,
			numElements,
			color_shader.get()->blobs[0]->GetBufferPointer(),
			color_shader.get()->blobs[0]->GetBufferSize(),
			input_layout_ptr.GetAddressOf());

		if (FAILED(hr))
			return 1;

		D3D11_BUFFER_DESC matrixBufferDesc;
		RtlZeroMemory(&matrixBufferDesc, sizeof(D3D11_BUFFER_DESC));

		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(Xplicit::Renderer::DX11::D3D11RenderComponent::Vertex);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		Microsoft::WRL::ComPtr<ID3D11Buffer> matrix_buffer_ptr;

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		hr = renderer->get().Device->CreateBuffer(&matrixBufferDesc, NULL, matrix_buffer_ptr.GetAddressOf());
		if (FAILED(hr))
		{
			return 1;
		}

		component->push_back(Xplicit::Nplicit::Vector<float>(-1.0f, -1.0f, 0.0f), 
			Xplicit::Nplicit::Color<float>(0.0f, 1.0f, 0.0f, 1.0f));
		component->push_back(Xplicit::Nplicit::Vector<float>(0.0f, 1.0f, 0.0f),
			Xplicit::Nplicit::Color<float>(0.0f, 1.0f, 0.0f, 1.0f));
		component->push_back(Xplicit::Nplicit::Vector<float>(1.0f, -1.0f, 0.0f),
			Xplicit::Nplicit::Color<float>(0.0f, 1.0f, 0.0f, 1.0f));

		component->set(renderer.get());
		component->create(renderer);

		return window->run(renderer.get());
	}
	catch (Xplicit::EngineError& err)
	{
		Xplicit::GUI::message_box(Xplicit::Studio::XPLICIT_APP_NAME_WIDE, L"C++ Engine Error!\r\nCan't Continue :(", MB_ICONASTERISK | MB_OK);
	}

	return 0;
}

#endif