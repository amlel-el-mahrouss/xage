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

#include "HumanoidComponent.h"

namespace Xplicit
{
	HumanoidComponent::HumanoidComponent() 
		: 
		Component(), 
		mPeer(nullptr),
		mHealth(XPLICIT_DEFAULT_HEALTH), 
		mCanSpawn(true)
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("HumanoidComponent::HumanoidComponent");
#endif
	}

	HumanoidComponent::~HumanoidComponent() 
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("HumanoidComponent::~HumanoidComponent");
#endif
	}

	PHYSICS_TYPE HumanoidComponent::physics() noexcept { return PHYSICS_NONE; }

	bool HumanoidComponent::has_physics() noexcept { return false; }

	void HumanoidComponent::update() {}

	void HumanoidComponent::health(const int32_t& health) noexcept { this->mHealth = health; }

	int64_t HumanoidComponent::health() const noexcept { return this->mHealth; }

	bool HumanoidComponent::can_collide() noexcept { return true; }

	COMPONENT_TYPE HumanoidComponent::type() noexcept { return COMPONENT_PLAYER; }

	const char* HumanoidComponent::name() noexcept { return "HumanoidComponent"; }

	bool HumanoidComponent::should_update() noexcept { return false;  }

	NetworkInstance* HumanoidComponent::get_peer() const noexcept { return mPeer; }

	void HumanoidComponent::set_peer(NetworkInstance* peer) noexcept 
	{
		mPeer = peer;
	}

	bool HumanoidComponent::can_spawn() const noexcept
	{
		return mCanSpawn;
	}

	Vector<float>& HumanoidComponent::pos() noexcept { return Position.Position; }

	void HumanoidComponent::should_spawn(const bool enable) noexcept { mCanSpawn = enable; }

	bool HumanoidComponent::alive() const noexcept { return mHealth > 0; }
}