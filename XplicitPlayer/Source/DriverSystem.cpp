/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: DriverSystem.cpp
 *			Purpose: C++ Rendering Driver
 *
 * =====================================================================
 */

#include "DriverSystem.h"

namespace Xplicit::Renderer
{
	const char* DriverSystem::name() noexcept { return ("DriverSystem"); }
	RENDER_SYSTEM DriverSystem::api() { return RENDER_SYSTEM::INVALID; }
}