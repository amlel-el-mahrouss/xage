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

		Vector<float>& pos() noexcept;

	public:
		void health(const int32_t& health) noexcept;

		void should_spawn(const bool enable) noexcept;
		bool can_spawn() const noexcept;

		int64_t health() const noexcept;
		bool alive() const noexcept;

	public:
		PositionComponent Position;

	private:
		NetworkInstance* mPeer;
		int64_t mHealth;
		bool mCanSpawn;

	};

	using PlayerArray = std::vector<HumanoidComponent*>;
}