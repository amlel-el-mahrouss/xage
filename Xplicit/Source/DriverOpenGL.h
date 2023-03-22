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

namespace Xplicit::Renderer::OpenGL
{
	class DriverSystemOpenGL : public DriverSystem
	{
	public:
		DriverSystemOpenGL();
		virtual ~DriverSystemOpenGL();

	};
}