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
		LuaScriptComponent() = delete;

	private:
		const String destroy_snippet() noexcept;

	public:
		LuaScriptComponent(const char* name, bool multi_threaded = true);
		~LuaScriptComponent() override;

	public:
		XPLICIT_COPY_DEFAULT(LuaScriptComponent);

	public:
		COMPONENT_TYPE type() noexcept;
		const char* path() noexcept;

	public:
		static void update(void* class_ptr);
		static bool should_update() noexcept;

	private:
		String mName;

	};
}