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
#include "Component.h"

#include <Nplicit.h>

#define XPLICIT_DELTA_TIME		(1000.f)
#define XPLICIT_DEFAULT_HEALTH	(100U)

#define XPLICIT_DEFAULT_NAME	"NetworkInstance"

namespace Xplicit
{
	class XPLICIT_API PositionComponent final
	{
	public:
		explicit PositionComponent() : Position(0.f, 0.f, 0.f) {}
		~PositionComponent() = default;

		XPLICIT_COPY_DEFAULT(PositionComponent);
		
		String serialize() noexcept
		{
			String serialized = "<Vector3 X=\"" 
				+ std::to_string(Position.X) 
				+ "\" Y=\"" + std::to_string(Position.Y) 
				+ "\" Z=\"" + std::to_string(Position.Z) + "\" />";

			return serialized;
		}

	public:
		Vector<float> Position;

	};

	class XPLICIT_API SizeComponent final
	{
	public:
		explicit SizeComponent() : Size(0.f, 0.f, 0.f) {}
		~SizeComponent() = default;

		XPLICIT_COPY_DEFAULT(SizeComponent);

		String serialize() noexcept
		{
			String serialized = "<Dimension Width=\"" + std::to_string(Size.X) 
				+ "\" Height=\"" + std::to_string(Size.Y) + "\" />";

			return serialized;
		}

	public:
		Vector<float> Size;

	};

	class XPLICIT_API RenderableComponent final
	{
	public:
		RenderableComponent() = default;
		~RenderableComponent() = default;
		
		XPLICIT_COPY_DEFAULT(RenderableComponent);
		
		PositionComponent Position;
		SizeComponent Size;
		ISceneNode* Node{ nullptr };

	};

	class XPLICIT_API ToolComponent final
	{
	public:
		explicit ToolComponent() = default;
		~ToolComponent() = default;
		
		typedef std::int64_t ToolOwner;

		std::int32_t Slot;
		ToolOwner Owner;
		bool Droppable;
		String Name;

	};

	class XPLICIT_API SoundComponent final
	{
	public:
		explicit SoundComponent() = default;
		~SoundComponent() = default;

	public:
		PositionComponent Position;
		String Filename{ "" };
		bool Looped{ false };

	};

	enum COMPONENT_ID_ENUM : std::int32_t
	{
		COMPONENT_ID_SCRIPT = 0xc307,
		COMPONENT_ID_TEXTURE = 0x687e,
		COMPONENT_ID_SHAPE = 0x0408,
		COMPONENT_ID_TOOL = 0x5b2f,
		COMPONENT_ID_SOUND = 0xa8cf,
		COMPONENT_ID_CLASS = 0x5fae,

		/* custom components here. */
		COMPONENT_ID_INVALID = 0xffff,
	};
}
