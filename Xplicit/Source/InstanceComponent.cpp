/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "InstanceComponent.h"

#define XPLICIT_LUA_GLOBAL "_G."
#define XPLICIT_LUA_NAMESPACE "Game."

namespace Xplicit
{
	InstanceComponent::InstanceComponent(
		const Vector<float>& position, 
		const Vector<float>& size, 
		const Color<float>& color,
		const char* script,
		const char* parent,
		const char* name,
		const bool read_only)
		: 
		mName(name),
		mParent(parent),
		mReadOnly(read_only)
	{
		mAttribute.pos() = position;
		mAttribute.scale() = size;
		mAttribute.color() = color;

		if (script)
		{
			mAttribute.script(ComponentManager::get_singleton_ptr()->add<LuaScriptComponent>(script, true));
			XPLICIT_ASSERT(mAttribute.script());
		}
	}

	InstanceComponent::~InstanceComponent()
	{
		String fmt = XPLICIT_LUA_NAMESPACE;
		fmt += mParent;
		fmt += ".";
		fmt += mName;
		
		Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
	}

	const Xplicit::String& InstanceComponent::parent() noexcept { return mParent; }

	bool InstanceComponent::should_update() noexcept { return true; }
	
	void InstanceComponent::update()
	{
		/*
		 *
		 * ReadOnly means that CLua can't edit this instance, only C++ can.
		 * This can be used for FFI.
		 *
		 */

		if (this->mReadOnly)
		{
			static const char* pos[] = { "X", "Y", "Z" };
			const float pos_raw[] = { mAttribute.pos().X, mAttribute.pos().Y, mAttribute.pos().Z };

			for (size_t i = 0; i < 3; i++)
			{
				String fmt = XPLICIT_LUA_GLOBAL;

				fmt += XPLICIT_LUA_NAMESPACE;
				fmt += mParent;
				fmt += ".";
				fmt += mName;
				fmt += ".Position.";
				fmt += pos[i];
				fmt += "= " + std::to_string(pos_raw[i]) + ";";

				Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
			}

			static const char* scale[] = { "X", "Y", "Z" };
			const float scale_raw[] = { mAttribute.scale().X, mAttribute.scale().Y, mAttribute.scale().Z };

			for (size_t i = 0; i < 3; i++)
			{
				String fmt = XPLICIT_LUA_GLOBAL;

				fmt += XPLICIT_LUA_NAMESPACE;
				fmt += mParent;
				fmt += ".";
				fmt += mName;
				fmt += ".Size.";
				fmt += pos[i];
				fmt += "= " + std::to_string(scale_raw[i]) + ";";

				Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
			}

			static const char* clr[] = { "R", "G", "B" };
			const float clr_raw[] = { mAttribute.color().R, mAttribute.color().G, mAttribute.color().B };

			for (size_t i = 0; i < 3; i++)
			{
				String fmt = XPLICIT_LUA_GLOBAL;

				fmt += XPLICIT_LUA_NAMESPACE;
				fmt += mParent;
				fmt += ".";
				fmt += mName;
				fmt += ".Color.";
				fmt += clr[i];
				fmt += "= " + std::to_string(clr_raw[i]) + ";";

				Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
			}

			String fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mParent;
			fmt += ".";
			fmt += mName;
			fmt += ".Archivable = ";
			fmt += this->get_attribute().is_archivable() ? "true" : "false";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

			fmt.clear();

			fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mParent;
			fmt += ".";
			fmt += mName;
			fmt += ".Locked = ";
			fmt += this->get_attribute().is_locked() ? "true" : "false";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

			fmt.clear();

			fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mParent;
			fmt += ".";
			fmt += mName;
			fmt += ".Anchor = ";
			fmt += this->get_attribute().is_anchored() ? "true" : "false";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

			fmt.clear();

			fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mParent;
			fmt += ".";
			fmt += mName;
			fmt += ".Collide = ";
			fmt += this->get_attribute().has_no_collide() ? "false" : "true";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

			fmt.clear();

			fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mParent;
			fmt += ".";
			fmt += mName;
			fmt += ".Alpha = ";
			fmt += std::to_string(this->get_attribute().alpha());

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}
		else
		{
			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(), 
				std::format("{}{}{}.Alpha;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().alpha(lua_tonumber(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

			// Anchored?
			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
				std::format("{}{}{}.Anchor;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().anchor(lua_toboolean(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

			// Locked?
			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
				std::format("{}{}{}.Lock;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().locked(lua_toboolean(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

			// Archived?
			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
				std::format("{}{}{}.Archivable;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().archivable(lua_toboolean(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

			// Colliding?
			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
				std::format("{}{}{}.Collide;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().collide(lua_toboolean(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

			// Pos X?
			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
				std::format("{}{}{}.Position.X;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().pos().X = (lua_tonumber(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

			// Pos Y?
			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
				std::format("{}{}{}.Position.Y;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().pos().Y = (lua_tonumber(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

			// Pos Z?
			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
				std::format("{}{}{}.Position.Z;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().pos().Z = (lua_tonumber(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

			// Color (red)?
			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
				std::format("{}{}{}.Color.R;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().color().R = (lua_tonumber(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

			// Color (green)?
			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
				std::format("{}{}{}.Color.G;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().color().G = (lua_tonumber(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

			// Color (blue)?
			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
				std::format("{}{}{}.Color.B;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().color().B = (lua_tonumber(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

			// Size X?
			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
				std::format("{}{}{}.Size.X;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().scale().X = (lua_tonumber(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

			// Size Y?
			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
				std::format("{}{}{}.Size.Y;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().scale().Y = (lua_tonumber(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

			// Size Z?
			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
				std::format("{}{}{}.Size.Z;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().scale().Z = (lua_tonumber(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

			lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
				std::format("{}{}{}.Alpha;", XPLICIT_LUA_GLOBAL, mParent, mName).c_str());

			get_attribute().alpha((lua_tonumber(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1)));

			lua_pop(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);
		}
	}

	XAttribute& InstanceComponent::get_attribute() noexcept { return mAttribute; }
}