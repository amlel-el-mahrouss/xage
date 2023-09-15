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
			"world", ("XPXRuntimeScript_" + std::to_string(xplicit_get_epoch())).c_str())
	{
		this->insert("Destroy", this->destroy_snippet());

		// Script.Current
		this->run_string(fmt::format("_G.Script.Current = _G.Script.{}", this->name()));

		//! ROBLOX(tm) like syntax
		this->run_string("_G.script = _G.Script.Current");

		if (auto err = this->run_path(this->mName);
			!err.empty())
		{
			XPLICIT_CRITICAL(err);
			ComponentSystem::get_singleton_ptr()->remove(this);
		}
	}

	LuaScriptComponent::~LuaScriptComponent() = default;

	void LuaScriptComponent::update(void* class_ptr) {}

	bool LuaScriptComponent::should_update() noexcept { return false; }

	const char* LuaScriptComponent::path() noexcept { return mName.c_str(); }

	COMPONENT_TYPE LuaScriptComponent::type() noexcept { return COMPONENT_SCRIPT; }
}