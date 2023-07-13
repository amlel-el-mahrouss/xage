/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ServerConfig.h"
#include <CLua/CLua.hpp>

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
		static bool should_update() noexcept;
		static void update(void* _this);

	public:
		bool can_collide() noexcept override;
		bool has_physics() noexcept override;

	public:
		XAttribute& get_attribute() noexcept;

	public:
		void set_health(const double& health) noexcept;
		const double& get_health() noexcept;
		bool is_alive() const noexcept;

	public:
		void set_walk_speed(const double& speed) noexcept { mWalkspeed = speed; }
		const double& get_walk_speed() noexcept { return mWalkspeed; }

	public:
		void set_state(const HUMANOID_STATE state) noexcept;
		void can_spawn(const bool enable) noexcept;
		const HUMANOID_STATE& get_state() noexcept;
		bool can_spawn() const noexcept;

	public:
		Xplicit::Lua::CLuaClass* get_class() const;

	private:
		std::unique_ptr<Lua::CLuaClass> mClass;

	private:
		XAttribute mAttribute;
		HUMANOID_STATE mState;
		NetworkPeer* mPeer;

	private:
		double mWalkspeed;
		double mMaxHealth;
		double mJumpPower;
		double mHealth;
		bool mCanSpawn;

	};

	using PlayerID = std::int64_t;
}