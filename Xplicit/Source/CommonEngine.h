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
	// replicated structures.

	PACKED_STRUCT(class Tool final
	{
	public:
		bool Droppable{ false };

	public:
		int Slot{ 0 };
		int SceneID{ 0 };
		int Owner{ -1 };

	});

	PACKED_STRUCT(class Sound final
	{
	public:
		bool Looped{ false };
		
	public:
		int SceneID{ 0 };

	public:
		float X{ 0.0f };
		float Y{ 0.0f };
		float Z{ 0.0f };

	});

	PACKED_STRUCT(class Texture final
	{
	public:
		int SceneID{ 0 }; // The Scene ID.
		int ApplyMatID{ 0 }; // Material ID to apply the texture on.

	});

	PACKED_STRUCT(class Point final
	{
	public:
		float Pos_X{ 0.0f };
		float Pos_Y{ 0.0f };
		float Pos_Z{ 0.0f };

	public:
		float Size_X{ 0.0f };
		float Size_Y{ 0.0f };
		float Size_Z{ 0.0f };

	public:
		int SceneID{ 0 };
		int TypeID; // 0 : SMOKE, 1 : FORCE FIELD, 2 : EXPLOSION, 3 : RESERVED

	});

	constexpr int POINT_TYPE_SMOKE = 0;
	constexpr int POINT_TYPE_FORCEFIELD = 1;
	constexpr int POINT_TYPE_EXPLOSION = 2;

	using Smoke = Point;

	PACKED_STRUCT(class Skybox final
	{
	public:
		int SceneID_Up{ 0 }; // Up
		int SceneID_Fr{ 0 }; // Front
		int SceneID_Bk{ 0 }; // Back
		int SceneID_Dn{ 0 }; // Down
		int SceneID_Rt{ 0 }; // Right
		int SceneID_Lt{ 0 }; // Left

	});

	// component ids

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
		COMPONENT_ID_SKYBOX,
		// NOTE: add more here, don't forget you can easily mod the engine with CLua.
	};
}
