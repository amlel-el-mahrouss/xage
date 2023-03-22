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

#include "Bites.h"
#include "Xplicit.h"
#include "DriverSystem.h"

#include <glfw3.h>

namespace Xplicit::Renderer::OpenGL
{
	namespace Details
	{
		class OpenGLShaderCompiler final
		{
		public:
			OpenGLShaderCompiler() = default;
			~OpenGLShaderCompiler() = default;

			XPLICIT_COPY_DEFAULT(OpenGLShaderCompiler);

		};
	}

	class DriverSystemOpenGL : public DriverSystem
	{
	public:
		DriverSystemOpenGL();
		virtual ~DriverSystemOpenGL();

		XPLICIT_COPY_DELETE(DriverSystemOpenGL);
		XPLICIT_DRIVER_SYSTEM_OVERRIDE();

	private:


	};
}