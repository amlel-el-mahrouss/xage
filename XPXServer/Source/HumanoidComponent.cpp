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

#include "HumanoidComponent.h"

#define XPX_DEFAULT_MAXHEALTH 100
#define XPX_DEFAULT_WALKSPEED 16
#define XPX_DEFAULT_JUMPPOWER 10
#define XPX_DEFAULT_HEALTH 100

namespace XPX
{
	HumanoidComponent::HumanoidComponent() noexcept
		:
		Component(),
		mPeer(nullptr),
		mHealth(XPX_DEFAULT_HEALTH),
		mCanSpawn(true),
		mState(HUMANOID_STATE::ALIVE),
		mClass(nullptr),
		mJumpPower(XPX_DEFAULT_JUMPPOWER),
		mMaxHealth(XPX_DEFAULT_MAXHEALTH),
		mWalkSpeed(XPX_DEFAULT_WALKSPEED),
		mActiveGear(nullptr),
		mGears()
	{}

	HumanoidComponent::~HumanoidComponent()
	{
		if (mClass)
			delete mClass;
	}

	GearComponent* HumanoidComponent::get_active_gear() noexcept { return mActiveGear; }

	PHYSICS_TYPE HumanoidComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	bool HumanoidComponent::has_physics() noexcept { return false; }

	void HumanoidComponent::update(ClassPtr class_ptr)
	{
		HumanoidComponent* self = (HumanoidComponent*)class_ptr;

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

			self->get_class()->assign("Health", std::to_string(XPLICIT_DEFAULT_HEALTH));

			XPLICIT_INFO("world:Spawn [EVENT]");

			String xpx_player_path("world.Players.");
			xpx_player_path += self->get_peer()->xplicit_id.as_string();

			xpx_player_path = std::format("world:Spawn({})", xpx_player_path);
			Lua::CLuaStateManager::get_singleton_ptr()->run_string(xpx_player_path);
		}

		String str = "{ X = " + std::to_string(self->mClass->pos().X) + ", Y = " +
						 std::to_string(self->mClass->pos().Y) + ", Z = " +
						 std::to_string(self->mClass->pos().Z) + "}";

		self->mClass->assign("Position", str);

		if (self->mClass->index_as_bool("Kick"))
		{
			self->mPeer->packet.hash = self->mPeer->hash;
			self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;

			String reason = "You have been kicked.";

			if (!self->mClass->index_as_string("KickReason").empty() &&
				self->mClass->index_as_string("KickReason").size() < XPLICIT_NETWORK_BUF_SZ)
				reason = self->mClass->index_as_string("KickReason");

			memcpy(self->mPeer->packet.additional_data, reason.c_str(), reason.size());

			return;
		}

		if (self->mHealth != self->mClass->index_as_number<double>("Health"))
		{
			String path("world.Players.");
			path += self->mPeer->xplicit_id.as_string();

			XPLICIT_INFO("world:Damage [EVENT]");

			self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_DAMAGE] = NETWORK_CMD_DAMAGE;
			self->mPeer->packet.health = self->mHealth;

			String fmt = fmt::format("world:Damage({})", path);
			Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt);
		}

		self->mHealth = self->mClass->index_as_number<double>("Health");
		self->mMaxHealth = self->mClass->index_as_number<double>("MaxHealth");
		self->mJumpPower = self->mClass->index_as_number<double>("JumpPower");
		self->mWalkSpeed = self->mClass->index_as_number<double>("WalkSpeed");

		self->mClass->assign("IsLeftClickHold", self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_LCLICK] == NETWORK_CMD_LCLICK ? "true" : "false");
		self->mClass->assign("IsRightClickHold", self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_RCLICK] == NETWORK_CMD_RCLICK ? "true" : "false");

		self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_LCLICK] = NETWORK_CMD_INVALID;
		self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_RCLICK] = NETWORK_CMD_INVALID;

		// select a specific item in our inventory.
		if (self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_SLOT] == NETWORK_CMD_SLOT)
		{
			if (self->mPeer->packet.id < self->mGears.size() &&
				self->mPeer->packet.id > -1)
			{
				if (self->mActiveGear &&
					self->mActiveGear->index_as_number("Slot") != self->mPeer->packet.id)
				{
					self->mActiveGear->call_method("Update('Unequipped')");
					self->mActiveGear = nullptr;
				}

				if (!self->mActiveGear)
				{
					for (auto& gear : self->mGears)
					{
						if (!gear)
							continue;

						if (gear->index_as_number("Slot") == self->mPeer->packet.id)
						{
							self->mActiveGear = gear;
							self->mActiveGear->call_method("Update('Equipped')");
						}
					}
				}

				self->mPeer->packet.id = -1;
				self->mPeer->packet.health = self->mHealth;
				self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_SLOT] = NETWORK_CMD_INVALID;
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

		if (mPeer)
		{
			String player_lua_arr("world.Players.");
			player_lua_arr += mPeer->xplicit_id.as_string();

			mClass = ComponentSystem::get_singleton_ptr()->add<ClassComponent>(Vector<NetworkFloat>(0, 0, 0),
				XPLICIT_CHARACTER_SCALE,
				Color<NetworkFloat>(0, 0, 0),
				nullptr, "world.Players", mPeer->xplicit_id.as_string().c_str());

			XPLICIT_ASSERT(mClass);

			if (mClass)
			{
				mClass->insert("UserName", "'Unconnected'");

				mClass->insert("LookAt", "{ X = 0, Y = 0, Z = 0 }");
				
				mClass->insert("Kick", "false");
				mClass->insert("KickReason", "'No message supplied.'");
				
				mClass->insert("IsLeftClickHold", "false");
				mClass->insert("IsRightClickHold", "false");

				mClass->insert("ContentType", "-1");
				mClass->insert("ContentBody", "nil");

				mClass->insert("PlayerId", fmt::format("\"{}\"", mPeer->xplicit_id.as_string()));

				mClass->insert("Health", std::to_string(mHealth));
				mClass->insert("MaxHealth", std::to_string(mMaxHealth));
				mClass->insert("JumpPower", std::to_string(mJumpPower));
				mClass->insert("WalkSpeed", std::to_string(mWalkSpeed));

				XPLICIT_INFO("world:Login [EVENT]");

				String fmt = std::format("world:Login({})", player_lua_arr);
				Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt);
			}
		}
		else
		{
			if (mClass)
			{
				for (auto* gear : mGears)
				{
					ComponentSystem::get_singleton_ptr()->remove(gear);
					gear = nullptr;
				}

				ComponentSystem::get_singleton_ptr()->remove(mClass);
				mClass = nullptr;
			}
		}
	}

	bool HumanoidComponent::can_spawn() const noexcept { return mCanSpawn; }

	void HumanoidComponent::can_spawn(const bool enable) noexcept { mCanSpawn = enable; }

	bool HumanoidComponent::is_alive() const noexcept { return mHealth > 0; }

	const HUMANOID_STATE& HumanoidComponent::get_state() noexcept { return mState; }

	void HumanoidComponent::set_state(const HUMANOID_STATE state) noexcept { mState = state; }

	ClassComponent* HumanoidComponent::get_class() const
	{
		XPLICIT_ASSERT(mClass);
		return mClass;
	}

	std::array<GearComponent*, XPLICIT_MAX_ELEMENTS_INVENTORY>& HumanoidComponent::get_gears() noexcept
	{
		return mGears;
	}
}
