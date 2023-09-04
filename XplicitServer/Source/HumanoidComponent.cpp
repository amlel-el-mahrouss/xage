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
		HumanoidComponent* self = (HumanoidComponent*)_this_ptr;

		if (self->get_peer() == nullptr)
		{
			self->mState = HUMANOID_STATE::INVALID;
			return;
		}

		// execute a series of commands for this humanoid.

		if (self->mHealth > 0)
			self->mState = HUMANOID_STATE::ALIVE;
		else
			self->mState = HUMANOID_STATE::DEAD;

		String str = "{" + std::to_string(self->mAttribute.pos().X) + "," +
						 std::to_string(self->mAttribute.pos().Y) + "," +
						 std::to_string(self->mAttribute.pos().Z) + "," + "}";

		self->mClass->assign("Position", str.c_str());

		if (self->mClass->index_as_bool("Kick"))
			self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;

		self->mHealth = self->mClass->index_as_number<double>("Health");
		self->mMaxHealth = self->mClass->index_as_number<double>("MaxHealth");
		self->mJumpPower = self->mClass->index_as_number<double>("JumpPower");
		self->mWalkspeed = self->mClass->index_as_number<double>("WalkSpeed");

		self->mClass->assign("IsLeftClickPressed", self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_LCLICK] == NETWORK_CMD_LCLICK ? "true" : "false");
		self->mClass->assign("IsRightClickPressed", self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_RCLICK] == NETWORK_CMD_RCLICK ? "true" : "false");

		self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_LCLICK] = NETWORK_CMD_INVALID;
		self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_RCLICK] = NETWORK_CMD_INVALID;

		for (auto gear : self->mGears)
		{
			if (gear == nullptr ||
				gear->index_as_bool("CanDrop"))
			{
				gear->insert("Parent", "World");
				gear->insert("Owner", "Parent");

				self->get_class()->assign(gear->name(), "nil");

				auto it = std::find(self->mGears.cbegin(), self->mGears.cend(), gear);

				if (it != self->mGears.cend())
				{
					self->mGears.erase(it);
				}

				continue;
			}
		}
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
			String path("_G.World.Players.");
			path += mPeer->xplicit_id.as_string();

			if (!mClass)
				mClass = std::make_unique<Lua::CLuaClass>(path);

			if (mClass)
			{
				//! Reset Humanoid information
				mClass->insert("Name", "\""
					XPLICIT_DEFAULT_NAME
					"\"");

				mClass->insert("LookAt", "{ X = 0, Y = 0, Z = 0 }");
				mClass->insert("Position", "{ X = 0, Y = 0, Z = 0 }");
				mClass->insert("State", "World.HumanoidState.Alive");
				mClass->insert("Kick", "false");
				mClass->insert("XPLICIT_ID", std::format("\"{}\"", mPeer->xplicit_id.as_string()).c_str());
				mClass->insert("Health", std::to_string(mHealth).c_str());
				mClass->insert("MaxHealth", std::to_string(mMaxHealth).c_str());
				mClass->insert("JumpPower", std::to_string(mJumpPower).c_str());
				mClass->insert("WalkSpeed", std::to_string(mWalkspeed).c_str());

				XPLICIT_INFO("World:Login [EVENT]");

				String fmt = std::format("World:Login({})", path);
				Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
			}
		}
	}

	bool HumanoidComponent::can_spawn() const noexcept { return mCanSpawn; }

	XAttribute& HumanoidComponent::get_attribute() noexcept { return mAttribute; }

	void HumanoidComponent::can_spawn(const bool enable) noexcept { mCanSpawn = enable; }

	bool HumanoidComponent::is_alive() const noexcept { return mHealth > 0; }

	const HUMANOID_STATE& HumanoidComponent::get_state() noexcept { return mState; }

	void HumanoidComponent::set_state(const HUMANOID_STATE state) noexcept { mState = state; }

	Xplicit::Lua::CLuaClass* HumanoidComponent::get_class() const
	{
		XPLICIT_ASSERT(mClass);
		return mClass.get();
	}

	std::vector<GearComponent*>& HumanoidComponent::get_gears() noexcept
	{
		return mGears;
	}
}
