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

namespace Xplicit
{
	HumanoidComponent::HumanoidComponent() 
		: 
		Component(), 
		mPeer(nullptr),
		mHealth(100), 
		mCanSpawn(true),
		mState(HUMANOID_STATE::ALIVE),
		mClass(nullptr),
		mJumpPower(1),
		mMaxHealth(100)
	{}

	HumanoidComponent::~HumanoidComponent() = default;

	PHYSICS_TYPE HumanoidComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	bool HumanoidComponent::has_physics() noexcept { return false; }

	void HumanoidComponent::update() 
	{
		if (this->get_peer() == nullptr)
			return;

		if (mHealth > 0)
		{
			mState = HUMANOID_STATE::ALIVE;

			if (mHealth >= 100 && !mCanSpawn)
				mCanSpawn = true;

		}

		// execute a series of commands for this humanoid.

		if (mState == HUMANOID_STATE::ALIVE)
		{
			mCanSpawn = true;
			mClass->insert("State", "Game.HumanoidState.Alive");
		}

		if (mState == HUMANOID_STATE::DEAD)
		{
			mCanSpawn = false;
			mClass->insert("State", "Game.HumanoidState.Dead");
		}

		if (mState == HUMANOID_STATE::INVALID)
		{
			mCanSpawn = false;
			mHealth = 0;

			mClass->insert("State", "Game.HumanoidState.Invalid");
		}

		String str = "{" + std::to_string(mAttribute.pos().X) + "," + 
						 std::to_string(mAttribute.pos().Y) + "," +
						 std::to_string(mAttribute.pos().Z) + "," + "}";

		mClass->assign("Position", str.c_str());

		mHealth = mClass->index_as_number<int64_t>("Health");
		mMaxHealth = mClass->index_as_number<int64_t>("MaxHealth");
		mJumpPower = mClass->index_as_number<int64_t>("JumpPower");
	}

	void HumanoidComponent::set_health(const int64_t& health) noexcept { this->mHealth = health; }

	const int64_t HumanoidComponent::get_health() noexcept { return this->mHealth; }

	bool HumanoidComponent::can_collide() noexcept { return true; }

	COMPONENT_TYPE HumanoidComponent::type() noexcept { return COMPONENT_HUMANOID; }

	const char* HumanoidComponent::name() noexcept { return "HumanoidComponent"; }

	bool HumanoidComponent::should_update() noexcept { return true; }

	NetworkPeer* HumanoidComponent::get_peer() const noexcept { return mPeer; }

	void HumanoidComponent::set_peer(NetworkPeer* peer) noexcept 
	{	
		mPeer = peer;

		if (mClass)
			mClass.reset();

		if (mPeer)
		{
			mClass = std::make_unique<Lua::CLuaClass>(("Game.Players." + mPeer->xplicit_id.as_string()).c_str());

			//! Reset Humanoid information
			mClass->insert("Name", "\""
									XPLICIT_DEFAULT_NAME
									"\"");

			mClass->insert("State", "Game.HumanoidState.Alive");

			mClass->insert("ID", mPeer->xplicit_id.as_string().c_str());

			mClass->insert("Health", std::to_string(mHealth).c_str());
			mClass->insert("MaxHealth", std::to_string(mMaxHealth).c_str());
			mClass->insert("JumpPower", std::to_string(mJumpPower).c_str());
		}
	}

	bool HumanoidComponent::can_spawn() const noexcept { return mCanSpawn; }

	XAttribute& HumanoidComponent::get_attribute() noexcept { return mAttribute; }

	void HumanoidComponent::can_spawn(const bool enable) noexcept { mCanSpawn = enable; }

	bool HumanoidComponent::is_alive() const noexcept { return mHealth > 0; }

	const HUMANOID_STATE HumanoidComponent::get_state() noexcept { return mState; }

	void HumanoidComponent::set_state(const HUMANOID_STATE state) noexcept { mState = state; }
}
