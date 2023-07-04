/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "InstanceComponent.h"

namespace Xplicit
{
	InstanceComponent::InstanceComponent(
		const Vector<float>& position, 
		const Vector<float>& size, 
		const Color<float>& color,
		const char* script,
		const char* parent,
		const char* name)
		: 
		mName(name),
		mParent(parent)
	{
		mAttribute.pos() = position;
		mAttribute.scale() = size;
		mAttribute.color() = color;

		if (script)
		{
			mAttribute.script(ComponentManager::get_singleton_ptr()->add<LuaScriptComponent>(script, true));
			XPLICIT_ASSERT(mAttribute.script());
		}

		String fmt = XPLICIT_LUA_GLOBAL;

		if (!mParent.empty())
		{
			fmt += ".";
			fmt += mParent;
		}

		fmt += ".";
		fmt += mName;
		fmt += " = { Size = { X = 0, Y = 0, Z = 0 }, Position = { X = 0, Y = 0, Z = 0 }, Color = { R = 0, G = 0, B = 0 } }";

		Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
	}

	InstanceComponent::~InstanceComponent()
	{
		String fmt = XPLICIT_LUA_GLOBAL;

		if (!mParent.empty())
		{
			fmt += ".";
			fmt += mParent;
		}

		fmt += ".";
		fmt += mName;
		fmt += " = nil";

		Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
	}

	const char* InstanceComponent::parent() noexcept { return mParent.c_str(); }

	bool InstanceComponent::should_update() noexcept { return true; }
	
	void InstanceComponent::update()
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

	XAttribute& InstanceComponent::get_attribute() noexcept { return mAttribute; }
}