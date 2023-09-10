/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#include "LuaEventListener.h"

namespace XPX
{
	void LuaEventListener::update(EventPtr _event)
	{
		if (!_event)
			return;

		String fmt = std::format("{}()", this->mSnippet);
		Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
	}

	const char* LuaEventListener::name() noexcept { return "LuaEventListener"; }
}