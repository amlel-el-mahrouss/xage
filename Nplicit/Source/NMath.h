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

#define NPLICIT_PI (3.1415926535897932384626)

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

	return min(a, b);
}

namespace Xplicit
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

		bool operator<(const Vector<TypeFloat>& vec) const
		{
			return vec.X < X && vec.Y < Y && vec.Z < Z;
		}

		bool operator>(const Vector<TypeFloat>& vec) const
		{
			return vec.X > X && vec.Y > Y && vec.Z > Z;
		}

		bool operator==(const Vector& vec)
		{
			return vec.X == X && vec.Y == Y && vec.Z == Z;
		}

		bool operator!=(const Vector& vec)
		{
			return vec.X != X && vec.Y != Y && vec.Z != Z;
		}

	};

	template <typename TypeFloat = float>
	class NPLICIT_API Quaternion final
	{
	public:
		TypeFloat X, Y, Z, W;

	public:
		Quaternion() = default;
		~Quaternion() = default;

		Quaternion(TypeFloat x = 0, TypeFloat y = 0, TypeFloat z = 0, TypeFloat w = 0) noexcept
			: X(x), Y(y), Z(z), W(w)
		{
		}

		Quaternion& operator=(const Quaternion&) = default;
		Quaternion(const Quaternion&) = default;

	public:
		Quaternion& operator*=(const Quaternion& quat)
		{
#ifdef NPLICIT_USE_VECTORS
			__m256d x1{ X, Y, Z, W };
			__m256d x2{ quat.X, quat.Y, quat.Z, quat.W };

			__m256d sum = _mm256_mul_pd(x1, x2);
#else
			X *= quat.X;
			Y *= quat.Y;
			Z *= quat.Z;
			W *= quat.W;
#endif

			return *this;
		}

		Quaternion& operator+=(const Quaternion& quat)
		{
#ifdef NPLICIT_USE_VECTORS
			__m256d x1{ X, Y, Z, W };
			__m256d x2{ quat.X, quat.Y, quat.Z, quat.W };

			__m256d sum = _mm256_add_pd(x1, x2);
#else
			X += quat.X;
			Y += quat.Y;
			Z += quat.Z;
			W += quat.W;
#endif

			return *this;
		}

		Quaternion& operator-=(const Quaternion& quat)
		{
#ifdef NPLICIT_USE_VECTORS
			__m256d x1{ X, Y, Z, W };
			__m256d x2{ quat.X, quat.Y, quat.Z, quat.W };

			__m256d sum = _mm256_sub_pd(x1, x2);
#else
			X -= quat.X;
			Y -= quat.Y;
			Z -= quat.Z;
			W -= quat.W;
#endif

			return *this;
		}

		bool operator<(const Quaternion<TypeFloat>& quat) const
		{
			return quat.X < X && quat.Y < Y && quat.Z < Z && quat.W < W;
		}

		bool operator>(const Quaternion<TypeFloat>& quat) const
		{
			return quat.X > X && quat.Y > Y && quat.Z > Z && quat.W > W;
		}
	};

	template <typename TypeFloat = float>
	class NPLICIT_API Color final
	{
	public:
		TypeFloat R, G, B, A;

	public:
		Color() = default;
		~Color() = default;

		Color(TypeFloat r = 0, TypeFloat g = 0, TypeFloat b = 0, TypeFloat a = 255) noexcept
			: R(r), G(b), B(b), A(a)
		{
		}

		Color& operator=(const Color&) = default;
		Color(const Color&) = default;

	public:
		Color& operator*=(const Color& clr)
		{
#ifdef NPLICIT_USE_VECTORS
			__m256d x1{ R, G, B, A };
			__m256d x2{ clr.R, clr.G, clr.B, clr.A };

			__m256d sum = _mm256_mul_pd(x1, x2);
#else
			R *= clr.R;
			G *= clr.G;
			B *= clr.B;
			A *= clr.A;
#endif

			return *this;
		}

		Color& operator+=(const Color& clr)
		{
#ifdef NPLICIT_USE_VECTORS
			__m256d x1{ R, G, B, A };
			__m256d x2{ clr.R, clr.G, clr.B, clr.A };

			__m256d sum = _mm256_add_pd(x1, x2);
#else
			R += clr.R;
			G += clr.G;
			B += clr.B;
			A += clr.A;
#endif

			return *this;
		}

		Color& operator-=(const Color& clr)
		{
#ifdef NPLICIT_USE_VECTORS
			__m256d x1{ R, G, B, A };
			__m256d x2{ clr.R, clr.G, clr.B, clr.A };

			__m256d sum = _mm256_sub_pd(x1, x2);
#else
			R -= clr.R;
			G -= clr.G;
			B -= clr.B;
			A -= clr.A;
#endif

			return *this;
		}

		bool operator<(const Color<TypeFloat>& clr) const
		{
			return clr.R < R && clr.G < G && clr.B < B && clr.A < A;
		}

		bool operator>(const Color<TypeFloat>& clr) const
		{
			return clr.R > R && clr.G > G && clr.B > B && clr.A > A;
		}

	};
}
