/*
 * =====================================================================
 *
 *			NplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: PhysicsComponent.h
 *			Purpose: Rigid Bodies
 *
 * =====================================================================
 */

#pragma once

#include "Nplicit.h"

namespace Xplicit::Physics
{

	class NPLICIT_API RigidBody final
	{
	public:
		RigidBody() = default;
		~RigidBody() = default;

		RigidBody& operator=(const RigidBody&) = delete;
		RigidBody(const RigidBody&) = delete;

	};
}