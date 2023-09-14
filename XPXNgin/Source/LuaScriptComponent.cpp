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
			String("world").c_str(), (String("XPXScript") + std::to_string(xplicit_get_epoch())).c_str())
	{
		Thread job([](LuaScriptComponent* self) {
			self->insert("Destroy", self->destroy_snippet().c_str());
			self->insert("__gc", self->destroy_snippet().c_str());

			self->run_string(fmt::format("_G.Script.{} = {}", self->name(), String(self->parent()) + "." + self->name()).c_str());

			// Script.Current
			self->run_string(fmt::format("_G.Script.Current = _G.Script.{}", self->name()).c_str());

			//! ROBLOX(tm) like syntax
			self->run_string("_G.script = _G.Script.Current");

			if (auto err = self->run_path(self->mName.c_str());
				err)
			{
				XPLICIT_CRITICAL(err);
				ComponentSystem::get_singleton_ptr()->remove(self);
			}
			}, this);

		job.detach();
	}

	LuaScriptComponent::~LuaScriptComponent() = default;

	void LuaScriptComponent::update(void* class_ptr) {}

	bool LuaScriptComponent::should_update() noexcept { return false; }

	const char* LuaScriptComponent::path() noexcept { return mName.c_str(); }

	COMPONENT_TYPE LuaScriptComponent::type() noexcept { return COMPONENT_SCRIPT; }
}