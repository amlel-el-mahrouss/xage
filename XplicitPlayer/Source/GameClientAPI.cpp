/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalNetworkMonitorEvent.h"
#include "LocalSoundComponent.h"
#include "Application.h"

#include <XplicitSound.h>
#include <lua/lua.hpp>
#include <codecvt>
#include <Util.h>
#include <Uri.h>

#ifdef XPLICIT_WINDOWS

static int lua_PlaySound(lua_State* L)
{
	try
	{
		Xplicit::String path = lua_tostring(L, 1);

		if (path.empty())
			return 0;

		if (auto snd = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::Player::LocalSoundComponent>("LocalSoundComponent"))
			snd->play(path);
	}
	catch (...)
	{
		XPLICIT_CRITICAL("There was an error executing this procedure!");
	}

	return 0;
}

static int lua_SetTitle(lua_State* L)
{
	Xplicit::String path = lua_tostring(L, 1);

	if (path.empty())
		return 0;

	RENDER->setWindowCaption(irr::core::stringw(path.c_str()).c_str());

	return 0;
}

void XplicitLoadClientLua() noexcept
{
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.RenderingService = {}");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.SoundService = {}");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_PlaySound);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "Engine_PlaySound");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.SoundService.Play = Engine_PlaySound");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_SetTitle);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "Engine_SetWindowCaption");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.RenderingService.SetWindowCaption = Engine_SetWindowCaption");

	Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::Player::LocalSoundComponent>();
}

#endif // ifdef XPLICIT_WINDOWS