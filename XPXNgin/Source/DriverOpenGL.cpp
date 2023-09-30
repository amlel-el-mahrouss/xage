/*
 * =====================================================================
 *
 *			XPXNgin (RigMan)
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: DriverOpenGL.cpp
 *			Purpose: C++ Rendering Driver for OpenGL
 *
 * =====================================================================
 */

#include "DriverOpenGL.h"
#include "FileIO.h"

#include <glfw3.h>

 /**
 * @file
 */

namespace XPX::Renderer::OpenGL
{
	namespace Details
	{
		OpenGLShaderFactory::Traits OpenGLShaderFactory::operator()(const char* path)
		{
			IFileReader file(path);

			if (!file)
				throw ShaderError(path);

			Traits traits;

			file.seek(file.seek_end);

			traits.shader_size = file.tell();
			traits.shader_text.reserve(traits.shader_size);

			file.seek(file.seek_beg);

			traits.shader_text += (file >> traits.shader_size);
			
			return traits;
		}
	}

	DriverSystemOpenGL::DriverSystemOpenGL() : m_bClose(false) {}
	DriverSystemOpenGL::~DriverSystemOpenGL() = default;

	const char* DriverSystemOpenGL::name() noexcept { return ("DriverSystemOpenGL"); }

	RENDER_SYSTEM DriverSystemOpenGL::api() noexcept { return RENDER_SYSTEM::OPENGL; }

	const bool& DriverSystemOpenGL::is_closed() noexcept { return m_bClose;  }

	void DriverSystemOpenGL::close() noexcept { m_bClose = true; }

	void DriverSystemOpenGL::begin_scene(const Color<float>& clr) noexcept
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
