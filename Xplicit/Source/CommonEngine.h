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
	class XPLICIT_API Tool final
	{
	public:
		typedef std::int64_t ID;

		std::int32_t Slot{ 0 };
		bool Droppable{ false };
		String Name{ "Tool" };
		ID Owner{ -1 };

	};

	class XPLICIT_API Sound final
	{
	public:
		Vector<float> Position;
		String Filename{ "" };
		bool Looped{ false };

	};

	class XPLICIT_API Particle final
	{
	public:
		Vector<float> Position;
		String Filename{ "" };
		bool Looped{ false };

		ISceneNode* Emitter{ nullptr };

	};

	class XPLICIT_API Texture final
	{
	public:
		String Filename{ "" };
		bool Looped{ false };
		std::int32_t MatID{ 0 };

	};

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
