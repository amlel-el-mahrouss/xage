/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "NginCore.h"

#include <windows.h>
#include <CLua.hpp>

#include "LuaUser.h"

static struct
{
    CRITICAL_SECTION LockSct;
    BOOL Init;
} gLuaLock;

void LuaLockInitial(lua_State* L)
{
    if (!gLuaLock.Init)
    {
        InitializeCriticalSection(&gLuaLock.LockSct);
        gLuaLock.Init = TRUE;
    }
}

void LuaLockFinal(lua_State* L)
{
    if (gLuaLock.Init)
    {
        DeleteCriticalSection(&gLuaLock.LockSct);
        gLuaLock.Init = FALSE;
    }
}

void LuaLock(lua_State* L)
{
    EnterCriticalSection(&gLuaLock.LockSct);
}

void LuaUnlock(lua_State* L)
{
    LeaveCriticalSection(&gLuaLock.LockSct);
}