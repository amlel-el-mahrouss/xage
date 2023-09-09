/*
 * =====================================================================
 *
 *			NplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
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
			Gravity(0, 0, 0),
			Force(0, 0, 0), 
			Anchored(false), 
			NoCollide(false)
		{}

		virtual ~PhysicsComponent() {}

		PhysicsComponent& operator=(const PhysicsComponent&) = default;
		PhysicsComponent(const PhysicsComponent&) = default;

	public:
		bool step(const int64_t& dt /*delta time*/) noexcept
		{
			if (this->Anchored)
				return false;

			this->Force = Vector<float>(this->Gravity.X * this->Mass,
				this->Gravity.Y * this->Mass,
				this->Gravity.Z * this->Mass);

			this->Velocity = Vector<float>(this->Force.X * this->Mass,
				this->Force.Y * this->Mass,
				this->Force.Z * this->Mass);

			this->Position = Vector<float>(this->Velocity.X * dt, this->Velocity.Y * dt, this->Velocity.Z * dt);

			this->Force = Vector<float>(0, 0, 0);

			return true;
		}

	public:
		Vector<TypeFloat> Position;
		Vector<TypeFloat> Velocity;
		Vector<TypeFloat> Force;
		Vector<TypeFloat> Gravity; // it has it's own gravity. so that we can easily customize it.

	public:
		TypeFloat Mass;
		bool NoCollide;
		bool Anchored;

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
}

// if you choose to not inherit from PhysicsComponent, use this.
#define NPLICIT_DO_UPDATE_PHYSICS(PHYS, DT)\
self->PHYS.Anchored = self->index_as_bool("Anchored");\
\
if (self->PHYS.step(self->DT))\
{\
	self->PHYS.Gravity.X = self->index_as_number("Gravity.X");\
	self->PHYS.Gravity.Y = self->index_as_number("Gravity.Y");\
	self->PHYS.Gravity.Z = self->index_as_number("Gravity.Z");\
\
	++self->mDt;\
\
	self->pos().X = self->PHYS.Position.X;\
	self->pos().Y = self->PHYS.Position.Y;\
	self->pos().Z = self->PHYS.Position.Z;\
}\


