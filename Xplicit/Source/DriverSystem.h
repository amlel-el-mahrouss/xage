/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: DriverSystem.h
 *			Purpose: C++ Rendering Driver
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

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
		FORMAT_GLSL, // vulkan
		FORMAT_HLSL, // direct3d
		FORMAT_COUNT,
	};

	// platform depedent chars
#ifdef XPLICIT_WINDOWS
	using pchar = wchar_t;
#else
	using pchar = char;
#endif

	using string = std::basic_string<pchar>;

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
}