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

#define XPLICIT_LUA_GLOBAL "_G."
#define XPLICIT_LUA_NAMESPACE "Game.Players."

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

	PHYSICS_TYPE HumanoidComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	bool HumanoidComponent::has_physics() noexcept { return false; }

	void HumanoidComponent::update() 
	{
		if (this->get_peer() == nullptr)
			return;

		// execute a series of commands for Component.

		String fmt = XPLICIT_LUA_GLOBAL;

		fmt += XPLICIT_LUA_NAMESPACE;
		fmt += this->get_peer()->xplicit_id.as_string();
		fmt += ".Health ";
		fmt += "= " + std::to_string(this->mHealth) + ";";

		Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

		fmt.clear();

		fmt = XPLICIT_LUA_GLOBAL;
		fmt += XPLICIT_LUA_NAMESPACE;
		fmt += this->get_peer()->xplicit_id.as_string();
		fmt += ".State = ";

		if (mState == HUMANOID_STATE::ALIVE)
			fmt += "HUMANOID.ALIVE";

		if (mState == HUMANOID_STATE::DEAD)
			fmt += "HUMANOID.DEAD";

		Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

		fmt.clear();

		static const char* pos[] = { "X", "Y", "Z" };
		const float pos_raw[] = { mAttribute.pos().X, mAttribute.pos().Y, mAttribute.pos().Z };
			
		for (size_t i = 0; i < 3; i++)
		{
			String fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += this->get_peer()->xplicit_id.as_string();
			fmt += ".Position.";
			fmt += pos[i];
			fmt += "= " + std::to_string(pos_raw[i]) + ";";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}

		static const char* clr[] = { "R", "G", "B" };
		const float clr_raw[] = { mAttribute.color().R, mAttribute.color().G, mAttribute.color().B };

		for (size_t i = 0; i < 3; i++)
		{
			String fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += this->get_peer()->xplicit_id.as_string();
			fmt += ".Color.";
			fmt += clr[i];
			fmt += "= " + std::to_string(clr_raw[i]) + ";";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}

		XPLICIT_INFO("Humanoid:Update [EVENT]");

		if (this->get_attribute().script() &&
			this->get_attribute().script()->name() == "Update")
			this->get_attribute().script()->run();

		if (mHealth >= XPLICIT_DEFAULT_HEALTH)
			mState = HUMANOID_STATE::ALIVE;
		else if (mHealth < 1)
			mState = HUMANOID_STATE::DEAD;

		if (this->get_peer()->packet.cmd[XPLICIT_NETWORK_CMD_DAMAGE] == NETWORK_CMD_DAMAGE)
		{
			XPLICIT_INFO("Humanoid:Damage [EVENT]");

			if (this->get_attribute().script() &&
				this->get_attribute().script()->name() == "Damage")
				this->get_attribute().script()->run();

			XPLICIT_INFO("Humanoid:Damage [EVENT]");
			Lua::XLuaStateManager::get_singleton_ptr()->run_string("Engine:Damage()");
		}
	}

	void HumanoidComponent::set_health(const int32_t& health) noexcept { this->mHealth = health; }

	const int64_t& HumanoidComponent::get_health() noexcept { return this->mHealth; }

	bool HumanoidComponent::can_collide() noexcept { return true; }

	COMPONENT_TYPE HumanoidComponent::type() noexcept { return COMPONENT_HUMANOID; }

	const char* HumanoidComponent::name() noexcept { return "HumanoidComponent"; }

	bool HumanoidComponent::should_update() noexcept { return true;  }

	NetworkInstance* HumanoidComponent::get_peer() const noexcept { return mPeer; }

	void HumanoidComponent::set_peer(NetworkInstance* peer) noexcept 
	{
		// invaliate previous instance
		if (this->get_peer() != nullptr)
		{
			String fmt = XPLICIT_LUA_GLOBAL;
			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += this->get_peer()->xplicit_id.as_string();
			fmt += " = nil";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}

		if (peer == nullptr)
			mState = HUMANOID_STATE::INVALID;
		
		mPeer = peer;

		if (mPeer)
		{
			String fmt = XPLICIT_LUA_GLOBAL;
			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mPeer->xplicit_id.as_string();
			fmt += " = { Position = { X = 0, Y = 0, Z = 0, }, Color = { R = 0, G = 0, B = 0, }, Health = 0, State = HUMANOID.ALIVE }";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}
	}

	bool HumanoidComponent::can_spawn() const noexcept { return mCanSpawn; }

	XAttribute& HumanoidComponent::get_attribute() noexcept { return mAttribute; }

	void HumanoidComponent::should_spawn(const bool enable) noexcept { mCanSpawn = enable; }

	bool HumanoidComponent::is_alive() const noexcept { return mHealth > 0; }

	const int64_t& HumanoidComponent::id() noexcept { return mId; }

	const HUMANOID_STATE& HumanoidComponent::get_state() noexcept { return mState; }
}

#undef XPLICIT_LUA_GLOBAL
#undef XPLICIT_LUA_NAMESPACE
