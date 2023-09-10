/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ServerConfig.h"
#include "GearComponent.h"

#include <CLua/CLua.hpp>

namespace XPX
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
		static void update(void* self);

	public:
		bool can_collide() noexcept override;
		bool has_physics() noexcept override;

	public:
		XPXAttribute& get_attribute() noexcept;

	public:
		void set_health(const double& health) noexcept;
		const double& get_health() noexcept;
		bool is_alive() const noexcept;

	public:
		void set_walk_speed(const double& speed) noexcept { mWalkSpeed = speed; }
		const double& get_walk_speed() noexcept { return mWalkSpeed; }

		void set_jump_power(const double& jump) noexcept { mJumpPower = jump; }
		const double& get_jump_power() noexcept { return mJumpPower; }

	public:
		void set_state(const HUMANOID_STATE state) noexcept;
		void can_spawn(const bool enable) noexcept;
		const HUMANOID_STATE& get_state() noexcept;
		bool can_spawn() const noexcept;

	public:
		std::array<GearComponent*, XPLICIT_MAX_ELEMENTS_INVENTORY>& get_gears() noexcept;
		GearComponent* get_active_gear() noexcept;

	public:
		XPX::Lua::CLuaClass* get_class() const;

	private:
		std::unique_ptr<Lua::CLuaClass> mClass;

	private:
		std::array<GearComponent*, XPLICIT_MAX_ELEMENTS_INVENTORY> mGears;
		GearComponent* mActiveGear;
		XPXAttribute mAttribute; // XPlicit attribute class. Used to reflect data about the current component.
		HUMANOID_STATE mState;
		NetworkPeer* mPeer;
		double mWalkSpeed;
		double mMaxHealth;
		double mJumpPower;
		double mHealth;
		bool mCanSpawn;

	};
}