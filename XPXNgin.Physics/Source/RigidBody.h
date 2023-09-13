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
	class NPLICIT_API RigidBodyHelper final
	{
	public:
		RigidBodyHelper() = default;
		~RigidBodyHelper() override = default;

	public:
		XPLICIT_COPY_DEFAULT(RigidBodyHelper);

	public:
		static bool is_touching(Vector<float>& min, Vector<float>& max,
								Vector<float>& min_rhs, Vector<float>& max_rhs) noexcept
		{
			return (max.X > min_rhs.X &&
				min.X < max_rhs.X &&
				max.Y > min_rhs.Y &&
				min.Y < max_rhs.Y &&
				max.Z > min_rhs.Z &&
				min.Z < max_rhs.Z);
		}

	};
}