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

#define XPLICIT_DEFAULT_VEL (5.f)
#define XPLICIT_MOVEMENT_RATE (25)
#define XPLICIT_DEFAULT_HEALTH (100)
#define XPLICIT_DELTA_SECOND (1000.f)
#define XPLICIT_PLAYER_DEATH_COOLDOWN (1000)

namespace Xplicit
{
	class XPLICIT_API PositionComponent final : public Component
	{
	public:
		PositionComponent() : Position(0, 0, 0) {}
		virtual ~PositionComponent() = default;

		XPLICIT_COPY_DELETE(PositionComponent);

	public:
		Nplicit::Vector<float> Position;

	};

	class XPLICIT_API PhysicsComponent final : public Component
	{
	public:
		PhysicsComponent() = default;
		virtual ~PhysicsComponent() = default;

		XPLICIT_COPY_DELETE(PhysicsComponent);

	public:
		Nplicit::PhysicsComponent<float> Physics;

	};
}