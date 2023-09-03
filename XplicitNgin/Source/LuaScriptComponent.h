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
			String func_proto = "function(self) World.ClassService.Destroy(";

			func_proto += "\"";
			func_proto += this->name();
			func_proto += "\"";
			func_proto += ",";
			func_proto += "\"";
			func_proto += "World";
			func_proto += "\"";
			func_proto += "); end";

			return func_proto;
		}

	public:
		explicit LuaScriptComponent(const char* name) 
			: mName(name), ClassComponent(
				std::string("World").c_str(), (std::string("CLS_") + std::to_string(xplicit_get_epoch())).c_str())
		{
			this->insert("Destroy", this->destroy_snippet().c_str());
			this->insert("ShouldRun", "false");
			this->insert("Run", "function(self) self.ShouldRun = true end");
		}

		~LuaScriptComponent() override = default;

	public:
		XPLICIT_COPY_DEFAULT(LuaScriptComponent);

	public:
		COMPONENT_TYPE type() noexcept;
		const char* name() noexcept;

	public:
		static void update(void* class_ptr);
		static bool should_update() noexcept;


	public:
		void run() noexcept;

	private:
		String mName;

	};
}