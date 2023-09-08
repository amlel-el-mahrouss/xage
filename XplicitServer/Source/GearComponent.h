/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Util.h>
#include <Component.h>
#include <CommonEngine.h>
#include <ClassComponent.h>

namespace Xplicit
{
	class HumanoidComponent;

	class GearComponent final : public ClassComponent
	{
	public:
		GearComponent(const char* name, const char* parent = "World") noexcept;

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
		static void update(ClassPtr _self);

	public:
		HumanoidComponent* get_owner() noexcept { return mOwner; }
		void set_owner(HumanoidComponent* owner) noexcept { mOwner = owner; }

	private:
		HumanoidComponent* mOwner;
		String mParent;
		String mName;

	};
}