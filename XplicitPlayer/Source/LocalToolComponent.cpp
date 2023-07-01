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

		String fmt = "_G.";
		fmt += parent;
		fmt += name;
		fmt += " = {}";

		Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

		mMeshPtr = std::make_unique<StaticMesh>(mesh);
	}

	const char* LocalToolComponent::name() noexcept { return "LocalToolComponent"; }
	COMPONENT_TYPE LocalToolComponent::type() noexcept { return COMPONENT_LOGIC; }
	bool LocalToolComponent::should_update() noexcept { return false; }
	PHYSICS_TYPE LocalToolComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	LocalToolComponent::~LocalToolComponent()
	{
		String fmt = "_G.";
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
}