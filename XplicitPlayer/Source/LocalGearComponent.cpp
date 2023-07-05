/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

// The component itself 
#include "LocalGearComponent.h"

namespace Xplicit::Player
{
	GearComponent::GearComponent(
		const char* name,
		const char* mesh, 
		const char* parent) noexcept
		:
		InstanceComponent(Vector<float>(0, 0, 0), Vector<float>(0, 0, 0), Color<float>(0, 0, 0), nullptr, parent, name),
		mMeshPtr(nullptr)
	{
		XPLICIT_ASSERT(name && mesh);

		// Create Lua table from current object.
		String fmt = String(XPLICIT_LUA_GLOBAL);
		fmt += mParent;
		fmt += mName;
		fmt += " = { Mesh = '' }";

		Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

		mMeshPtr = std::make_unique<StaticMesh>(mesh, mName.c_str(), mParent.c_str());
	}

	const char* GearComponent::name() noexcept { return "GearComponent"; }

	COMPONENT_TYPE GearComponent::type() noexcept { return COMPONENT_LOGIC; }

	PHYSICS_TYPE GearComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	GearComponent::~GearComponent() = default;

	StaticMesh* GearComponent::get_mesh() const noexcept
	{
		return mMeshPtr.get();
	}

	bool GearComponent::should_update() noexcept { return true; }

	void GearComponent::update() noexcept
	{
		InstanceComponent::update();

		String fmt = std::format("{}{}{}.Mesh;", XPLICIT_LUA_GLOBAL, mParent, mName);

		lua_pushstring(Lua::XLuaStateManager::get_singleton_ptr()->state(), fmt.c_str());
		String path = lua_tostring(Lua::XLuaStateManager::get_singleton_ptr()->state(), -1);

		if (auto _path = mMeshPtr->path(); 
			path != _path)
		{
			mMeshPtr.reset();
			mMeshPtr = std::make_unique<StaticMesh>(_path.c_str(), mName.c_str(), mParent.c_str());

			lua_setglobal(Lua::XLuaStateManager::get_singleton_ptr()->state(), mMeshPtr->path().c_str());
		}

		if (this->get_attribute().script() &&
			this->get_attribute().script()->name() == "Update")
		{
			XPLICIT_INFO("GearComponent:Update()");
			this->get_attribute().script()->run();
		}
	}
}

#undef XPLICIT_LUA_GLOBAL