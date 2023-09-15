/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LuaScriptComponent.h"
#include "LuaUser.h"

namespace XPX
{
	const String LuaScriptComponent::destroy_snippet() noexcept
	{
		String func_proto("function(self) destroyScript(self.Name); end");
		return func_proto;
	}

	LuaScriptComponent::LuaScriptComponent(const char* name)
		: mName(name), ClassComponent(
			"_G.Script", ("XPXRuntimeScript_" + std::to_string(xplicit_get_epoch())).c_str())
	{
		for (size_t i = 0; i < mName.size(); i++)
		{
			if (mName[i] == '\\')
			{
				mName[i] = '/';
			}
		}

		this->insert("FilesystemPath", fmt::format("'{}'", mName));

		this->insert("Destroy", this->destroy_snippet());

		XPX::Thread job([&]() {
			String name = this->name();
			String path = this->path();

			auto lua_ThisSleep = [](lua_State* L) -> int {
				std::this_thread::sleep_for(std::chrono::seconds(lua_tointeger(L, 1)));
				return 0;
			};

			// Script.Current
			XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("Script.Current = Script.{}", name));

			// ROBLOX(tm) like syntax
			XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string("script = Script.Current");

			XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_ThisSleep, "script.wait");

			clua_lock();

			if (XPX::Lua::CLuaStateManager::get_singleton_ptr()->run(path) != LUA_OK)
			{
				if (lua_isstring(XPX::Lua::CLuaStateManager::get_singleton_ptr()->state(), -1))
					XPLICIT_CRITICAL(lua_tostring(XPX::Lua::CLuaStateManager::get_singleton_ptr()->state(), -1));

				clua_unlock();

				ComponentSystem::get_singleton_ptr()->remove(this);

				return;
			}

			clua_unlock();
		});

		job.detach();
	}

	LuaScriptComponent::~LuaScriptComponent() = default;

	void LuaScriptComponent::update(void* class_ptr) {}

	bool LuaScriptComponent::should_update() noexcept { return false; }

	const char* LuaScriptComponent::path() noexcept { return mName.c_str(); }

	COMPONENT_TYPE LuaScriptComponent::type() noexcept { return COMPONENT_SCRIPT; }
}