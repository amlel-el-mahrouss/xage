/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Actor.h
 *			Purpose: Server-Side Actors (players)
 *
 * =====================================================================
 */

#pragma once

#include "CommonInclude.h"

namespace Xplicit
{
	class PlayerComponent final : public Component
	{
	public:
		explicit PlayerComponent();
		~PlayerComponent() override;

		XPLICIT_COPY_DEFAULT(PlayerComponent);

		void set(NetworkInstance* peer) noexcept;
		NetworkInstance* get() noexcept;

		PHYSICS_TYPE physics() noexcept override;
		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

		bool should_update() noexcept override;
		void update() override;

		bool can_collide() noexcept override;
		bool has_physics() noexcept override;

		Vector<float>& pos() noexcept;

		void health(const int32_t& health) noexcept;
		void freeze(const bool enable) noexcept;
		const int64_t health() noexcept;
		bool is_frozen() noexcept;
		bool alive() noexcept;

	public:
		PositionComponent Position;

	private:
		NetworkInstance* mPeer;
		int64_t mDeathTimeout;
		int64_t mCooldown;
		int64_t mHealth;
		bool mFrozen;

	};

	using PlayerArray = std::vector<PlayerComponent*>;
}