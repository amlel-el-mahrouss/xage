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

 /**
 @file
 */

#include "DriverSystem.h"

namespace Xplicit::Renderer
{
	const char* DriverSystem::name() noexcept { return ("DriverSystem"); }

	RENDER_SYSTEM DriverSystem::api() noexcept { return RENDER_SYSTEM::INVALID; }
}