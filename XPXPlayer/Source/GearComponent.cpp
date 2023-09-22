/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "GearComponent.h"

namespace XPX
{
	static String gear_destroy(const char* name, const char* parent) noexcept
	{
		String func_proto = fmt::format("function(self) destroyGear(\"{}\", \"{}\"); end", name, parent);
		return func_proto;
	}

	GearComponent::GearComponent(
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

	const char* GearComponent::name() noexcept { return "GearComponent"; }

	COMPONENT_TYPE GearComponent::type() noexcept { return COMPONENT_LOGIC; }

	PHYSICS_TYPE GearComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	GearComponent::~GearComponent() = default;

	MeshComponent* GearComponent::get_mesh() const noexcept { return mMeshPtr.get(); }

	bool GearComponent::should_update() noexcept { return true; }

	void GearComponent::update(ClassPtr class_ptr)
	{
		ClassComponent::update(class_ptr);

		GearComponent* self = (GearComponent*)class_ptr;

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
