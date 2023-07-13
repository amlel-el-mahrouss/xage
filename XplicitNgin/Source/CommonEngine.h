/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
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

#define XPLICIT_DEFAULT_HEALTH	100U
#define XPLICIT_DEFAULT_NAME	"Visitor"

namespace Xplicit
{
	enum COMPONENT_ID_ENUM : std::int32_t
	{
		COMPONENT_ID_INVALID = 0,
		COMPONENT_ID_SCRIPT = 1,
		COMPONENT_ID_SOUND = 2,
		COMPONENT_ID_PARTICLE = 3,
		COMPONENT_ID_ROXML = 4,
		COMPONENT_ID_TEXTURE = 5,
		COMPONENT_ID_TOOL = 6,
		COMPONENT_ID_SMOKE = 7,
		COMPONENT_ID_FORCEFIELD = 8,
		COMPONENT_ID_EXPLOSION = 9,
		COMPONENT_ID_SKYBOX = 10,
		COMPONENT_ID_UNLOAD = 11,
		COMPONENT_ID_LOAD = 12,
		COMPONENT_ID_COUNT,
		// NOTE: add more here, don't forget you can easily mod the engine with CLua.
	};
}
