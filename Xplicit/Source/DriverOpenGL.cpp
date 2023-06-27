/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: DriverOpenGL.cpp
 *			Purpose: C++ Rendering Driver for OpenGL
 *
 * =====================================================================
 */

#include "DriverOpenGL.h"
#include <glfw3.h>

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
		: m_bClose(false)
	{
		
	}

	DriverSystemOpenGL::~DriverSystemOpenGL() = default;

	const char* DriverSystemOpenGL::name() noexcept { return ("DriverSystemOpenGL"); }

	RENDER_SYSTEM DriverSystemOpenGL::api() noexcept { return RENDER_SYSTEM::OPENGL; }

	const bool& DriverSystemOpenGL::is_closed() noexcept { return m_bClose;  }
	void DriverSystemOpenGL::close() noexcept { m_bClose = true; }

	void DriverSystemOpenGL::begin_scene(const Color<float>& clr)
	{
		glClearColor(clr.R / 255, clr.G / 255, clr.B / 255, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

	}

	void DriverSystemOpenGL::end_scene(const std::unique_ptr<Bites::GLFWWindow>& window) noexcept
	{
		glfwSwapBuffers(window->get());
		glfwPollEvents();
	}
}
