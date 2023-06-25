/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "DataValue.h"
#include <lua/lua.hpp>

#ifdef _MSC_VER
#	define XPLICIT_CALLING_CONV __cdecl
#else
#	define XPLICIT_CALLING_CONV
#endif // ifdef

namespace Xplicit
{
	class GameObjectTool final
	{
	public:
		void (XPLICIT_CALLING_CONV *Equip)(lua_State*);
		void (XPLICIT_CALLING_CONV *Unequip)(lua_State*);
		void (XPLICIT_CALLING_CONV *Drop)(lua_State*);
		void (XPLICIT_CALLING_CONV *Use)(lua_State*);

	public:
		std::int32_t ID;
		String Tooltip;
		String Name;

	};
}