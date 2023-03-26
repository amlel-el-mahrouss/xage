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

#define XPLICIT_DEFAULT_VEL (2.f)
#define XPLICIT_MOVEMENT_RATE (25)
#define XPLICIT_DEFAULT_HEALTH (100)
#define XPLICIT_DELTA_TIME (1000.f)
#define XPLICIT_PLAYER_DEATH_COOLDOWN (1000)

namespace Xplicit
{
	class XPLICIT_API PositionComponent final : public Component
	{
	public:
		PositionComponent() : Position(0.f, 0.f, 0.f) {}
		virtual ~PositionComponent() = default;

		XPLICIT_COPY_DEFAULT(PositionComponent);

	public:
		Nplicit::Vector<float> Position;

	};

	class XPLICIT_API PhysicsComponent final : public Component
	{
	public:
		PhysicsComponent() = default;
		virtual ~PhysicsComponent() = default;

		XPLICIT_COPY_DEFAULT(PhysicsComponent);

	public:
		Nplicit::PhysicsComponent<float> Physics;

	};
}