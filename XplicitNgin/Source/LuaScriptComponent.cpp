/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "LuaScriptComponent.h"

namespace Xplicit
{
	void LuaScriptComponent::run() noexcept
	{
		if (!this->mName.empty())
		{
			Lua::CLuaStateManager::get_singleton_ptr()->run(this->mName.c_str());
		}
	}

	void LuaScriptComponent::update(void* class_ptr) 
	{
		LuaScriptComponent* comp = (LuaScriptComponent*)class_ptr;

		if (comp->index_as_bool("ShouldRun"))
		{
			comp->insert("Destroy", comp->destroy_snippet().c_str());
			comp->insert("ShouldRun", "false");
			comp->insert("Run", "function(self) self.ShouldRun = true end");


			Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.Script.{} = {}", comp->name(), "{}").c_str());
			Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.Script.{}.Parent = {}", comp->name(), std::format("_G.{}.{}", comp->parent(), comp->name())).c_str());

			// Script.Current
			Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.Script.Current = {}{}", "_G.Script.", comp->name()).c_str());

			//! ROBLOX(tm) like syntax
			Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.script = {}{}", "_G.Script.", comp->name()).c_str());

			comp->run();

			if (comp->index_as_bool("Archivable"))
			{
				comp->call("Destroy");
			}
		}
	}

	bool LuaScriptComponent::should_update() noexcept { return true; }

	const char* LuaScriptComponent::path() noexcept { return mName.c_str(); }

	COMPONENT_TYPE LuaScriptComponent::type() noexcept { return COMPONENT_SCRIPT; }
}