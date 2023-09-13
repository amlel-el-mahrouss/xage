/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalNetworkMonitorEvent.h"

#include "StaticBundleMesh.h"
#include "SoundComponent.h"
#include "PartComponent.h"
#include "MeshComponent.h"
#include "Application.h"
#include "ClientUtils.h"
#include "MenuUI.h"

#include <SoundNgin.h>
#include <CLua.hpp>
#include <LuaAPI.h>
#include <codecvt>
#include <RoXML.h>
#include <Util.h>
#include <Uri.h>

 // RoXML parser
XPX::RoXML::RoXMLDocumentParser XPLICIT_PARSER;

#ifndef XPLICIT_XASSET_IDENT
#	define XPLICIT_XASSET_IDENT ("xasset")
#endif // ifndef XPLICIT_XASSET_IDENT

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> XPLICIT_TO_WCHAR;

#define XPLICIT_ALLOWED_AUDIO_EXTENSION ".wav"

static int lua_PlaySound(lua_State* L)
{
	try
	{
		XPX::String url = lua_tostring(L, 1);

		// that means if we don't find it, then fail silently.
		if (url.find(XPLICIT_ALLOWED_AUDIO_EXTENSION) == XPX::String::npos)
			return 0;

		if (url.empty() ||
			url.find(XPLICIT_XASSET_IDENT) == XPX::String::npos)
			return 0;

		XPX::String substr_tmp = url.erase(url.find(XPLICIT_XASSET_IDENT), strlen(XPLICIT_XASSET_IDENT) + 3);

		url.clear();
		url = "/";
		url += substr_tmp;

		static XPX::LocalNetworkMonitorEvent* monitor = XPX::EventSystem::get_singleton_ptr()->get<XPX::LocalNetworkMonitorEvent>("LocalNetworkMonitorEvent");

		if (!monitor)
			monitor = XPX::EventSystem::get_singleton_ptr()->get<XPX::LocalNetworkMonitorEvent>("LocalNetworkMonitorEvent");

		XPX::String endpoint = XPLICIT_XASSET_ENDPOINT;
		monitor->HTTP->set_endpoint(endpoint);

		auto tmp = std::to_string(xplicit_get_epoch()) + "-tmp-snd.wav";

		if (monitor &&
			monitor->HTTP->download(url, tmp))
		{
			XPLICIT_GET_DATA_DIR(full_path);

			XPX::String full_download_path;

			full_download_path += full_path;
			full_download_path += "Contents/";
			full_download_path += tmp;

			if (auto snd = XPX::ComponentSystem::get_singleton_ptr()->get<XPX::SoundComponent>("SoundComponent"))
				snd->play(full_download_path);
		}

	}
	catch (...)
	{
		XPLICIT_CRITICAL("There was an error executing this procedure!");
	}

	return 0;
}

static int lua_GetX(lua_State* L)
{
	lua_pushnumber(L, CAD->getCursorControl()->getPosition().Y);
	return 1;
}

static int lua_GetY(lua_State* L)
{
	lua_pushnumber(L, CAD->getCursorControl()->getPosition().Y);
	return 1;
}

static int lua_MakeRect(lua_State* L)
{
	const char* parent = lua_tostring(L, 1);
	const char* name = lua_tostring(L, 2);

	XPX::RectComponent* frame = XPX::ComponentSystem::get_singleton_ptr()->add<XPX::RectComponent>(parent, name);

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("return {}.{}", lua_tostring(L, 1), lua_tostring(L, 2)).c_str());
	return 1;
}

static int lua_KeyDown(lua_State* L)
{
	lua_pushboolean(L, KEYBOARD->key_down());
	return 1;
}

static int lua_IsKeyDown(lua_State* L)
{
	int key = lua_tointeger(L, 1);

	lua_pushboolean(L, KEYBOARD->key_down(key));
	return 1;
}

static int lua_IsLeftDown(lua_State* L)
{
	lua_pushboolean(L, KEYBOARD->left_down());
	return 1;
}

static int lua_IsRightDown(lua_State* L)
{
	lua_pushboolean(L, KEYBOARD->right_down());
	return 1;
}

class XPXInstance
{
public:
	static int new_instance(lua_State* L)
	{
		XPX::String component_name = lua_tostring(L, 1);

		if (component_name == "Part")
		{
			if (lua_tostring(L, 2) &&
				lua_tostring(L, 3))
			{
				XPX::ComponentSystem::get_singleton_ptr()->add<XPX::PartComponent>(lua_tostring(L, 2), lua_tostring(L, 3));

				XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("return {}.{}", lua_tostring(L, 2), lua_tostring(L, 3)).c_str());
				return 1;
			}
		}
		else if (component_name == "Sound")
		{
			if (lua_tostring(L, 2) &&
				lua_tostring(L, 3))
			{
				XPX::ComponentSystem::get_singleton_ptr()->add<XPX::SoundComponent>(lua_tostring(L, 2),
					lua_tostring(L, 3));

				auto fmt = std::format("return {}.{}", lua_tostring(L, 2),
					lua_tostring(L, 3));

				XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

				return 1;
			}
		}
		else if (component_name == "Scene")
		{
			XPX::RoXML::RoXMLDocumentParameters params;

			params.Has3D = true;
			params.NoLua = true;
			params.LuaOnly = false;

			XPX::String generated_path = uuids::to_string(XPX::UUIDFactory::version<4>());
			generated_path += "-ROXML";

			XPLICIT_GET_DATA_DIR(path);
			path += "Contents/";
			path += lua_tostring(L, 3);

			if (!DownloadURL(lua_tostring(L, 2), generated_path))
			{
				lua_pushboolean(L, false);
				return 1;
			}

			params.Path = generated_path;

			if (params.Path.empty())
			{
				lua_pushboolean(L, false);
				return 1;
			}

			XPLICIT_PARSER.parse(params);

			lua_pushboolean(L, true);
			return 1;
		}

		lua_pushnil(L);
		return 1;
	}

};

void XplicitLoadClientLua() noexcept
{
	XPX::RLua::RuntimeClass<XPXInstance> instance;
	instance.begin_class("Instance", &XPXInstance::new_instance).end_class();

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_PlaySound, "XPXPlaySound");

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_MakeRect, "XPXMakeRectangle");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_KeyDown, "XPXKeyDown");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_IsKeyDown, "XPXIsKeyDown");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_IsLeftDown, "XPXIsLeftDown");

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_IsRightDown, "XPXIsRightDown");

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_GetY, "XPXGetY");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_GetX, "XPXGetX");
}
