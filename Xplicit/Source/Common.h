/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Common.h
 *			Purpose: Common Engine Macros/Components
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include "Component.h"

#include <Nplicit.h>

#define XPLICIT_DEFAULT_VEL (0.035f)
#define XPLICIT_MOVEMENT_RATE (50U)
#define XPLICIT_DEFAULT_HEALTH (100U)
#define XPLICIT_DELTA_TIME (500.f)
#define XPLICIT_COOLDOWN (1000)

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
			String serialized;
			serialized = "<Pos X=\"" + std::to_string(Position.X) + "\" Y=\"" + std::to_string(Position.Y) + "\" Z=\"" + std::to_string(Position.Z) + "\" />";

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
			String serialized;
			serialized = "<Size Width=\"" + std::to_string(Size.X) + "\" Height=\"" + std::to_string(Size.Y) + "\" />";

			return serialized;
		}

	public:
		Vector<float> Size;

	};

	class XPLICIT_API BrickComponent
	{
	public:
		BrickComponent() = default;
		virtual ~BrickComponent() = default;

		XPLICIT_COPY_DEFAULT(BrickComponent);

	public:
		PositionComponent Position;
		SizeComponent Size;

	};
}