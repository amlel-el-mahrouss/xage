/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ClassComponent.h"

namespace Xplicit
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
		const String destroy_snippet() noexcept
		{
			String func_proto = std::format("function(self) World.ClassService.Destroy(\"{}\", \"{}\"); end", 
				this->name(), "World");

			return func_proto;
		}

	public:
		explicit LuaScriptComponent(const char* name);
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
		void run() noexcept;

	private:
		String mName;

	};
}