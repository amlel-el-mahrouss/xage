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
#include <lua/CLua.hpp>

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
		mState(HUMANOID_STATE::DEAD)
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

		String fmt = XPLICIT_LUA_GLOBAL;

		fmt += XPLICIT_LUA_NAMESPACE;
		fmt += this->get_peer()->xplicit_id.as_string();
		fmt += ".Health ";
		fmt += "= " + std::to_string(this->mHealth) + ";";

		Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

		fmt.clear();

		fmt = XPLICIT_LUA_GLOBAL;
		fmt += XPLICIT_LUA_NAMESPACE;
		fmt += this->get_peer()->xplicit_id.as_string();
		fmt += ".State = ";

		if (mState == HUMANOID_STATE::ALIVE)
			fmt += XPLICIT_LUA_GLOBAL + String("Game.HumanoidState.Alive");

		if (mState == HUMANOID_STATE::DEAD)
			fmt += XPLICIT_LUA_GLOBAL + String("Game.HumanoidState.Dead");

		if (mState == HUMANOID_STATE::INVALID)
			fmt += XPLICIT_LUA_GLOBAL + String("Game.HumanoidState.Invalid");

		Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

		fmt.clear();

		fmt = XPLICIT_LUA_GLOBAL;

		fmt += XPLICIT_LUA_NAMESPACE;
		fmt += this->get_peer()->xplicit_id.as_string();
		fmt += ".Position ";
		fmt += String("= ") + "{" + std::to_string(mAttribute.pos().X) + "," + 
									std::to_string(mAttribute.pos().Y) + "," +
									std::to_string(mAttribute.pos().Z) + "," + "}";

		Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

		fmt.clear();

		fmt += XPLICIT_LUA_NAMESPACE;
		fmt += this->get_peer()->xplicit_id.as_string();
		fmt += ".ID = ";
		fmt += "\"" +  mPeer->xplicit_id.as_string() + "\"";

		Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

		fmt.clear();

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
		// invaliate previous instance
		if (this->get_peer() != nullptr)
		{
			String fmt = XPLICIT_LUA_GLOBAL;
			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += this->get_peer()->xplicit_id.as_string();
			fmt += " = nil";

			Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}

		if (peer == nullptr)
			mState = HUMANOID_STATE::DEAD;
		
		mPeer = peer;

		if (mPeer)
		{
			String fmt = XPLICIT_LUA_GLOBAL;
			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mPeer->xplicit_id.as_string();
			fmt += String(" = { Position = { X = 0, Y = 0, Z = 0, }, Health = 0, ID = '', State = ") + XPLICIT_LUA_GLOBAL + String("Game.HumanoidState.Dead") + " }";

			Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}
	}

	bool HumanoidComponent::can_spawn() const noexcept { return mCanSpawn; }

	XAttribute& HumanoidComponent::get_attribute() noexcept { return mAttribute; }

	void HumanoidComponent::should_spawn(const bool enable) noexcept { mCanSpawn = enable; }

	bool HumanoidComponent::is_alive() const noexcept { return mHealth > 0; }

	const HUMANOID_STATE& HumanoidComponent::get_state() noexcept { return mState; }
}

#undef XPLICIT_LUA_GLOBAL
#undef XPLICIT_LUA_NAMESPACE
