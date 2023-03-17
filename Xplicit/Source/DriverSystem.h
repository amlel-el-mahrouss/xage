/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: DriverSystem.h
 *			Purpose: C++ Rendering Driver API
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include <Nplicit.h>

namespace Xplicit::Renderer
{
	enum class RENDER_SYSTEM : uint8_t
	{
		DIRECT3D11, // Direct3D 11, to make things explicit.
		VULKAN, // For Mobile users
		INVALID,
	};

	class XPLICIT_API DriverSystem
	{
	public:
		DriverSystem() {}
		virtual ~DriverSystem() {}

		DriverSystem& operator=(const DriverSystem&) = default;
		DriverSystem(const DriverSystem&) = default;

		virtual const char* name() noexcept;
		virtual RENDER_SYSTEM api();

	};

	enum XPLICIT_SHADER_FORMAT : uint8_t {
		FORMAT_GLSL, // Vulkan legacy shader.
		FORMAT_HLSL, // Direct3D 11 shader type.
		FORMAT_COUNT,
	};

	class XPLICIT_API ShaderSystem
	{
	public:
		ShaderSystem() = delete;

	public:
		explicit ShaderSystem(const pchar* filename, uint8_t format = FORMAT_HLSL)
			: m_shader(filename), m_type(format)
		{}

		virtual ~ShaderSystem() = default;

		ShaderSystem& operator=(const ShaderSystem&) = default;
		ShaderSystem(const ShaderSystem&) = default;

		const string& shader() noexcept { return m_shader; }
		const uint8_t& type() noexcept { return m_type; }

	public:
		virtual int operator()() = 0;

	protected:
		string m_shader;
		uint8_t m_type;

	};

	enum class RENDER_TYPE
	{
		RENDER_MESH, // .dae meshes (.fbx support is planned)
		RENDER_TEXTURE, // ID3D11Texture
		RENDER_POLYGON, // applies for 2d and 3d contexts.
		RENDER_PARTICLE_SYSTEM, // Emitter
		RENDER_TYPE_COUNT,
	};

	template <typename Implementer>
	class XPLICIT_API MeshComponent final
	{
	public:
		MeshComponent(
			Nplicit::Vector<float> position,
			Nplicit::Color<float> color
		)
			: m_position(position), m_color(color)
		{}

		~MeshComponent() = default;

		MeshComponent& operator=(const MeshComponent&) = default;
		MeshComponent(const MeshComponent&) = default;

	protected:
		Nplicit::Vector<float> m_position;
		Nplicit::Color<float> m_color;

		friend Implementer;

	};
}