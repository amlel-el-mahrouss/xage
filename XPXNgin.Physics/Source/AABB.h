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
	class NPLICIT_API AABBHelper final
	{
	public:
		static bool is_touching(Vector<NplicitFloat>& min, Vector<NplicitFloat>& max) noexcept
		{
			if (max == min)
				return true;

			if (max > min)
			{
				if ((max.sub(min.X, min.Y, min.Z) == Vector<NplicitFloat>(0, 0, 0)))
					return true;

				auto upscaled = min.mul(2, 2, 2);

				if (upscaled == max)
					return true;
			}
			else
			{
				auto downscaled = max.div(2, 2, 2);

				if (downscaled == min)
					return true;
			}

			return false;
		}

	};
}