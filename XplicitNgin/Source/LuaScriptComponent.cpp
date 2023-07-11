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

	void LuaScriptComponent::update(void* class_ptr) {  }

	bool LuaScriptComponent::should_update() noexcept { return false; }

	const char* LuaScriptComponent::name() noexcept { return mName.c_str(); }

	COMPONENT_TYPE LuaScriptComponent::type() noexcept { return COMPONENT_SCRIPT; }
}