/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Common.h
 *			Purpose: Common Engine Macros/Components/Properties
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include "Component.h"

#include <Nplicit.h>

#define XPLICIT_DELTA_TIME		(1000.f)
#define XPLICIT_COOLDOWN		(1000U)
#define XPLICIT_DEFAULT_HEALTH	(100U)
#define XPLICIT_MOVEMENT_RATE	(50U)

namespace Xplicit
{
	class XPLICIT_API PositionComponent final
	{
	public:
		explicit PositionComponent() : Position(0.f, 0.f, 0.f) {}
		virtual ~PositionComponent() = default;

		XPLICIT_COPY_DEFAULT(PositionComponent);

	public:
		String serialize() noexcept
		{
			String serialized = "<Vector3 X=\"" 
				+ std::to_string(Position.X) 
				+ "\" Y=\"" + std::to_string(Position.Y) 
				+ "\" Z=\"" + std::to_string(Position.Z) + "\" />";

			return serialized;
		}

	public:
		Quaternion<float> Position;

	};

	class XPLICIT_API SizeComponent final
	{
	public:
		explicit SizeComponent() : Size(0.f, 0.f, 0.f) {}
		virtual ~SizeComponent() = default;

		XPLICIT_COPY_DEFAULT(SizeComponent);

	public:
		String serialize() noexcept
		{
			String serialized = "<Dimension Width=\"" + std::to_string(Size.X) 
				+ "\" Height=\"" + std::to_string(Size.Y) + "\" />";

			return serialized;
		}

	public:
		Vector<float> Size;

	};

	class XPLICIT_API RenderableComponent
	{
	public:
		RenderableComponent() = default;
		virtual ~RenderableComponent() = default;

	public:
		XPLICIT_COPY_DEFAULT(RenderableComponent);

	public:
		PositionComponent Position;
		scene::ISceneNode* Node;
		SizeComponent Size;

	};

	class XPLICIT_API ToolComponent final
	{
	public:
		explicit ToolComponent() = default;
		virtual ~ToolComponent() = default;

	public:
		std::int32_t OwnerHash;
		std::int32_t Slot;
		bool Droppable;
		String Name;

	};

	class XPLICIT_API SoundComponent final
	{
	public:
		explicit SoundComponent() = default;
		virtual ~SoundComponent() = default;

	public:
		PositionComponent Position;
		String Filename;
		bool Looped;

	};

	enum class COMPONENT_ID_ENUM : std::int32_t
	{
		COMPONENT_ID_SCRIPT = 0xc307,
		COMPONENT_ID_TEX = 0x687e,
		COMPONENT_ID_PART = 0x0408,
		COMPONENT_ID_TOOL = 0x5b2f,
		COMPONENT_ID_SOUND = 0xa8cf,
		COMPONENT_ID_START = 0x0d7f, /* start of custom components */

		/* custom components here. */
		COMPONENT_ID_INVALID = 0xffff,
	};
}
