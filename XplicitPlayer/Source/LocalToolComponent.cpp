/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalToolComponent.h"
#include <lua/lua.hpp>

#define XPLICIT_LUA_GLOBAL "_G."

namespace Xplicit::Player
{
	LocalToolComponent::LocalToolComponent(const char* name, const char* mesh, const char* parent) noexcept
		: 
		mMeshPtr(nullptr),
		mName(name),
		mParent(parent)
	{
		XPLICIT_ASSERT(name && mesh);

		// Create Lua table for current object.

		String fmt = XPLICIT_LUA_GLOBAL;
		fmt += parent;
		fmt += name;
		fmt += " = {}";

		Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

		mMeshPtr = std::make_unique<StaticMesh>(mesh);
	}

	const char* LocalToolComponent::name() noexcept { return "LocalToolComponent"; }
	COMPONENT_TYPE LocalToolComponent::type() noexcept { return COMPONENT_LOGIC; }
	PHYSICS_TYPE LocalToolComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	LocalToolComponent::~LocalToolComponent()
	{
		String fmt = XPLICIT_LUA_GLOBAL;
		fmt += mParent;
		fmt += mName;
		fmt += " = nil";

		Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
	}

	StaticMesh* LocalToolComponent::get_mesh() const noexcept
	{
		return mMeshPtr.get();
	}

	XAttribute& LocalToolComponent::get_attribute() noexcept
	{
		return mAttribute;
	}

	bool LocalToolComponent::should_update() noexcept { return true; }

	void LocalToolComponent::update() noexcept
	{
		String fmt = XPLICIT_LUA_GLOBAL;
		fmt += mParent;
		fmt += mName;
		fmt += ".Mesh";

		lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(), fmt.c_str());
		String path = lua_tostring(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

		if (path != mMeshPtr->path())
		{
			mMeshPtr.reset();
			mMeshPtr = std::make_unique<StaticMesh>(mMeshPtr->path());

			lua_setglobal(Lua::XLuaStateManager::get_singleton_ptr()->state(), mMeshPtr->path().c_str());
		}

		if (this->get_attribute().script() &&
			this->get_attribute().script()->name() == "Update")
			this->get_attribute().script()->run();

	}
}

#undef XPLICIT_LUA_GLOBAL