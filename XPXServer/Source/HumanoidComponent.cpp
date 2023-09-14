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

namespace XPX
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
		mWalkSpeed(16),
		mActiveGear(nullptr),
		mGears()
	{}

	HumanoidComponent::~HumanoidComponent() = default;

	GearComponent* HumanoidComponent::get_active_gear() noexcept { return mActiveGear; }

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

		if (self->get_health() < 1)
		{
			self->set_health(XPLICIT_DEFAULT_HEALTH);
			self->set_state(HUMANOID_STATE::ALIVE);
			self->can_spawn(true);

			self->get_class()->assign("Health", std::to_string(XPLICIT_DEFAULT_HEALTH).c_str());

			XPLICIT_INFO("world:Spawn [EVENT]");

			String xpx_player_path("world.Players.");
			xpx_player_path += self->get_peer()->xplicit_id.as_string();

			xpx_player_path = std::format("world:Spawn({})", xpx_player_path);
			Lua::CLuaStateManager::get_singleton_ptr()->run_string(xpx_player_path.c_str());
		}

		String str = "{" + std::to_string(self->mAttribute.pos().X) + "," +
						 std::to_string(self->mAttribute.pos().Y) + "," +
						 std::to_string(self->mAttribute.pos().Z) + "," + "}";

		self->mClass->assign("Position", str.c_str());

		if (self->mClass->index_as_bool("Kick"))
		{
			self->mPeer->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;

			String reason = "You have been kicked.";

			if (!self->mClass->index_as_string("KickReason").empty() &&
				self->mClass->index_as_string("KickReason").size() < XPLICIT_NETWORK_BUF_SZ)
				reason = self->mClass->index_as_string("KickReason");

			memcpy(self->mPeer->packet.buffer, reason.c_str(), reason.size());

			return;
		}

		if (self->mHealth != self->mClass->index_as_number<double>("Health"))
		{
			String path("world.Players.");
			path += self->mPeer->xplicit_id.as_string();

			XPLICIT_INFO("world:Damage [EVENT]");

			String fmt = fmt::format("world:Damage({})", path);
			Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
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
			if (self->mPeer->packet.id < self->mGears.size())
			{
				if (self->mActiveGear &&
					self->mActiveGear->index_as_number("Slot") != self->mPeer->packet.id)
				{
					self->mActiveGear->assign("Equipped", "false");
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

							self->mActiveGear->assign("Equipped", "true");
							self->mActiveGear->call_method("Update('Equipped')");
						}
					}
				}

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
			if (mClass)
			{				// reset gear array
				for (auto& gear : this->mGears)
				{
					if (gear)
						XPX::ComponentSystem::get_singleton_ptr()->remove(gear);

					gear = nullptr;
				}

				mClass.reset();
			}

			String path("world.Players.");
			path += mPeer->xplicit_id.as_string();

			mClass.reset();
			mClass = std::make_unique<Lua::CLuaClass>(path);

			if (mClass)
			{
				mClass->insert("UserName", "'Unconnected'");

				mClass->insert("Parent", "world.Players");
				mClass->insert("PlayerId", fmt::format("\"{}\"", mPeer->xplicit_id.as_string()).c_str());
				mClass->insert("IsMoving", "false");
				mClass->insert("LookAt", "{ X = 0, Y = 0, Z = 0 }");
				mClass->insert("Position", "{ X = 0, Y = 0, Z = 0 }");
				mClass->insert("State", "world.HumanoidState.ALIVE");
				mClass->insert("Kick", "false");
				mClass->insert("KickReason", "'Kicked by server.'");
				mClass->insert("Anchored", "false");
				mClass->insert("Locked", "false");
				mClass->insert("Health", std::to_string(mHealth).c_str());
				mClass->insert("MaxHealth", std::to_string(mMaxHealth).c_str());
				mClass->insert("JumpPower", std::to_string(mJumpPower).c_str());
				mClass->insert("WalkSpeed", std::to_string(mWalkSpeed).c_str());

				mClass->insert("IsLeftClickHold", "false");
				mClass->insert("IsRightClickHold", "false");

				mClass->insert("PacketKind", "-1");
				mClass->insert("PacketDeliveryKind", "-1");
				mClass->insert("PacketContent", "nil");

				XPLICIT_INFO("world:Login [EVENT]");

				String fmt = std::format("world:Login({})", path);
				Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
			}
		}
	}

	bool HumanoidComponent::can_spawn() const noexcept { return mCanSpawn; }

	XPXAttribute& HumanoidComponent::get_attribute() noexcept { return mAttribute; }

	void HumanoidComponent::can_spawn(const bool enable) noexcept { mCanSpawn = enable; }

	bool HumanoidComponent::is_alive() const noexcept { return mHealth > 0; }

	const HUMANOID_STATE& HumanoidComponent::get_state() noexcept { return mState; }

	void HumanoidComponent::set_state(const HUMANOID_STATE state) noexcept { mState = state; }

	XPX::Lua::CLuaClass* HumanoidComponent::get_class() const
	{
		XPLICIT_ASSERT(mClass);
		return mClass.get();
	}

	std::array<GearComponent*, XPLICIT_MAX_ELEMENTS_INVENTORY>& HumanoidComponent::get_gears() noexcept
	{
		return mGears;
	}
}
