/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: DriverSystem.h
 *			Purpose: C++ Rendering Driver API, replacement of the LegacyRenderer (irrlicht)
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"
#include <NplicitNgine.h>

#include <map>
#include <CLua.hpp>

namespace XPX::Renderer
{
	/// <summary>
	/// Tells what rendering api we're using.
	/// </summary>
	enum class RENDER_SYSTEM : uint8_t
	{
		DIRECT3D11,
		OPENGL,
		VULKAN,
		INVALID,
	};

	/// <summary>
	/// A generic DriverSystem
	/// </summary>
	class XPLICIT_API DriverSystem
	{
	public:
		DriverSystem() {}
		virtual ~DriverSystem() {}

		DriverSystem& operator=(const DriverSystem&) = default;
		DriverSystem(const DriverSystem&) = default;

		virtual const char* name() noexcept;
		virtual RENDER_SYSTEM api() noexcept;

	};

	enum XPLICIT_SHADER_FORMAT : uint8_t 
	{
		FORMAT_SPIRV,
		FORMAT_GLSL, // Vulkan legacy shader.
		FORMAT_HLSL, // Direct3D 11 shader type.
		FORMAT_COUNT,
	};

	/// <summary>
	/// Generic Shader System
	/// </summary>
	class XPLICIT_API ShaderSystem
	{
	public:
		ShaderSystem() = delete;

	public:
		explicit ShaderSystem(const PChar* filename, uint8_t type, uint8_t format = FORMAT_HLSL)
			: m_shader(filename), m_format(format), m_type(type)
		{}

		virtual ~ShaderSystem() = default;

		ShaderSystem& operator=(const ShaderSystem&) = default;
		ShaderSystem(const ShaderSystem&) = default;

		const PString& shader() noexcept { return m_shader; }
		const uint8_t& format() noexcept { return m_format; }
		const uint8_t& type() noexcept { return m_type; }

	public:
		virtual int compile() = 0;

	protected:
		uint8_t m_format;
		PString m_shader;
		uint8_t m_type;

	};

	/// <summary>
	/// RENDER_TYPE describes the type of render we're doing
	/// (Mesh, Texture, Polygon, Particles)
	/// </summary>
	enum class RENDER_TYPE
	{
		RENDER_MESH, // .dae meshes (.fbx support is planned)
		RENDER_TEXTURE, // ID3D11Texture
		RENDER_POLYGON, // applies for 2d and 3d contexts.
		RENDER_PARTICLE_SYSTEM, // Emitter
		RENDER_LIGHT,
		RENDER_TYPE_COUNT,
	};

	class XPLICIT_API DriverCameraSystem
	{
	public:
		explicit DriverCameraSystem() noexcept : m_vPos(0, 0, 0), m_vRot(0, 0, 0) {}
		virtual ~DriverCameraSystem() {}

	public:
		XPLICIT_COPY_DEFAULT(DriverCameraSystem);

	public:
		void set_position(const Vector<NplicitFloat>& pos) { m_vPos = pos; }
		void set_rotation(const Vector<NplicitFloat>& rot) { m_vRot = rot; }

	public:
		Vector<NplicitFloat>& position() noexcept { return m_vPos; }
		Vector<NplicitFloat>& rotation() noexcept { return m_vRot; }

	protected:
		Vector<NplicitFloat> m_vPos;
		Vector<NplicitFloat> m_vRot;

	};
}

#define XPLICIT_DRIVER_SYSTEM_OVERRIDE()\
		virtual const char* name() noexcept override;\
		virtual RENDER_SYSTEM api() noexcept override;
