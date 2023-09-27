/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

// Include the component class.

#include "WeaponComponent.h"
#include "CharacterComponent.h"

//! This file handles Gears (Sword, Pistol, Build Tools...)

namespace XPX
{
	WeaponComponent::WeaponComponent(const char* name, const std::size_t slot) noexcept
		:
		f_iSlot(slot),
		f_strName(name),
		mOwner(nullptr)
	{}

	const char* WeaponComponent::name() noexcept { return f_strName.c_str(); }

	COMPONENT_TYPE WeaponComponent::type() noexcept { return COMPONENT_LOGIC; }

	PHYSICS_TYPE WeaponComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	WeaponComponent::~WeaponComponent() = default;

	bool WeaponComponent::should_update() noexcept { return true; }

	void WeaponComponent::update(ClassPtr this_ptr) 
	{ 
		if (!this_ptr)
			return;

		WeaponComponent* self = (WeaponComponent*)this_ptr;

		if (self->f_bDrop &&
			self->get_owner())
		{
			self->set_owner(nullptr);
		}

		if (self->get_owner() &&
			self->get_owner()->get_peer())
		{
			self->pos() = self->get_owner()->pos();
			self->look_pos() = self->get_owner()->look_pos();
		}
	}

	CharacterComponent* WeaponComponent::get_owner() noexcept { return mOwner; }
	void WeaponComponent::set_owner(CharacterComponent* owner) noexcept { mOwner = owner; }
}
