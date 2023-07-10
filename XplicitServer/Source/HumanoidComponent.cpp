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
		mJumpPower(10),
		mMaxHealth(100),
		mWalkspeed(16)
	{}

	HumanoidComponent::~HumanoidComponent() = default;

	PHYSICS_TYPE HumanoidComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	bool HumanoidComponent::has_physics() noexcept { return false; }

	void HumanoidComponent::update(void* _this_ptr)
	{
		HumanoidComponent* _this = (HumanoidComponent*)_this_ptr;

		if (_this->get_peer() == nullptr)
			return;

		if (_this->mHealth > 0)
		{
			_this->mState = HUMANOID_STATE::ALIVE;

			if (_this->mHealth >= 100 && !_this->mCanSpawn)
				_this->mCanSpawn = true;

		}

		// execute a series of commands for this humanoid.

		if (_this->mState == HUMANOID_STATE::ALIVE)
		{
			_this->mClass->insert("Health", std::to_string(_this->mHealth).c_str());

			_this->mCanSpawn = true;
			_this->mClass->insert("State", "World.HumanoidState.Alive");
		}

		if (_this->mState == HUMANOID_STATE::DEAD)
		{
			_this->mCanSpawn = false;
			_this->mClass->insert("State", "World.HumanoidState.Dead");
		}

		if (_this->mState == HUMANOID_STATE::INVALID)
		{
			_this->mCanSpawn = false;
			_this->mClass->insert("State", "World.HumanoidState.Invalid");
		}

		String str = "{" + std::to_string(_this->mAttribute.pos().X) + "," +
						 std::to_string(_this->mAttribute.pos().Y) + "," +
						 std::to_string(_this->mAttribute.pos().Z) + "," + "}";

		_this->mClass->assign("Position", str.c_str());

		_this->mHealth = _this->mClass->index_as_number<double>("Health");
		_this->mMaxHealth = _this->mClass->index_as_number<double>("MaxHealth");
		_this->mJumpPower = _this->mClass->index_as_number<double>("JumpPower");
		_this->mWalkspeed = _this->mClass->index_as_number<double>("Walkspeed");
	}

	void HumanoidComponent::set_health(const double& health) noexcept { this->mHealth = health; }

	const double& HumanoidComponent::get_health() noexcept { return this->mHealth; }

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
			String path("World.Players.");
			path += mPeer->xplicit_id.as_string();

			if (!mClass)
				mClass = std::make_unique<Lua::CLuaClass>(path);

			if (mClass)
			{
				//! Reset Humanoid information
				mClass->insert("Name", "\""
					XPLICIT_DEFAULT_NAME
					"\"");

				mClass->insert("State", "World.HumanoidState.Alive");
				mClass->insert("ID", mPeer->xplicit_id.as_string().c_str());
				mClass->insert("Health", std::to_string(mHealth).c_str());
				mClass->insert("MaxHealth", std::to_string(mMaxHealth).c_str());
				mClass->insert("JumpPower", std::to_string(mJumpPower).c_str());
				mClass->insert("WalkSpeed", std::to_string(mWalkspeed).c_str());
			}
		}
	}

	bool HumanoidComponent::can_spawn() const noexcept { return mCanSpawn; }

	XAttribute& HumanoidComponent::get_attribute() noexcept { return mAttribute; }

	void HumanoidComponent::can_spawn(const bool enable) noexcept { mCanSpawn = enable; }

	bool HumanoidComponent::is_alive() const noexcept { return mHealth > 0; }

	const HUMANOID_STATE& HumanoidComponent::get_state() noexcept { return mState; }

	void HumanoidComponent::set_state(const HUMANOID_STATE state) noexcept { mState = state; }
}
