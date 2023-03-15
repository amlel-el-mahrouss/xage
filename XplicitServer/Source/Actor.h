/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Actor.h
 *			Purpose: Actor Logic
 *
 * =====================================================================
 */

#pragma once

#include "SDK.h"

namespace Xplicit
{
	class Actor final : public Instance
	{
	public:
		Actor();
		virtual ~Actor();

		Actor& operator=(const Actor&) = default;
		Actor(const Actor&) = default;

	public:
		void health(const int32_t& health) noexcept;
		const int64_t health() noexcept; // gets the health of the actor.

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

	private:
		NetworkPeer* m_peer;
		int64_t m_health;

	};
}