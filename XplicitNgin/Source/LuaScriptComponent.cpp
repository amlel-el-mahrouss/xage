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
	LuaScriptComponent::LuaScriptComponent(const char* name)
		: mName(name), ClassComponent(
			std::string("World").c_str(), (std::string("XPXLuaScript") + std::to_string(xplicit_get_epoch())).c_str())
	{
		this->insert("Destroy", this->destroy_snippet().c_str());

		Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.Script.{} = {}", this->name(), String(this->parent()) + "." + this->name()).c_str());

		// Script.Current
		Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.Script.Current = _G.Script.{}", this->name()).c_str());

		//! ROBLOX(tm) like syntax
		Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.script = _G.Script.Current");
		
		this->run();
	}

	LuaScriptComponent::~LuaScriptComponent() = default;

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

		if (comp->index_as_bool("__Destroy"))
		{
			ComponentSystem::get_singleton_ptr()->remove(comp);
		}
	}

	bool LuaScriptComponent::should_update() noexcept { return true; }

	const char* LuaScriptComponent::path() noexcept { return mName.c_str(); }

	COMPONENT_TYPE LuaScriptComponent::type() noexcept { return COMPONENT_SCRIPT; }
}