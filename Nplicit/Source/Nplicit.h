/*
 * =====================================================================
 *
 *			NplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Config.h"
#include "NMath.h" /* Nplicit Math Library */

namespace Xplicit
{
	template <typename TypeFloat = float>
	class NPLICIT_API PhysicsComponent
	{
	public:
		explicit PhysicsComponent() : 
			Position(0, 0, 0), 
			Velocity(0, 0, 0), 
			Force(0, 0, 0), 
			Anchored(false), 
			NoCollide(false)
		{}

		virtual ~PhysicsComponent() {}

		PhysicsComponent& operator=(const PhysicsComponent&) = default;
		PhysicsComponent(const PhysicsComponent&) = default;

	public:
		Vector<TypeFloat> Position;
		Vector<TypeFloat> Velocity;
		Vector<TypeFloat> Force;

	public:
		TypeFloat Mass;
		bool NoCollide;
		bool Anchored;

	};

	template <typename TypeFloat = float>
	class NPLICIT_API SolverSystem final
	{
	public:
		explicit SolverSystem() : mComponents(), mGravity(0, -9.81f, 0) {}
		virtual ~SolverSystem() = default;

	public:
		SolverSystem& operator=(const SolverSystem&) = delete;
		SolverSystem(const SolverSystem&) = delete;

	public:
		void step(const int64_t& dt /*delta time*/) noexcept
		{
			for (auto* component : mComponents)
			{
				if (component->Anchored)
					continue;

				component->Force.add(mGravity.X * component->Mass, mGravity.Y * component->Mass, mGravity.Z * component->Mass);
				component->Velocity.add(component->Force.X / component->Mass * dt, component->Force.Y / component->Mass * dt, component->Force.Z / component->Mass * dt);
				component->Position.add(component->Velocity.X * dt, component->Velocity.Y * dt, component->Velocity.Z * dt);
			}
		}

	public:
		void set(const Vector<float>& gravity) noexcept { mGravity.add(gravity.X, gravity.Y, gravity.Z); }

		Vector<float>& get() noexcept { return mGravity; }

		void add(PhysicsComponent<TypeFloat>* rigid)
		{
			if (!rigid)
				return;

			mComponents.push_back(rigid);
		}

		bool remove(PhysicsComponent<TypeFloat>* rigid)
		{
			if (!rigid)
				return false;

			auto it = std::find(mComponents.cbegin(), mComponents.cend(), rigid);

			if (it != mComponents.cend())
			{
				mComponents.erase(it);
				return true;
			}

			return false;
		}

	private:
		std::vector<PhysicsComponent<TypeFloat>*> mComponents;
		Vector<float> mGravity;

	};

	template <typename TypeFloat = float>
	struct NPLICIT_API Transform
	{
		Quaternion<TypeFloat> Rotation;
		Vector<TypeFloat> Position;
		Vector<TypeFloat> Scale;
		
	};
}