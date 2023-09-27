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

namespace XPX
{
	class CharacterComponent;

	class WeaponComponent final : public XPXAttribute, public Component
	{
	public:
		WeaponComponent(const char* name, std::size_t slot) noexcept;

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

	public:
		virtual void equip() noexcept { }
		virtual void unequip() noexcept {  }

	private:
		CharacterComponent* mOwner;

	public:
		std::int64_t f_iSlot{ 0 };
		bool f_bDrop{ false };
		String f_strName;

	};
}