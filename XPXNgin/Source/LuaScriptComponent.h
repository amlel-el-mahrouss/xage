/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ClassComponent.h"

namespace XPX
{
	/*
	 * @brief Lua Script Instance.
	 * NOTE: only one LuaScriptComponent should be created. you don't need a lot of them.
	 */

	class XPLICIT_API LuaScriptComponent final : public ClassComponent
	{
	public:
		enum
		{
			LUA_LOADING,
			LUA_RUNNING,
			LUA_STOP,
		};

	public:
		LuaScriptComponent() = delete;

	private:
		const String destroy_snippet() noexcept;

	public:
		explicit LuaScriptComponent(const char* name);
		~LuaScriptComponent() override;

	public:
		XPLICIT_COPY_DEFAULT(LuaScriptComponent);

	public:
		COMPONENT_TYPE type() noexcept;
		const char* path() noexcept;
		void run_script();
		std::int32_t status() noexcept;

	public:
		static void update(void* class_ptr);
		static bool should_update() noexcept;

	private:
		String mName;
		std::int32_t mStatus;

	};
}