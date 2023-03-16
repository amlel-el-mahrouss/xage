/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Nplicit.h
 *			Purpose: Nplicit Common Header, Nplicit Handles 
			from mathematics to equation solving
 *
 * =====================================================================
 */

#pragma once

#include "Config.h"
#include "NMath.h" /* Nplicit Math Library */

namespace Xplicit::Physics
{
	template <typename TypeFloat = float>
	class NPLICIT_API PhysicsComponent final
	{
	public:
		PhysicsComponent() : Position(0, 0, 0), Velocity(0, 0, 0), Force(0, 0, 0) {}
		~PhysicsComponent() {}

		PhysicsComponent& operator=(const PhysicsComponent&) = default;
		PhysicsComponent(const PhysicsComponent&) = default;

	public:
		Vector<TypeFloat> Position;
		Vector<TypeFloat> Velocity;
		Vector<TypeFloat> Force;

		TypeFloat Mass;

	};

	template <typename TypeFloat = float>
	class NPLICIT_API PhysicsSystem final
	{
	public:
		PhysicsSystem() : m_rigid_bodies(), m_gravity(0, -9.81f, 0) {}
		virtual ~PhysicsSystem() = default;

		PhysicsSystem& operator=(const PhysicsSystem&) = delete;
		PhysicsSystem(const PhysicsSystem&) = delete;

		void update(const int32_t& dt)
		{
			for (PhysicsComponent<TypeFloat>* rigid : m_rigid_bodies)
			{
				rigid->Force.add(m_gravity.X * rigid->Mass, m_gravity.Y * rigid->Mass, m_gravity.Z * rigid->Mass);

				rigid->Velocity.add(rigid->Force.X / rigid->Mass * dt,
					rigid->Force.Y / rigid->Mass * dt,
					rigid->Force.Z / rigid->Mass * dt);

				rigid->Position.add(rigid->Velocity.X * dt, rigid->Velocity.Y * dt, rigid->Velocity.Z * dt);

				rigid->Force = Vector<TypeFloat>(0, 0, 0);
			}
		}

	public:
		void set(const Vector<float>& gravity) noexcept { m_gravity.add(gravity.X, gravity.Y, gravity.Z); }
		Vector<float>& get() noexcept { return m_gravity; }

		void add(PhysicsComponent<TypeFloat>* rigid)
		{
			if (!rigid)
				return;

			m_rigid_bodies.push_back(rigid);
		}

		bool remove(PhysicsComponent<TypeFloat>* rigid)
		{
			if (!rigid)
				return false;

			auto it = std::find(m_rigid_bodies.cbegin(), m_rigid_bodies.cend(), rigid);

			if (it != m_rigid_bodies.cend())
			{
				m_rigid_bodies.erase(it);
				return true;
			}

			return false;
		}

	private:
		std::vector<PhysicsComponent<TypeFloat>*> m_rigid_bodies;
		Vector<float> m_gravity;

	};
}