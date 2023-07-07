/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "HumanoidComponent.h"

#define XPLICIT_LUA_GLOBAL "_G."
#define XPLICIT_LUA_NAMESPACE "Game.Players."

namespace Xplicit
{
	HumanoidComponent::HumanoidComponent() 
		: 
		Component(), 
		mPeer(nullptr),
		mHealth(0), 
		mCanSpawn(true),
		mState(HUMANOID_STATE::DEAD),
		mClass(nullptr)
	{
		// Don't initialize lua code here, because we got no peer.
	}

	HumanoidComponent::~HumanoidComponent() = default;

	PHYSICS_TYPE HumanoidComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	bool HumanoidComponent::has_physics() noexcept { return false; }

	void HumanoidComponent::update() 
	{
		if (this->get_peer() == nullptr)
			return;

		// execute a series of commands for this humanoid.

		if (mState == HUMANOID_STATE::ALIVE)
			mClass->assign("State", "Game.HumanoidState.Alive");

		if (mState == HUMANOID_STATE::DEAD)
			mClass->assign("State", "Game.HumanoidState.Dead");

		if (mState == HUMANOID_STATE::INVALID)
			mClass->assign("State", "Game.HumanoidState.Invalid");

		auto str = "{" + std::to_string(mAttribute.pos().X) + "," + 
									std::to_string(mAttribute.pos().Y) + "," +
									std::to_string(mAttribute.pos().Z) + "," + "}";

		mClass->assign("Position", str.c_str());

		try
		{
			mHealth = std::atol(mClass->index("Health"));
		}
		catch (...)
		{
			mHealth = 0UL;
		}

		if (mHealth >= XPLICIT_DEFAULT_HEALTH)
			mState = HUMANOID_STATE::ALIVE;
		else if (mHealth < 1)
			mState = HUMANOID_STATE::DEAD;
		else if (!this->get_peer())
			mState = HUMANOID_STATE::INVALID;

		if (this->get_peer()->packet.cmd[XPLICIT_NETWORK_CMD_DAMAGE] == NETWORK_CMD_DAMAGE)
		{
			XPLICIT_INFO("Game:Damage [EVENT]");
			Lua::CLuaStateManager::get_singleton_ptr()->run_string("Game:Damage()");
		}

	}

	void HumanoidComponent::set_health(const int64_t& health) noexcept { this->mHealth = health; }

	const int64_t& HumanoidComponent::get_health() noexcept { return this->mHealth; }

	bool HumanoidComponent::can_collide() noexcept { return true; }

	COMPONENT_TYPE HumanoidComponent::type() noexcept { return COMPONENT_HUMANOID; }

	const char* HumanoidComponent::name() noexcept { return "HumanoidComponent"; }

	bool HumanoidComponent::should_update() noexcept { return true;  }

	NetworkInstance* HumanoidComponent::get_peer() const noexcept { return mPeer; }

	void HumanoidComponent::set_peer(NetworkInstance* peer) noexcept 
	{	
		mPeer = peer;

		if (mClass)
			mClass.reset();

		if (mPeer)
		{
			mClass = std::make_unique<Lua::CLuaClass>(("Game.Players." + mPeer->xplicit_id.as_string()).c_str());

			//! Reset Humanoid information
			mClass->insert("Position", "{ X = 0, Y = 0, Z = 0, }");
			mClass->insert("Health", std::to_string(XPLICIT_DEFAULT_HEALTH).c_str());
			mClass->insert("State", "Game.HumanoidState.Dead");
			mClass->insert("ID", mPeer->xplicit_id.as_string().c_str());
		}
	}

	bool HumanoidComponent::can_spawn() const noexcept { return mCanSpawn; }

	XAttribute& HumanoidComponent::get_attribute() noexcept { return mAttribute; }

	void HumanoidComponent::should_spawn(const bool enable) noexcept { mCanSpawn = enable; }

	bool HumanoidComponent::is_alive() const noexcept { return mHealth > 0; }

	const HUMANOID_STATE& HumanoidComponent::get_state() noexcept { return mState; }
}
