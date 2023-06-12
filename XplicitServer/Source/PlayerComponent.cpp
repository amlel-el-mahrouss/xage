/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
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
		: Component(), mPeer(nullptr), mHealth(XPLICIT_DEFAULT_HEALTH), 
		mDeathTimeout(0), 
		m_position(0, 0, 0), 
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

	PlayerComponent::PHYSICS_TYPE PlayerComponent::physics() noexcept 
	{
		return PHYSICS_NONE; 
	}

	bool PlayerComponent::has_physics() noexcept 
	{ 
		return false; 
	}

	void PlayerComponent::update()
	{
		if (!mPeer)
			return;

		if (mPeer->packet.cmd[XPLICIT_NETWORK_CMD_DEAD] == NETWORK_CMD_DEAD)
			mDeathTimeout = XPLICIT_COOLDOWN;
		
		if (mDeathTimeout > 0)
		{
			--mDeathTimeout;

			if (mDeathTimeout <= 0)
			{
				mDeathTimeout = 0;
				this->health(XPLICIT_DEFAULT_HEALTH);
			}
		}

		--mCooldown;
	}

	void PlayerComponent::health(const int32_t& health) noexcept 
	{ 
		this->mHealth = health; 
	}

	const int64_t PlayerComponent::health() noexcept 
	{ 
		return this->mHealth; 
	}

	bool PlayerComponent::can_collide() noexcept 
	{
		return true; 
	}

	PlayerComponent::INSTANCE_TYPE PlayerComponent::type() noexcept { return INSTANCE_PLAYER; }

	const char* PlayerComponent::name() noexcept { return "Player"; }

	bool PlayerComponent::should_update() noexcept 
	{ 
		return true; 
	}

	NetworkPeer* PlayerComponent::get() noexcept 
	{ 
		return mPeer; 
	}

	void PlayerComponent::set(NetworkPeer* peer) noexcept 
	{
		XPLICIT_ASSERT(peer);
		mPeer = peer;
	}

	bool PlayerComponent::is_frozen() noexcept
	{
		if (mFrozen)
			return true;

		return mCooldown > 0;
	}

	void PlayerComponent::freeze_for(const int64_t& cooldown) noexcept
	{
		mCooldown = cooldown;
	}

	Vector<float>& PlayerComponent::pos() noexcept
	{
		return m_position;
	}

	void PlayerComponent::freeze(const bool enable) noexcept
	{
		mFrozen = true;
	}

	bool PlayerComponent::alive() noexcept
	{
		return mHealth > 0;
	}
}