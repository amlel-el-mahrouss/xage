/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright XPX, all rights reserved.
 *
 *			File: DriverOpenGL.h
 *			Purpose: C++ Rendering Driver for OpenGL
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include "DriverSystem.h"

#ifndef __gl_h_
#include "GLad.h"
#include <glfw3.h>
#endif

namespace Xplicit::Renderer::OpenGL
{
	class ShaderError : public std::runtime_error
	{
	public:
		ShaderError(const char* file) : std::runtime_error("Shader Error (OpenGL)"), Path(file) {}
		~ShaderError() = default; // let the ABI define that.

		ShaderError& operator=(const ShaderError&) = default;
		ShaderError(const ShaderError&) = default;

	public:
		std::filesystem::path Path;

	};

	namespace Details
	{
		class XPLICIT_API OpenGLShaderFactory final
		{
		public:
			struct Traits
			{
				std::string shader_text;
				size_t shader_size;
			};

		public:
			OpenGLShaderFactory() = default;
			~OpenGLShaderFactory() = default;

			XPLICIT_COPY_DEFAULT(OpenGLShaderFactory);

			OpenGLShaderFactory::Traits operator()(const char* path);

		};
	}

	class XPLICIT_API DriverSystemOpenGL : public DriverSystem
	{
	public:
		DriverSystemOpenGL();
		virtual ~DriverSystemOpenGL();

		XPLICIT_COPY_DELETE(DriverSystemOpenGL);
		XPLICIT_DRIVER_SYSTEM_OVERRIDE();

	public:
		const bool& is_closed() noexcept;
		void close() noexcept;

	private:
		bool m_bClose;

	};
}