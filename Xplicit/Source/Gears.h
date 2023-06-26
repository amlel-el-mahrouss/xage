/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

#include <lua/lua.hpp>

#ifdef _MSC_VER
#	define XPLICIT_CALLING_CONV __cdecl
#else
#	define XPLICIT_CALLING_CONV
#endif // ifdef

namespace Xplicit
{
	/* 
		Class: Gear View.
		Purpose: Gear data container.

		Example:
			Tooltip: "The best gun in the world
			Name: Fargun
	*/

	class GearView final
	{
	public:
		String Tooltip; // gear tooltip
		String Name; // gear name
		String Mesh; // xasset mesh
		int32_t Delay;
		int32_t Dmg;
		int32_t Id;

	};
}