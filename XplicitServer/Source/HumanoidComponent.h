/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "CommonInclude.h"

namespace Xplicit
{
	enum class HUMANOID_STATE
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

		XPLICIT_COPY_DEFAULT(HumanoidComponent);

	public:
		void set_peer(NetworkInstance* peer) noexcept;
		NetworkInstance* get_peer() const noexcept;

	public:
		PHYSICS_TYPE physics() noexcept override;
		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

	public:
		bool should_update() noexcept override;
		void update() override;

	public:
		bool can_collide() noexcept override;
		bool has_physics() noexcept override;

		XAttribute& get_attribute() noexcept;

	public:
		void set_health(const int32_t& health) noexcept;
		int64_t get_health() const noexcept;
		bool is_alive() const noexcept;

	public:
		void should_spawn(const bool enable) noexcept;
		HUMANOID_STATE get_state() const noexcept;
		bool can_spawn() const noexcept;
		int64_t id() const noexcept;

	private:
		NetworkInstance* mPeer;
		XAttribute mAttribute;
		HUMANOID_STATE mState;
		int16_t mHealth;
		bool mCanSpawn;
		int64_t mId;

	};

	using PlayerID = std::int64_t;
}