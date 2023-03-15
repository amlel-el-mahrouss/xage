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

#include <Foundation.h>
#include <Avx.h>

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

