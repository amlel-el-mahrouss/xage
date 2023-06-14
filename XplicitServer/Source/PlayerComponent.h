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

#include "CommonInc.h"

namespace Xplicit
{
	class PlayerComponent final : public Component
	{
	public:
		PlayerComponent();
		virtual ~PlayerComponent();

		XPLICIT_COPY_DELETE(PlayerComponent);

	public:
		void set(NetworkPeer* peer) noexcept;
		NetworkPeer* get() noexcept;

	public:
		virtual PHYSICS_TYPE physics() noexcept override;
		virtual INSTANCE_TYPE type() noexcept override;
		virtual bool should_update() noexcept override;
		virtual const char* name() noexcept override;
		virtual bool can_collide() noexcept override;
		virtual bool has_physics() noexcept override;
		virtual void update() override;

	public:
		Vector<float>& pos() noexcept;

	public:
		void freeze_for(const int64_t& cooldown) noexcept;
		void health(const int32_t& health) noexcept;
		void freeze(const bool enable) noexcept;
		const int64_t health() noexcept;
		bool is_frozen() noexcept;
		bool alive() noexcept;

	private:
		Vector<float> m_position;
		NetworkPeer* mPeer;

	private:
		int64_t mDeathTimeout;
		int64_t mCooldown;
		int64_t mHealth;
		bool mFrozen;

	};

	using PlayerArray = std::vector<PlayerComponent*>;
}