/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 *			File: CommonEngine.h
 *			Purpose: Common Engine Macros/Components/Properties
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include <Nplicit.h>

#include <CLua/CLua.hpp>

#define XPLICIT_DEFAULT_HEALTH	(100)
#define XPLICIT_MAX_ELEMENTS_INVENTORY (9)

namespace Xplicit
{
	enum COMPONENT_ID_ENUM : std::int32_t
	{
		COMPONENT_ID_INVALID = 0,
		COMPONENT_ID_SCRIPT = 1,
		COMPONENT_ID_ROXML = 2,
		COMPONENT_ID_COUNT,
		// NOTE: add more here, don't forget you can easily mod the engine with CLua.
	};
}
