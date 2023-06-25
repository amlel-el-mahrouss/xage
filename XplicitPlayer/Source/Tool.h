/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <CommonEngine.h>
#include <lua/lua.hpp>

#include "Application.h"
#include "Mesh.h"

/* All of these classes are non-virtual for a reason. */

#ifdef _MSC_VER
#	define XPLICIT_CALLING_CONV __cdecl
#else
#	define XPLICIT_CALLING_CONV
#endif // ifdef

namespace Xplicit::Player
{
	class Tool final
	{
	public:
		Tool() = default;
		~Tool() = default;

	public:
		XPLICIT_COPY_DEFAULT(Tool);

	public:
		const char* name() { return Name.c_str(); }
		const std::int32_t id() { return ID; }

	public:
		void (XPLICIT_CALLING_CONV *Equip)(lua_State*);
		void (XPLICIT_CALLING_CONV *Unequip)(lua_State*);
		void (XPLICIT_CALLING_CONV *Drop)(lua_State*);
		void (XPLICIT_CALLING_CONV *Use)(lua_State*);

		StaticMesh* Mesh;
		std::int32_t ID;
		String Tooltip;
		String Name;

	};
}