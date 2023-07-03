/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Component.h"
#include <lua/lua.hpp>

namespace Xplicit
{
	/*
	 * @brief Lua Script Instance.
	 * NOTE: only one LuaScriptComponent should be created. you don't need a lot of them.
	 */

	class XPLICIT_API LuaScriptComponent final : public Component
	{
	public:
		LuaScriptComponent() = delete;

	public:
		explicit LuaScriptComponent(const char* name, const bool run_now = false) 
			: Component(), mName(name)
		{
			if (run_now)
				this->run();
		}

		~LuaScriptComponent() override = default;

	public:
		XPLICIT_COPY_DEFAULT(LuaScriptComponent);

	public:
		COMPONENT_TYPE type() noexcept override;
		bool should_update() noexcept override;
		const char* name() noexcept override;

	public:
		void update() noexcept override;
		void run() noexcept;

	private:
		String mName;

	};
}