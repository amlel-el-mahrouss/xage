/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Actor.cpp
 *			Purpose: Server-Side Actors
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Actor.h"

namespace Xplicit
{
	constexpr const int16_t XPLICIT_ACTOR_COOLDOWN = 100;

	Actor::Actor() 
		: Component(), m_peer(nullptr), m_health(0), m_timeout(0) 
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("Actor::Actor");
#endif
	}

	Actor::~Actor() 
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("Actor::~Actor");
#endif
	}

	Actor::PHYSICS_TYPE Actor::physics() noexcept 
	{
		return PHYSICS_NONE; 
	}

	bool Actor::has_physics() noexcept 
	{ 
		return false; 
	}

	void Actor::update()
	{
		if (!m_peer)
			return;

		if (m_peer->packet.cmd[XPLICIT_NETWORK_CMD_DEAD] == NETWORK_CMD_DEAD)
			m_timeout = XPLICIT_ACTOR_COOLDOWN;
		
		if (m_timeout > 0)
		{
			--m_timeout;

			if (m_timeout <= 0)
			{
				m_timeout = 0;
				this->health(100);
			}
		}
	}

	void Actor::health(const int32_t& health) noexcept 
	{ 
		this->m_health = health; 
	}

	const int64_t Actor::health() noexcept 
	{ 
		return this->m_health; 
	}

	bool Actor::can_collide() noexcept 
	{
		return true; 
	}

	Actor::INSTANCE_TYPE Actor::type() noexcept { return INSTANCE_ACTOR; }
	const char* Actor::name() noexcept { return "Actor"; }

	bool Actor::should_update() noexcept 
	{ 
		return true; 
	}

	NetworkPeer* Actor::get() noexcept 
	{ 
		return m_peer; 
	}

	void Actor::set(NetworkPeer* peer) noexcept 
	{
		XPLICIT_ASSERT(peer);
		m_peer = peer;
	}
}