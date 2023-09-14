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
		static bool is_touching(Vector<NplicitFloat>& min, Vector<NplicitFloat>& max,
								Vector<NplicitFloat>& min_rhs, Vector<NplicitFloat>& max_rhs) noexcept
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