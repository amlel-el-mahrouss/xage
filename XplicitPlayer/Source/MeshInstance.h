/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: MeshInstance.h
 *			Purpose: Custom Mesh Instance (.dae or .xm)
 *
 * =====================================================================
 */

#pragma once

#include <Xplicit.h>
#include <ApplicationContext.h>

namespace Xplicit::Client
{
	enum class RENDER_COMPONENT_TYPE
	{
		MESH,
		TEXTURE,
		PARTICLE_SYSTEM,
		COUNT,
	};
}