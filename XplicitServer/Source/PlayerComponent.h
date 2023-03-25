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

#include "SDK.h"

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
		Nplicit::Vector<float>& pos() noexcept;

		void health(const int32_t& health) noexcept;
		const int64_t health() noexcept;

		void freeze(const int64_t& cooldown) noexcept;
		bool is_frozen() noexcept;

	private:
		Nplicit::Vector<float> m_position;
		int64_t m_freeze_cooldown;
		int64_t m_death_timeout;
		NetworkPeer* m_peer;
		int64_t m_health;

	};

	using PlayerArray = std::vector<PlayerComponent*>;
}