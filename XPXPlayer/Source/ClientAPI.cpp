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
#include "GearComponent.h"
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

struct XPXSoundCache final
{
	XPX::String name = "";
	XPX::SoundComponent* component = nullptr;
	XPX::String path = "";
};

std::vector<XPXSoundCache> XPX_CACHE;

static int lua_PlaySound(lua_State* L)
{
	try
	{
		auto name = lua_tostring(L, 1);
		auto url_raw = lua_tostring(L, 2);

		if (!name ||
			!url_raw)
		{
			return 0;
		}

		auto it = std::find_if(XPX_CACHE.cbegin(), XPX_CACHE.cend(), [&](const XPXSoundCache cache) {
			return cache.name == name;
		});

		XPX::String url = url_raw;

		if (it == XPX_CACHE.cend())
		{
			// that means if we don't find it, then fail silently.
			if (url.find(".wav") == XPX::String::npos)
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

			auto tmp = uuids::to_string(XPX::UUIDFactory::version<4>()) + "-SND.wav";

			if (monitor &&
				monitor->HTTP->download(url, tmp))
			{
				XPLICIT_GET_DATA_DIR(full_path);

				XPX::String full_download_path;

				full_download_path += full_path;
				full_download_path += "Contents/";
				full_download_path += tmp;

				if (auto snd = XPX::ComponentSystem::get_singleton_ptr()->get<XPX::SoundComponent>(name);
					snd)
				{
					snd->play(full_download_path);

					XPXSoundCache cache;
					cache.component = snd;
					cache.path = full_download_path;
					cache.name = name;

					XPX_CACHE.push_back(cache);
				}
				else
				{
					std::remove(full_download_path.c_str());
				}
			}
			else
			{
				if (it->component)
				{
					it->component->play(it->path);
				}
			}

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
				XPX::String key = lua_tostring(L, 2);
				XPX::String value = lua_tostring(L, 3);

				XPX::ComponentSystem::get_singleton_ptr()->add<XPX::PartComponent>(value.c_str(), key.c_str());
				XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("return {}.{}", key, value).c_str());

				return 1;
			}
		}
		else if (component_name == "Sound")
		{
			if (lua_tostring(L, 2) &&
				lua_tostring(L, 3))
			{
				XPX::String key = lua_tostring(L, 2);
				XPX::String value = lua_tostring(L, 3);

				XPX::ComponentSystem::get_singleton_ptr()->add<XPX::SoundComponent>(value.c_str(), key.c_str());
				XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("return {}.{}", key, value).c_str());

				return 1;
			}
		}
		else if (component_name == "Scene")
		{
			XPX::RoXML::RoXMLDocumentParameters params;

			params.Has3D = true;
			params.NoLua = true;
			params.WaitFor = false;
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

static int lua_DestroyPart(lua_State* L)
{
	auto name = lua_tostring(L, 1);
	auto parent = lua_tostring(L, 2);

	if (name && parent)
	{
		XPX::String name_str = name;
		XPX::String parent_str = parent;

		XPX::Thread job([](XPX::String name, XPX::String parent) {
			auto all_of_parts = XPX::ComponentSystem::get_singleton_ptr()->all_of<XPX::PartComponent>("PartComponent");

			for (auto& part : all_of_parts)
			{
				if (part)
				{
					if (part->name() == name &&
						part->parent() == parent)
					{
						XPX::ComponentSystem::get_singleton_ptr()->remove(part);
						return;
					}
				}
			}

			XPLICIT_INFO(name + " has not been found on parent: " + parent + ".");
		}, name_str, parent_str);

		job.detach();
	}

	return 0;
}

static int lua_DestroyGear(lua_State* L)
{
	auto name = lua_tostring(L, 1);
	auto parent = lua_tostring(L, 2);

	if (name && parent)
	{
		XPX::String name_str = name;
		XPX::String parent_str = parent;

		XPX::Thread job([](XPX::String name, XPX::String parent) {
			auto all_of_parts = XPX::ComponentSystem::get_singleton_ptr()->all_of<XPX::GearComponent>("GearComponent");

			for (auto& part : all_of_parts)
			{
				if (part)
				{
					if (part->instance_name() == name &&
						part->group_name() == parent)
					{
						XPX::ComponentSystem::get_singleton_ptr()->remove(part);
						return;
					}
				}
			}

			XPLICIT_INFO(name + " has not been found on parent: " + parent + ".");
			}, name_str, parent_str);

		job.detach();
	}

	return 0;
}

static int lua_DestroySound(lua_State* L)
{
	auto name = lua_tostring(L, 1);
	auto parent = lua_tostring(L, 2);

	if (name && parent)
	{
		XPX::String name_str = name;
		XPX::String parent_str = parent;

		XPX::Thread job([](XPX::String name, XPX::String parent) {
			auto all_of_parts = XPX::ComponentSystem::get_singleton_ptr()->all_of<XPX::SoundComponent>(name.c_str());

			for (auto& part : all_of_parts)
			{
				if (part)
				{
					if (part->instance_name() == name &&
						part->group_name() == parent)
					{
						XPX::ComponentSystem::get_singleton_ptr()->remove(part);
						return;
					}
				}
			}

			XPLICIT_INFO(name + " has not been found on parent: " + parent + ".");
			}, name_str, parent_str);

		job.detach();
	}

	return 0;
}

static int lua_DestroyMesh(lua_State* L)
{
	auto name = lua_tostring(L, 1);
	auto parent = lua_tostring(L, 2);

	if (name && parent)
	{
		XPX::String name_str = name;
		XPX::String parent_str = parent;

		XPX::Thread job([](XPX::String name, XPX::String parent) {
			auto all_of_parts = XPX::ComponentSystem::get_singleton_ptr()->all_of<XPX::MeshComponent>("MeshComponent");

			for (auto& part : all_of_parts)
			{
				if (part)
				{
					if (part->instance_name() == name &&
						part->group_name() == parent)
					{
						XPX::ComponentSystem::get_singleton_ptr()->remove(part);
						return;
					}
				}
			}

			XPLICIT_INFO(name + " has not been found on parent: " + parent + ".");
			}, name_str, parent_str);

		job.detach();
	}

	return 0;
}

void XplicitLoadClientLua() noexcept
{
	XPX::RLua::RuntimeClass<XPXInstance> instance;
	instance.begin_class("Class", &XPXInstance::new_instance).end_class();

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_PlaySound, "playSound");

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_KeyDown, "someKeyDown");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_IsKeyDown, "isKeyDown");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_IsLeftDown, "isLeftMouseDown");

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_IsRightDown, "isRightMouseDown");

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroySound, "destroySound");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroyGear, "destroyGear");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroyMesh, "destroyMesh");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroyPart, "destroyPart");

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_GetY, "getMouseY");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_GetX, "getMouseX");
}
