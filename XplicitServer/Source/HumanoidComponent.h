/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "CommonInclude.h"
#include <lua/CLua.hpp>

namespace Xplicit
{
	enum class HUMANOID_STATE : std::int16_t
	{
		ALIVE,
		DEAD,
		INVALID,
	};

	class HumanoidComponent final : public Component
	{
	public:
		explicit HumanoidComponent();
		~HumanoidComponent() override;

	public:
		XPLICIT_COPY_DEFAULT(HumanoidComponent);

	public:
		void set_peer(NetworkPeer* peer) noexcept;
		NetworkPeer* get_peer() const noexcept;

	public:
		PHYSICS_TYPE physics() noexcept override;
		COMPONENT_TYPE type() noexcept override;

	public:
		const char* name() noexcept override;

	public:
		bool should_update() noexcept override;
		void update() override;

	public:
		bool can_collide() noexcept override;
		bool has_physics() noexcept override;

	public:
		XAttribute& get_attribute() noexcept;

	public:
		void set_health(const int64_t& health) noexcept;
		const int64_t get_health() noexcept;
		bool is_alive() const noexcept;

	public:
		void set_state(const HUMANOID_STATE state) noexcept;
		void can_spawn(const bool enable) noexcept;
		const HUMANOID_STATE get_state() noexcept;
		bool can_spawn() const noexcept;

	private:
		std::unique_ptr<Lua::CLuaClass> mClass;
		XAttribute mAttribute;
		HUMANOID_STATE mState;
		int64_t mMaxHealth;
		int64_t mJumpPower;
		NetworkPeer* mPeer;
		int64_t mHealth;
		bool mCanSpawn;

	};

	using PlayerID = std::int64_t;
}