/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: CommonEngine.h
 *			Purpose: Common Engine Macros/Components/Properties
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include <Nplicit.h>

#define XPLICIT_DEFAULT_HEALTH	(100U)

#define XPLICIT_DEFAULT_NAME	"Humanoid"

namespace Xplicit
{
	/*
	 * ============================
	 * 
	 * Xplicit Replicated structures.
	 * Use them to create new components client-side.
	 * 
	 * The objective here is let lua access these structs, that can be also modified using C++ code.
	 * 
	 * ============================
     */

	PACKED_STRUCT(class Tool final
	{
	public:
		typedef int64_t ID;

	public:
		bool Droppable{ false };
		int32_t Slot{ 0 };
		int SceneID{ 0 };
		ID Owner{ -1 };

	});

	PACKED_STRUCT(class Sound final
	{
	public:
		bool Looped{ false };
		int SceneID{ 0 };
		float X{ 0.0f };
		float Y{ 0.0f };
		float Z{ 0.0f };

	});

	PACKED_STRUCT(class Texture final
	{
	public:
		int SceneID{ 0 };
		int MatID{ 0 };

	});

	enum COMPONENT_ID_ENUM : std::int32_t
	{
		COMPONENT_ID_INVALID = 0,
		COMPONENT_ID_SCRIPT = 1,
		COMPONENT_ID_SOUND = 2,
		COMPONENT_ID_PARTICLE = 3,
		COMPONENT_ID_ROXML = 4,
		COMPONENT_ID_TEXTURE = 5,

		// Continue here...
	};
}
