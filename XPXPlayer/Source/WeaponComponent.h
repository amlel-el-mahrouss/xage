/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "MeshComponent.h"

#include <Util.h>
#include <Enums.h>
#include <ClassComponent.h>

namespace XPX
{
	class WeaponComponent final : public ClassComponent
	{
	public:
		WeaponComponent() = delete;
		~WeaponComponent() override;

	public:
		XPLICIT_COPY_DEFAULT(WeaponComponent);

	public:
		const char* name() noexcept override;
		COMPONENT_TYPE type() noexcept override;
		PHYSICS_TYPE physics() noexcept override;
	
	public:
		static bool should_update() noexcept;
		static void update(void* class_ptr);

	public:
		explicit WeaponComponent(const char* name, 
			const char* mesh = nullptr,
			const char* parent = XPLICIT_LUA_NAMESPACE) noexcept;

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