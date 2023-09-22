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

		Thread job;

		job = Thread([&]() {
			clua_lock();

			auto self = this;

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

			XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_ThisSleep, "wait");

			XPX::String code;

			std::ifstream if_path(path);
			if_path.seekg(SEEK_END);

			auto len = if_path.tellg();

			if_path.seekg(SEEK_SET);

			code.reserve(len);

			XPX::String tmp = "";

			bool comment = false;

			mStatus = LUA_LOADING;

			while (std::getline(if_path, tmp))
				code += tmp + "\r\n";

			clua_lock();

			mStatus = LUA_RUNNING;

			if (auto ret = self->run_string(code);
				!ret.empty())
			{
				XPLICIT_CRITICAL(ret);
			}

			mStatus = LUA_STOP;

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