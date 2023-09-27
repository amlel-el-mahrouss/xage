/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "CharacterComponent.h"
#include <NpPhysicsEngine.h>

#define XPX_DEFAULT_MAXHEALTH (100)
#define XPX_DEFAULT_WALKSPEED (5)
#define XPX_DEFAULT_JUMPPOWER (3)
#define XPX_DEFAULT_HEALTH (100)

namespace XPX
{
	CharacterComponent::CharacterComponent() noexcept
		:
		mPeer(nullptr),
		mHealth(XPX_DEFAULT_HEALTH),
		mCanSpawn(true),
		mState(HUMANOID_STATE::ALIVE),
		mJumpPower(XPX_DEFAULT_JUMPPOWER),
		mMaxHealth(XPX_DEFAULT_MAXHEALTH),
		mWalkSpeed(XPX_DEFAULT_WALKSPEED),
		mActiveWeapon(nullptr),
		mWeapons()
	{}

	CharacterComponent::~CharacterComponent()
	{
		for (auto& wep : mWeapons)
		{
			delete wep;
		}
	}

	WeaponComponent* CharacterComponent::get_current_weapon() noexcept { return mActiveWeapon; }

	PHYSICS_TYPE CharacterComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	bool CharacterComponent::has_physics() noexcept { return false; }

	void CharacterComponent::update(ClassPtr class_ptr)
	{
		CharacterComponent* self = (CharacterComponent*)class_ptr;

		if (self->get_peer() == nullptr)
		{
			self->mState = HUMANOID_STATE::INVALID;
			return;
		}

		// execute a series of commands for this humanoid.

		if (self->get_health() < 1)
		{
			self->set_health(XPLICIT_DEFAULT_HEALTH);
			self->set_state(HUMANOID_STATE::ALIVE);
			self->can_spawn(true);
		}

		// select a specific item in our inventory.
		if (self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_SLOT] == NETWORK_CMD_SLOT)
		{
			if (self->mPeer->packet.id < self->mWeapons.size() &&
				self->mPeer->packet.id > -1)
			{
				if (self->mActiveWeapon &&
					self->mActiveWeapon->f_iSlot != self->mPeer->packet.id)
				{
					self->mActiveWeapon->unequip();
					self->mActiveWeapon = nullptr;
				}

				if (!self->mActiveWeapon)
				{
					for (auto& wep : self->mWeapons)
					{
						if (!wep)
							continue;

						if (wep->f_iSlot == self->mPeer->packet.id)
						{
							self->mActiveWeapon = wep;
							self->mActiveWeapon->equip();
							
							break;
						}
					}
				}

				self->mPeer->packet.id = -1;
				self->mPeer->packet.health = self->mHealth;
				self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_SLOT] = NETWORK_CMD_INVALID;
			}
		}
	}

	void CharacterComponent::set_health(const double& health) noexcept { this->mHealth = health; }

	const double& CharacterComponent::get_health() noexcept { return this->mHealth; }

	bool CharacterComponent::can_collide() noexcept { return true; }

	COMPONENT_TYPE CharacterComponent::type() noexcept { return COMPONENT_HUMANOID; }

	const char* CharacterComponent::name() noexcept { return "CharacterComponent"; }

	bool CharacterComponent::should_update() noexcept { return true; }

	NetworkPeer* CharacterComponent::get_peer() const noexcept { return mPeer; }

	void CharacterComponent::set_peer(NetworkPeer* peer) noexcept 
	{	
		if (peer)
			mPeer = peer;
	}

	bool CharacterComponent::can_spawn() const noexcept { return mCanSpawn; }

	void CharacterComponent::can_spawn(const bool enable) noexcept { mCanSpawn = enable; }

	bool CharacterComponent::is_alive() const noexcept { return mHealth > 0; }

	const HUMANOID_STATE& CharacterComponent::get_state() noexcept { return mState; }

	void CharacterComponent::set_state(const HUMANOID_STATE state) noexcept { mState = state; }

	void CharacterComponent::kick(const String& reason) noexcept
	{
		mPeer->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
		memcpy(mPeer->packet.additional_data, reason.c_str(), reason.size());
	}

	std::array<WeaponComponent*, XPX_MAX_WEAPONS>& CharacterComponent::get_weapons() noexcept
	{
		return mWeapons;
	}
}
