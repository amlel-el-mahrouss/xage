/*
 * =====================================================================
 *
 *			NplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Nplicit.h
 *			Purpose: Nplicit SDK, Nplicit handles 
			from mathematics to equation solving
 *
 * =====================================================================
 */

#pragma once

#include "Config.h"
#include "NMath.h" /* Nplicit Math Library */

namespace Xplicit::Nplicit
{
	template <typename TypeFloat = float>
	class NPLICIT_API PhysicsComponent
	{
	public:
		PhysicsComponent() : Position(0, 0, 0), Velocity(0, 0, 0), Force(0, 0, 0) {}
		virtual ~PhysicsComponent() {}

		PhysicsComponent& operator=(const PhysicsComponent&) = default;
		PhysicsComponent(const PhysicsComponent&) = default;

	public:
		Vector<TypeFloat> Position;
		Vector<TypeFloat> Velocity;
		Vector<TypeFloat> Force;

		TypeFloat Mass;

	};

	template <typename TypeFloat = float>
	class NPLICIT_API DynamicsSystem final
	{
	public:
		DynamicsSystem() : m_components(), m_gravity(0, -9.81f, 0) {}
		virtual ~DynamicsSystem() = default;

		DynamicsSystem& operator=(const DynamicsSystem&) = delete;
		DynamicsSystem(const DynamicsSystem&) = delete;

		void step(const int32_t& dt) noexcept
		{
			for (auto* component : m_components)
			{
				component->Force.add(m_gravity.X * component->Mass, m_gravity.Y * component->Mass, m_gravity.Z * component->Mass);

				component->Velocity.add(component->Force.X / component->Mass * dt,
					component->Force.Y / component->Mass * dt,
					component->Force.Z / component->Mass * dt);

				component->Position.add(component->Velocity.X * dt, component->Velocity.Y * dt, component->Velocity.Z * dt);

				component->Force = Vector<TypeFloat>(0, 0, 0);
			}
		}

	public:
		void set(const Vector<float>& gravity) noexcept { m_gravity.add(gravity.X, gravity.Y, gravity.Z); }
		Vector<float>& get() noexcept { return m_gravity; }

		void add(PhysicsComponent<TypeFloat>* rigid)
		{
			if (!rigid)
				return;

			m_components.push_back(rigid);
		}

		bool remove(PhysicsComponent<TypeFloat>* rigid)
		{
			if (!rigid)
				return false;

			auto it = std::find(m_components.cbegin(), m_components.cend(), rigid);

			if (it != m_components.cend())
			{
				m_components.erase(it);
				return true;
			}

			return false;
		}

	private:
		std::vector<PhysicsComponent<TypeFloat>*> m_components;
		Vector<float> m_gravity;

	};

	template <typename TypeFloat = float>
	struct NPLICIT_API Transform
	{
		Vector<TypeFloat> Position;
		Vector<TypeFloat> Scale;
		Quaternion<TypeFloat> Rotation;

	};
}