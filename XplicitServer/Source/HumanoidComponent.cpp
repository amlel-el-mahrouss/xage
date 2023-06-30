/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "HumanoidComponent.h"
#include <lua/lua.hpp>

namespace Xplicit
{
	HumanoidComponent::HumanoidComponent() 
		: 
		Component(), 
		mPeer(nullptr),
		mHealth(XPLICIT_DEFAULT_HEALTH), 
		mCanSpawn(true)
	{}

	HumanoidComponent::~HumanoidComponent() = default;

	PHYSICS_TYPE HumanoidComponent::physics() noexcept { return PHYSICS_NONE; }

	bool HumanoidComponent::has_physics() noexcept { return false; }

	void HumanoidComponent::update() 
	{
		if (this->get_peer() == nullptr)
			return;

		if (mHealth >= XPLICIT_DEFAULT_HEALTH)
			mState = HUMANOID_STATE::ALIVE;
		else if (mHealth < 1)
			mState = HUMANOID_STATE::DEAD;

		if (this->get_peer()->packet.cmd[XPLICIT_NETWORK_CMD_DAMAGE] == NETWORK_CMD_DAMAGE)
		{
			XPLICIT_INFO("Humanoid:Damage [EVENT]");
			Lua::XLuaStateManager::get_singleton_ptr()->run_string("Engine:Damage()");
		}
	}

	void HumanoidComponent::set_health(const int32_t& health) noexcept { this->mHealth = health; }

	int64_t HumanoidComponent::get_health() const noexcept { return this->mHealth; }

	bool HumanoidComponent::can_collide() noexcept { return true; }

	COMPONENT_TYPE HumanoidComponent::type() noexcept { return COMPONENT_HUMANOID; }

	const char* HumanoidComponent::name() noexcept { return "HumanoidComponent"; }

	bool HumanoidComponent::should_update() noexcept { return true;  }

	NetworkInstance* HumanoidComponent::get_peer() const noexcept { return mPeer; }

	void HumanoidComponent::set_peer(NetworkInstance* peer) noexcept 
	{
		if (peer == nullptr)
			mState = HUMANOID_STATE::INVALID;
		
		mPeer = peer;
	}

	bool HumanoidComponent::can_spawn() const noexcept { return mCanSpawn; }

	XAttribute& HumanoidComponent::get_attribute() noexcept { return mAttribute; }

	void HumanoidComponent::should_spawn(const bool enable) noexcept { mCanSpawn = enable; }

	bool HumanoidComponent::is_alive() const noexcept { return mHealth > 0; }

	int64_t HumanoidComponent::id() const noexcept { return mId; }

	HUMANOID_STATE HumanoidComponent::get_state() const noexcept { return mState; }
}