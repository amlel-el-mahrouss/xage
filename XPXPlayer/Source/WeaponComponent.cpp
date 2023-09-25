/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "WeaponComponent.h"

namespace XPX
{
	static String gear_destroy(const char* name, const char* parent) noexcept
	{
		String func_proto = fmt::format("function(self) destroyGear(\"{}\", \"{}\"); end", name, parent);
		return func_proto;
	}

	WeaponComponent::WeaponComponent(
		const char* name,
		const char* mesh, 
		const char* parent) noexcept
		:
		ClassComponent(Vector<NetworkFloat>(0, 0, 0),
			Vector<NetworkFloat>(0, 0, 0),
			Color<NetworkFloat>(0, 0, 0),
			nullptr,
			parent,
			name),
		mMeshPtr(nullptr)
	{
		this->insert("Path", mesh ? mesh : "nil");

		this->insert("Destroy", gear_destroy(mName.c_str(), mParent.c_str()).c_str());

		if (mesh)
			mMeshPtr = std::make_unique<MeshComponent>(mesh, mName.c_str(), mParent.c_str());
	}

	const char* WeaponComponent::name() noexcept { return "WeaponComponent"; }

	COMPONENT_TYPE WeaponComponent::type() noexcept { return COMPONENT_LOGIC; }

	PHYSICS_TYPE WeaponComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	WeaponComponent::~WeaponComponent() = default;

	MeshComponent* WeaponComponent::get_mesh() const noexcept { return mMeshPtr.get(); }

	bool WeaponComponent::should_update() noexcept { return true; }

	void WeaponComponent::update(ClassPtr class_ptr)
	{
		ClassComponent::update(class_ptr);

		WeaponComponent* self = (WeaponComponent*)class_ptr;

		String path = self->index_as_string("Path");

		if (!path.empty())
		{
			if (auto mesh_path = self->mMeshPtr->path();
				path != mesh_path)
			{
				self->mMeshPtr.reset();
				self->mMeshPtr = std::make_unique<MeshComponent>(path.c_str(), self->mName.c_str(), self->mParent.c_str());
			}
		}
	}
}
