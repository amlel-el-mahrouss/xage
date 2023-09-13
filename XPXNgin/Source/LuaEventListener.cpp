/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LuaEventListener.h"

namespace XPX
{
	LuaEventListener::LuaEventListener(RLua::RLuaProc proc) : mProcedure(proc) {}
	LuaEventListener::~LuaEventListener() = default;

	void LuaEventListener::update(EventPtr _event)
	{
		if (!_event)
			return;

		if (mProcedure(Lua::CLuaStateManager::get_singleton_ptr()->state()) != LUA_OK)
		{
			XPLICIT_CRITICAL(lua_tostring(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1));
		}
	}

	const char* LuaEventListener::name() noexcept { return "LuaEventListener"; }
}