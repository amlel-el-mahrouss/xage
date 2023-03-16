/*
 * =====================================================================
 *
 *			NplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: RigidBody.h
 *			Purpose: RigidBody Class
 *
 * =====================================================================
 */

#pragma once

#include "Nplicit.h"

namespace Xplicit::Physics
{
	template <typename TypeFloat = float>
	class NPLICIT_API RigidBody final
	{
	public:
		RigidBody() = default;
		~RigidBody() = default;

		RigidBody& operator=(const RigidBody&) = default;
		RigidBody(const RigidBody&) = default;

		/*
				We need to take into consideration:

				Vector<TypeFloat> Position;
				Vector<TypeFloat> Velocity;
				Vector<TypeFloat> Force;
		*/

		void set(const Vector<TypeFloat>& pos, const Vector<TypeFloat>& velocity, const Vector<TypeFloat>& force) noexcept
		{
			m_physics.Position = pos;
			m_physics.Force = force;
			m_physics.Velocity = velocity;

#ifdef XPLICIT_DEBUG
			XPLICIT_INFO("[RigidBody::set] New parameters set for RigidBody");
#endif
		}

		PhysicsComponent<TypeFloat>& get() noexcept { return m_physics; }

	private:
		PhysicsComponent<TypeFloat> m_physics;

	};
}