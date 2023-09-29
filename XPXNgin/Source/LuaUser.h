/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#ifndef XPLICIT_API
#	define XPLICIT_API
#endif

XPLICIT_API void LuaLock(lua_State* L);
XPLICIT_API void LuaUnlock(lua_State* L);

#define clua_lock()  LuaLock(XPX::Lua::CLuaStateManager::get_singleton_ptr()->state())
#define clua_unlock()  LuaUnlock(XPX::Lua::CLuaStateManager::get_singleton_ptr()->state())