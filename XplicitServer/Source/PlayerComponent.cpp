/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Actor.cpp
 *			Purpose: Server-Side Actors
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "PlayerComponent.h"

namespace Xplicit
{
	PlayerComponent::PlayerComponent() 
		: 
		Component(), 
		mPeer(nullptr),
		mHealth(XPLICIT_DEFAULT_HEALTH), 
		mDeathTimeout(0), 
		Position(), 
		mCooldown(0),
		mFrozen(false)
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("PlayerComponent::PlayerComponent");
#endif
	}

	PlayerComponent::~PlayerComponent() 
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("PlayerComponent::~PlayerComponent");
#endif
	}

	PHYSICS_TYPE PlayerComponent::physics() noexcept { return PHYSICS_NONE; }

	bool PlayerComponent::has_physics() noexcept { return false; }

	void PlayerComponent::update() {}

	void PlayerComponent::health(const int32_t& health) noexcept { this->mHealth = health; }

	const int64_t PlayerComponent::health() noexcept { return this->mHealth; }

	bool PlayerComponent::can_collide() noexcept { return true; }

	COMPONENT_TYPE PlayerComponent::type() noexcept { return COMPONENT_PLAYER; }

	const char* PlayerComponent::name() noexcept { return "PlayerComponent"; }

	bool PlayerComponent::should_update() noexcept { return false;  }

	NetworkInstance* PlayerComponent::get_peer() noexcept { return mPeer; }

	void PlayerComponent::set_peer(NetworkInstance* peer) noexcept 
	{
		mPeer = peer;
	}

	bool PlayerComponent::is_frozen() noexcept
	{
		if (mFrozen)
			return true;

		return mCooldown > 0;
	}

	Vector<float>& PlayerComponent::pos() noexcept { return Position.Position; }

	void PlayerComponent::freeze(const bool enable) noexcept { mFrozen = true; }

	bool PlayerComponent::alive() noexcept { return mHealth > 0; }
}