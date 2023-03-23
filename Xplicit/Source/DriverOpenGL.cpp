/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright XPX, all rights reserved.
 *
 *			File: DriverOpenGL.cpp
 *			Purpose: C++ Rendering Driver for OpenGL
 *
 * =====================================================================
 */

#include "GLad.h"
#include "DriverOpenGL.h"

 /**
 * @file
 */

namespace Xplicit::Renderer::OpenGL
{
	namespace Details
	{
		OpenGLShaderFactory::Traits OpenGLShaderFactory::operator()(const char* path)
		{
			FILE* file = fopen(path, "rb");

			if (!file)
				throw ShaderError(path);

			Traits traits;

			fseek(file, 0, SEEK_END);

			traits.shader_size = ftell(file);
			traits.shader_text.reserve(traits.shader_size);

			fseek(file, 0, SEEK_SET);
			fread(traits.shader_text.data(), traits.shader_size, SEEK_CUR, file);
			fclose(file);

			return traits;
		}
	}

	DriverSystemOpenGL::DriverSystemOpenGL()
	{
		
	}

	DriverSystemOpenGL::~DriverSystemOpenGL()
	{
		
	}

	const char* DriverSystemOpenGL::name() noexcept { return ("DriverSystemOpenGL"); }

	RENDER_SYSTEM DriverSystemOpenGL::api() { return RENDER_SYSTEM::OPENGL; }
}
