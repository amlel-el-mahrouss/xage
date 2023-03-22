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

namespace Xplicit::Renderer::OpenGL
{
	DriverSystemOpenGL::DriverSystemOpenGL()
	{
		// TODO
	}

	DriverSystemOpenGL::~DriverSystemOpenGL()
	{


	}

	const char* DriverSystemOpenGL::name() noexcept { return ("DriverSystemOpenGL"); }

	RENDER_SYSTEM DriverSystemOpenGL::api() { return RENDER_SYSTEM::OPENGL; }
}