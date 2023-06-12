/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Actor.h
 *			Purpose: Server-Side Actors (players)
 *
 * =====================================================================
 */

#pragma once

#include "ServerSDK.h"

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
		virtual INSTANCE_TYPE type() noexcept override;
		virtual const char* name() noexcept override;
		virtual bool should_update() noexcept override;
		virtual void update() override;

	public:
		virtual PHYSICS_TYPE physics() noexcept override;
		virtual bool can_collide() noexcept override;
		virtual bool has_physics() noexcept override;

	public:
		Vector<float>& pos() noexcept;

	public:
		void health(const int32_t& health) noexcept;
		const int64_t health() noexcept;

	public:
		void freeze_for(const int64_t& cooldown) noexcept;
		void freeze(const bool enable) noexcept;

	public:
		bool alive() noexcept;
		bool is_frozen() noexcept;

	private:
		Vector<float> m_position;
		NetworkPeer* mPeer;

	private:
		int64_t mCooldown;
		int64_t mDeathTimeout;
		int64_t mHealth;
		bool mFrozen;

	};

	using PlayerArray = std::vector<PlayerComponent*>;
}