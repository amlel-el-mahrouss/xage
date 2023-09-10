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
