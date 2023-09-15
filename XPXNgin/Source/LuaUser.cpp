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

static struct {
    CRITICAL_SECTION LockSct;
    BOOL Init;
} Gl;

void LuaLockInitial(lua_State* L)
{
    if (!Gl.Init)
    {
        InitializeCriticalSection(&Gl.LockSct);
        Gl.Init = TRUE;
    }
}

void LuaLockFinal(lua_State* L)
{
    if (Gl.Init)
    {
        DeleteCriticalSection(&Gl.LockSct);
        Gl.Init = FALSE;
    }
}

void LuaLock(lua_State* L)
{
    EnterCriticalSection(&Gl.LockSct);
}

void LuaUnlock(lua_State* L)
{
    LeaveCriticalSection(&Gl.LockSct);
}