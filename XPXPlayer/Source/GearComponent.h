/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "StaticBundleMesh.h"

#include <Util.h>
#include <Component.h>
#include <Enums.h>
#include <ClassComponent.h>

namespace XPX
{
	class GearComponent final : public ClassComponent
	{
	public:
		GearComponent() = delete;
		~GearComponent() override;

	public:
		XPLICIT_COPY_DEFAULT(GearComponent);

	public:
		const char* name() noexcept override;
		COMPONENT_TYPE type() noexcept override;
		PHYSICS_TYPE physics() noexcept override;
	
	public:
		static bool should_update() noexcept;
		static void update(void* class_ptr);

	public:
		explicit GearComponent(const char* name, 
			const char* mesh = nullptr,
			const char* parent = "world") noexcept;

	public:
		MeshComponent* get_mesh() const noexcept;

		const auto instance_name() noexcept { return mName; }
		const auto group_name() noexcept { return mParent; }

	private:
		std::unique_ptr<MeshComponent> mMeshPtr;

	private:
		String mParent;
		String mName;

	};
}