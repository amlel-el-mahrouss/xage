/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalNetworkMonitorEvent.h"

#include "SoundComponent.h"
#include "GearComponent.h"
#include "PartComponent.h"
#include "MeshComponent.h"
#include "ClientFX.h"
#include "MenuUI.h"
#include "App.h"

#include <SoundNgin.h>
#include <CLua.hpp>
#include <LuaAPI.h>
#include <codecvt>
#include <RoXML.h>
#include <Util.h>
#include <Uri.h>

// RoXML parser
static XPX::RoXML::RoXMLDocumentParser XPLICIT_PARSER;

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

static std::vector<XPXSoundCache> XPX_CACHE;
static std::condition_variable XPLICIT_CV;

static int lua_PlaySound(lua_State* L)
{
	try
	{
		auto name = lua_tostring(L, 1);
		auto url_raw = lua_tostring(L, 2);

		if (!name ||
			!url_raw)
		{
			XPLICIT_CV.notify_one();

			lua_pushboolean(L, false);
			return 1;
		}

		auto it = std::find_if(XPX_CACHE.cbegin(), XPX_CACHE.cend(), [&](const XPXSoundCache cache) {
			return cache.name == name;
		});

		XPX::String url = url_raw;

		if (it == XPX_CACHE.cend())
		{
			if (url.find(".wav") == XPX::String::npos)
			{
				XPLICIT_CV.notify_one();

				lua_pushboolean(L, false);
				return 1;
			}

			if (url.empty() ||
				url.find(XPLICIT_XASSET_IDENT) == XPX::String::npos)
			{
				XPLICIT_CV.notify_one();

				lua_pushboolean(L, false);
				return 1;
			}

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

	XPLICIT_CV.notify_one();

	lua_pushboolean(L, true);
	return 1;
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

class XPXEngineBridge
{
public:
	static const char* name() { return "Engine"; }

public:
	static int new_instance(lua_State* L)
	{
		XPX::String component_name = lua_tostring(L, 1);

		if (component_name == "Sound")
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
		else if (component_name == "RoXML")
		{
			XPX::Thread job([&]() {
				XPX::RoXML::RoXMLDocumentParameters params;

				params.Has3D = false;
				params.NoLua = false;
				params.WaitFor = true;
				params.LuaOnly = false;

				XPX::String generated_path = uuids::to_string(XPX::UUIDFactory::version<4>());
				generated_path += "-XMLSCENE";

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

				for (auto& additional : params.WorldNodes)
				{
					if (additional.Name == "Sound")
					{
						XPX::ComponentSystem::get_singleton_ptr()->add<XPX::SoundComponent>(additional.ID.c_str(), additional.Value.c_str());
					}
				}

				DVFromRoXML(params);
			});

			job.detach();

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
			auto all_of_parts = XPX::ComponentSystem::get_singleton_ptr()->all_of<XPX::PartComponent>();

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
			auto all_of_parts = XPX::ComponentSystem::get_singleton_ptr()->all_of<XPX::GearComponent>();

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
			auto part = XPX::ComponentSystem::get_singleton_ptr()->get<XPX::SoundComponent>(name.c_str());

			if (part)
			{
				if (part->instance_name() == name &&
					part->group_name() == parent)
				{
					XPX::ComponentSystem::get_singleton_ptr()->remove(part);
					return;
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
			auto all_of_parts = XPX::ComponentSystem::get_singleton_ptr()->all_of<XPX::MeshComponent>();

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

static int from_scheme(lua_State* L)
{
	auto uri = *reinterpret_cast<XPXUri**>(lua_newuserdata(L, sizeof(XPXUri*))) = new XPXUri();
	uri->Uri = XPX::Utils::UriParser(lua_tostring(L, 1));

	XPLICIT_INFO("XPXUri::on_new");

	return 1;
}

void XplicitLoadClientLua() noexcept
{
	XPX::RLua::RuntimeClass<XPXEngineBridge> instance;
	instance.begin_class().append_proc("new", &XPXEngineBridge::new_instance).end_class();

	lua_setglobal(XPX::Lua::CLuaStateManager::get_singleton_ptr()->state(), XPXEngineBridge::name());

	XPX::RLua::RuntimeClass<XPXUri> uri;
	uri.begin_class().append_proc("from_scheme", from_scheme).append_proc("parse", &XPXUri::parse_url).end_class();

	lua_setglobal(XPX::Lua::CLuaStateManager::get_singleton_ptr()->state(), XPXUri::name());

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
