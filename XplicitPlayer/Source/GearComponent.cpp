/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

// The component itself 
#include "GearComponent.h"

namespace Xplicit::Player
{
	GearComponent::GearComponent(
		const char* name,
		const char* mesh, 
		const char* parent) noexcept
		:
		ClassComponent(Vector<float>(0, 0, 0), 
			Vector<float>(0, 0, 0),
			Color<float>(0, 0, 0),
			nullptr,
			parent,
			name),
		mMeshPtr(nullptr)
	{
		this->insert("Mesh", mesh ? mesh : "nil");

		if (mesh)
			mMeshPtr = std::make_unique<StaticMesh>(mesh, mName.c_str(), mParent.c_str());
	}

	const char* GearComponent::name() noexcept { return "GearComponent"; }

	COMPONENT_TYPE GearComponent::type() noexcept { return COMPONENT_LOGIC; }

	PHYSICS_TYPE GearComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	GearComponent::~GearComponent() = default;

	StaticMesh* GearComponent::get_mesh() const noexcept { return mMeshPtr.get(); }

	bool GearComponent::should_update() noexcept { return true; }

	void GearComponent::update(void* class_ptr)
	{
		ClassComponent::update(class_ptr);
		GearComponent* _this = (GearComponent*)class_ptr;

		String path = _this->index_as_string("Mesh");

		if (auto _path = _this->mMeshPtr->path();
			path != _path && !path.empty())
		{
			_this->mMeshPtr.reset();
			_this->mMeshPtr = std::make_unique<StaticMesh>(_path.c_str(), _this->mName.c_str(), _this->mParent.c_str());
		}
	}
}

#undef XPLICIT_LUA_GLOBAL