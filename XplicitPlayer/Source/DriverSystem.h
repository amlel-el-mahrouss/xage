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

#include <Xplicit.h>

namespace Xplicit::Renderer
{
	enum class RENDER_SYSTEM : uint8_t
	{
		DIRECT3D11,
		INVALID,
		VULKAN,
	};

	class DriverSystem
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
		FORMAT_GLSL,
		FORMAT_HLSL,
		FORMAT_COUNT,
	};

	class Shader final 
	{
	public:
		Shader() = delete;

	public:
		explicit Shader(const char* glsl, uint8_t format = FORMAT_GLSL)
			: m_strGlsl(glsl)
		{}

		~Shader() = default;

		Shader& operator=(const Shader&) = default;
		Shader(const Shader&) = default;

		const std::string& shader() noexcept { return m_strGlsl; }
		const uint8_t& type() noexcept { return m_glslType; }

	private:
		std::string m_strGlsl;
		uint8_t m_glslType;

	};
}