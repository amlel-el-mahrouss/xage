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

				component->Force = Vector<float>(mGravity.X * component->Mass, 
					mGravity.Y * component->Mass,
					mGravity.Z * component->Mass);


				component->Velocity = Vector<float>(component->Force.X * component->Mass,
					component->Force.Y * component->Mass,
					component->Force.Z * component->Mass);

				component->Position = Vector<float>(component->Velocity.X* dt, component->Velocity.Y* dt, component->Velocity.Z* dt);

				component->Force = Vector<float>(0, 0, 0);
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
		Quaternion<TypeFloat> Rotation{ 0, 0, 0 };
		Vector<TypeFloat> Position{ 0, 0, 0 };
		Vector<TypeFloat> Scale{ 0, 0, 0 };
		
	};

#ifndef __NPLICIT_DETAIL_IEE__
	typedef float NplicitFloat;
#else
	typedef double NplicitFloat;
#endif

	enum COLLISION_RESOLUTION_STAT
	{
		COLLISION_RESOLUTION_STOP,
		COLLISION_RESOLUTION_RUN,
	};

	class NPLICIT_API CollisionNode final
	{
	public:
		CollisionNode() = default;
		~CollisionNode() = default;

	public:
		XPLICIT_COPY_DEFAULT(CollisionNode);

	public:
		Transform<NplicitFloat> Entity;
		COLLISION_RESOLUTION_STAT Stat{ COLLISION_RESOLUTION_STOP };

	public:
		CollisionNode* Parent{ nullptr };
		CollisionNode* Child{ nullptr };
		bool Valid{ true };

	public:
		void simulate() noexcept
		{
			if (Stat == COLLISION_RESOLUTION_RUN)
				return;

			//! Start simulation
			Stat = COLLISION_RESOLUTION_RUN;

			//! Simulate our zone.
			Thread zone([&]() {
				if (this)
				{
					CollisionNode* entt = this;

					while (entt)
					{
						// We are colliding.
						if (entt->Entity.Position == this->Entity.Position)
							this->Valid = false;

						entt = entt->Child;
					}
				}

				if (this)
				{
					CollisionNode* entt_parent = this;

					while (entt_parent)
					{
						// We are colliding.
						if (entt_parent->Entity.Position == this->Entity.Position)
							this->Valid = false;

						entt_parent = entt_parent->Parent;
					}

					if (this)
						this->Stat = COLLISION_RESOLUTION_STOP;
				}
			});

			zone.detach();
		}

		CollisionNode& operator+=(CollisionNode* collision)
		{
			if (collision)
			{
				CollisionNode* prev = Child;
				CollisionNode* entt = Child;
				
				while (entt)
				{
					prev = entt;
					entt = entt->Child;
				}

				if (prev)
				{
					prev->Child = collision;
					collision->Parent = prev;
				}
			}

			return *this;
		}

	};

	typedef CollisionNode CollisionNodeRoot;
}