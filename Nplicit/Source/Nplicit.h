/*
 * =====================================================================
 *
 *			NplicitNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Config.h"
#include "NMath.h" /* Nplicit Math Library */

namespace XPX
{
	template <typename TypeFloat = float>
	class NPLICIT_API PhysicsComponent
	{
	public:
		explicit PhysicsComponent() : 
			PositionMin(0, 0, 0), 
			PositionMax(0, 0, 0),
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
		Vector<TypeFloat> PositionMax;
		Vector<TypeFloat> PositionMin;

	public:
		Vector<TypeFloat> Velocity;
		Vector<TypeFloat> Force;
		Vector<TypeFloat> Gravity; // it has it's own gravity. so that we can easily customize it.

	public:
		TypeFloat Mass;
		bool NoCollide;
		bool Anchored;

	};

#ifndef __NPLICIT_DETAIL_IEE754__
	typedef float NplicitFloat;
#else
	typedef double NplicitFloat;
#endif
}
