/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LuaScriptComponent.h"

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

		this->insert("UniqueID", fmt::format("'{}'", mName));

		this->insert("Destroy", this->destroy_snippet());

		// Script.Current
		this->run_string(fmt::format("Script.Current = Script.{}", this->name()));

		// ROBLOX(tm) like syntax
		this->run_string("script = Script.Current");

		mRunningLua = Thread([&]() {
			if (auto err = this->run_path(this->mName);
				!err.empty())
			{
				XPLICIT_CRITICAL(err);
				ComponentSystem::get_singleton_ptr()->remove(this);

				return;
			}

			mRunningLua.detach();
		});
	}

	LuaScriptComponent::~LuaScriptComponent() = default;

	void LuaScriptComponent::update(void* class_ptr) {}

	bool LuaScriptComponent::should_update() noexcept { return false; }

	const char* LuaScriptComponent::path() noexcept { return mName.c_str(); }

	Thread& LuaScriptComponent::leak_thread() noexcept { return mRunningLua; }

	COMPONENT_TYPE LuaScriptComponent::type() noexcept { return COMPONENT_SCRIPT; }
}