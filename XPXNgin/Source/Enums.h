/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"

#include <CLua.hpp>

#define XPLICIT_DEFAULT_HEALTH	(100)
#define XPLICIT_MAX_ELEMENTS_INVENTORY (9)

namespace XPX
{
	enum COMPONENT_ID_ENUM : std::int32_t
	{
		COMPONENT_ID_INVALID = 0,
		COMPONENT_ID_SCRIPT = 1,
		COMPONENT_ID_ROXML = 2,
		COMPONENT_ID_COUNT,
		// NOTE: add more here, don't forget you can easily mod the engine with CLua.
	};

	enum
	{
		XPX_PART_ID,
		XPX_MESH_ID,
	};
}
