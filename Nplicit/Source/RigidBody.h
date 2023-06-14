/*
 * =====================================================================
 *
 *			NplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: RigidBody.h
 *			Purpose: RigidBodyComponent Class
 *
 * =====================================================================
 */

#pragma once

#include "Nplicit.h"

namespace Xplicit
{
	template <typename TypeFloat = float>
	class NPLICIT_API RigidBodyComponent final : public virtual PhysicsComponent<TypeFloat>
	{
	public:
		RigidBodyComponent() = default;
		virtual ~RigidBodyComponent() = default;

		RigidBodyComponent& operator=(const RigidBodyComponent&) = default;
		RigidBodyComponent(const RigidBodyComponent&) = default;

		void set(const Vector<TypeFloat>& pos, const Vector<TypeFloat>& velocity, const Vector<TypeFloat>& force) noexcept
		{
			this->Position = pos;
			this->Force = force;
			this->Velocity = velocity;

#ifdef XPLICIT_DEBUG
			XPLICIT_INFO("[RigidBodyComponent::set] New parameters set for RigidBody.");
#endif
		}

	};
}