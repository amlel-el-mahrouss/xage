/*
 * =====================================================================
 *
 *			NplicitNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Nplicit.h"

namespace XPX
{
	template <typename TypeFloat = float>
	class NPLICIT_API RigidBodyComponent final : public virtual PhysicsComponent<TypeFloat>
	{
	public:
		RigidBodyComponent() = default;
		~RigidBodyComponent() override = default;

	public:
		XPLICIT_COPY_DEFAULT(RigidBodyComponent);

	public:
		bool is_touching(RigidBodyComponent<float>* rigid_body) noexcept
		{
			if (rigid_body)
				return (this->PositionMax.X > rigid_body->PositionMin.X &&
					this->PositionMin.X < rigid_body->PositionMax.X &&
					this->PositionMax.Y > rigid_body->PositionMin.Y &&
					this->PositionMin.Y < rigid_body->PositionMax.Y &&
					this->PositionMax.Z > rigid_body->PositionMin.Z &&
					this->PositionMin.Z < rigid_body->PositionMax.Z);

			return false;
		}

	};
}