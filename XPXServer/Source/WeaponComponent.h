/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Util.h>
#include <Enums.h>
#include <Component.h>
#include <ClassComponent.h>

namespace XPX
{
	class CharacterComponent;

	class WeaponComponent final : public ClassComponent
	{
	public:
		WeaponComponent(const char* name, 
					    const char* parent) noexcept;

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
		static void update(ClassPtr _self);

	public:
		void set_owner(CharacterComponent* owner) noexcept;
		CharacterComponent* get_owner() noexcept;

	private:
		CharacterComponent* mOwner;
		String mParent;
		String mName;

	};
}