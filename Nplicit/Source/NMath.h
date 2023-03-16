/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: NMath.h
 *			Purpose: Nplicit C++ Math Library
 *
 * =====================================================================
 */

#pragma once

#include "Config.h"

template <typename TypeLeft, typename TypeRight>
static inline constexpr auto nplicit_circle_add(
	const TypeLeft& lhs,
	const TypeRight rhs,
	const decltype(lhs + rhs) rmin = 0,
	const double rmax = 360
)
{
	auto result = lhs + rhs;
	auto range = rmax - rmin;

	while (result >= rmax)
		result -= range;

	while (result < rmin)
		result += range;

	return range;
}

template <typename TypeLeft, typename TypeRight>
constexpr auto nplicit_circle_sub(
	const TypeLeft& lhs,
	const TypeRight rhs,
	const decltype(lhs + rhs) rmin = 0,
	const double rmax = 360
)
{
	auto result = lhs - rhs;
	auto range = rmax - rmin;

	while (result >= rmax)
		result -= range;

	while (result < rmin)
		result += range;

	return range;
}

template <typename TypeLeft, typename TypeRight>
static inline constexpr auto nplicit_circle_mul(
	const TypeLeft& lhs,
	const TypeRight rhs,
	const decltype(lhs + rhs) rmin = 0,
	const double rmax = 360
)
{
	auto result = lhs * rhs;
	auto range = rmax - rmin;

	while (result >= rmax)
		result -= range;

	while (result < rmin)
		result += range;

	return range;
}

template <typename TypeLeft, typename TypeRight>
static inline constexpr auto nplicit_cricle_shortdist(
	const TypeLeft& lhs,
	const TypeRight rhs,
	const decltype(lhs + rhs) rmin = 0,
	const double rmax = 360
)
{
	auto a = nplicit_circle_sub(rhs, lhs, rmin, rmax);
	auto b = nplicit_circle_sub(lhs, rhs, rmin, rmax);

	return std::min(a, b);
}

namespace Xplicit::Physics
{
	template <typename TypeFloat = float>
	class NPLICIT_API Vector final
	{
	public:
		TypeFloat X, Y, Z;

	public:
		Vector() = default;
		~Vector() = default;

		Vector(TypeFloat x = 0, TypeFloat y = 0, TypeFloat z = 0) noexcept
		{
			this->add(x, y, z);
		}

		Vector& operator=(const Vector&) = default;
		Vector(const Vector&) = default;

		Vector& add(TypeFloat x = 0.f, TypeFloat y = 0.f, TypeFloat z = 0.f) noexcept
		{
#ifdef NPLICIT_USE_VECTORS
			__m256d x1{ X, Y, Z };
			__m256d x2{ x, y, z };

			__m256d sum = _mm256_add_pd(x1, x2);
#else
			this->X += x;
			this->Y += y;
			this->Z += z;
#endif

			return *this;
		}

		Vector& sub(TypeFloat x = 0.f, TypeFloat y = 0.f, TypeFloat z = 0.f) noexcept
		{
#ifdef NPLICIT_USE_VECTORS
			__m256d x1{ X, Y, Z };
			__m256d x2{ x, y, z };

			__m256d sum = _mm256_sub_pd(x1, x2);
#else
			this->X -= x;
			this->Y -= y;
			this->Z -= z;
#endif

			return *this;
		}

		Vector& mul(TypeFloat x = 1.f, TypeFloat y = 1.f, TypeFloat z = 1.f) noexcept
		{
#ifdef NPLICIT_USE_VECTORS
			__m256d x1{ X, Y, Z };
			__m256d x2{ x, y, z };

			__m256d sum = _mm256_mul_pd(x1, x2);
#else
			this->X *= x;
			this->Y *= y;
			this->Z *= z;
#endif

			return *this;
		}

	public:
		Vector& operator*=(const Vector& vec)
		{
			this->mul(vec.X, vec.Y, vec.Z);
			return *this;
		}

		Vector& operator+=(const Vector& vec)
		{
			this->add(vec.X, vec.Y, vec.Z);
			return *this;
		}

		Vector& operator-=(const Vector& vec)
		{
			this->sub(vec.X, vec.Y, vec.Z);
			return *this;
		}

	};
}