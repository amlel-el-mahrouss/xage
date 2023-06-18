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
	class XPLICIT_API PositionComponent final : public Component
	{
	public:
		explicit PositionComponent() : Position(0.f, 0.f, 0.f) {}
		virtual ~PositionComponent() = default;

		XPLICIT_COPY_DEFAULT(PositionComponent);

	public:
		String serialize() noexcept
		{
			String serialized = "<Pos X=\"" 
				+ std::to_string(Position.X) 
				+ "\" Y=\"" + std::to_string(Position.Y) 
				+ "\" Z=\"" + std::to_string(Position.Z) + "\" />";

			return serialized;
		}

	public:
		Quaternion<float> Position;

	};

	class XPLICIT_API SizeComponent final : public Component
	{
	public:
		explicit SizeComponent() : Size(0.f, 0.f, 0.f) {}
		virtual ~SizeComponent() = default;

		XPLICIT_COPY_DEFAULT(SizeComponent);

	public:
		String serialize() noexcept
		{
			String serialized = "<Size Width=\"" + std::to_string(Size.X) 
				+ "\" Height=\"" + std::to_string(Size.Y) + "\" />";

			return serialized;
		}

	public:
		Vector<float> Size;

	};

	class XPLICIT_API RenderableComponent : public Component
	{
	public:
		RenderableComponent() = default;
		virtual ~RenderableComponent() = default;

	public:
		XPLICIT_COPY_DEFAULT(RenderableComponent);

	public:
		PositionComponent Position;
		SizeComponent Size;

	};

	enum class COMPONENT_ID_ENUM : std::int32_t
	{
		COMPONENT_ID_SCRIPT = 0xc307,
		COMPONENT_ID_TEXTURE = 0x687e,
		COMPONENT_ID_MODEL = 0x0408,
		COMPONENT_ID_SOUND = 0xa8cf,
		COMPONENT_ID_UI = 0x0d7f,
		COMPONENT_ID_INVALID = 0xffff,
	};
}
