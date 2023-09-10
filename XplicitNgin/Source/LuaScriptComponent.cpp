/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#include "LuaScriptComponent.h"

namespace XPX
{
	const String LuaScriptComponent::destroy_snippet() noexcept
	{
		String func_proto("function(self) XPXDestroyScript(self.Name); end");
		return func_proto;
	}

	LuaScriptComponent::LuaScriptComponent(const char* name)
		: mName(name), ClassComponent(
			std::string("World").c_str(), (String("XPXScript") + std::to_string(xplicit_get_epoch())).c_str())
	{
		this->insert("Destroy", this->destroy_snippet().c_str());

		this->run_string(std::format("_G.Script.{} = {}", this->name(), String(this->parent()) + "." + this->name()).c_str());

		// Script.Current
		this->run_string(std::format("_G.Script.Current = _G.Script.{}", this->name()).c_str());

		//! ROBLOX(tm) like syntax
		this->run_string("_G.script = _G.Script.Current");

		this->run_path(this->mName.c_str());
	}

	LuaScriptComponent::~LuaScriptComponent() = default;

	void LuaScriptComponent::update(void* class_ptr) {}

	bool LuaScriptComponent::should_update() noexcept { return false; }

	const char* LuaScriptComponent::path() noexcept { return mName.c_str(); }

	COMPONENT_TYPE LuaScriptComponent::type() noexcept { return COMPONENT_SCRIPT; }
}