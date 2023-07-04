/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalToolComponent.h"
#include <InstanceComponent.h>
#include <lua/lua.hpp>

namespace Xplicit::Player
{
	LocalToolComponent::LocalToolComponent(
		const char* name,
		const char* mesh, 
		const char* parent) noexcept
		: 
		mMeshPtr(nullptr),
		mName(name),
		mParent(parent)
	{
		XPLICIT_ASSERT(name && mesh);

		// Create Lua table for current object.

		String fmt = String(XPLICIT_LUA_GLOBAL);
		fmt += mParent;
		fmt += mName;
		fmt += " = { Mesh = '' }";

		Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

		mMeshPtr = std::make_unique<StaticMesh>(mesh, mName.c_str(), mParent.c_str());
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
		String fmt = std::format("{}{}{}.Mesh;", XPLICIT_LUA_GLOBAL, mParent, mName);

		lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(), fmt.c_str());
		String path = lua_tostring(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

		if (auto _path = mMeshPtr->path(); path != _path)
		{
			mMeshPtr.reset();
			mMeshPtr = std::make_unique<StaticMesh>(_path.c_str(), mName.c_str(), mParent.c_str());

			lua_setglobal(Lua::XLuaStateManager::get_singleton_ptr()->state(), mMeshPtr->path().c_str());
		}

		if (this->get_attribute().script() &&
			this->get_attribute().script()->name() == "Update")
		{
			XPLICIT_INFO("LocalToolComponent:Update()");
			this->get_attribute().script()->run();
		}
	}
}

#undef XPLICIT_LUA_GLOBAL