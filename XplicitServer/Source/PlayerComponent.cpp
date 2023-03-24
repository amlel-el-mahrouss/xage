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

#include "PlayerComponent.h"

namespace Xplicit
{
	constexpr const int16_t XPLICIT_PLAYER_COOLDOWN = 100;
	constexpr const int64_t XPLICIT_DEFAULT_HEALTH = 100;

	PlayerComponent::PlayerComponent() 
		: Component(), m_peer(nullptr), m_health(XPLICIT_DEFAULT_HEALTH), m_timeout(0), Position(0, 0, 0), Rotation(0, 0, 0, 0)
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("PlayerComponent::PlayerComponent");
#endif
	}

	PlayerComponent::~PlayerComponent() 
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("PlayerComponent::~PlayerComponent");
#endif
	}

	PlayerComponent::PHYSICS_TYPE PlayerComponent::physics() noexcept 
	{
		return PHYSICS_NONE; 
	}

	bool PlayerComponent::has_physics() noexcept 
	{ 
		return false; 
	}

	void PlayerComponent::update()
	{
		if (!m_peer)
			return;

		if (m_peer->packet.cmd[XPLICIT_NETWORK_CMD_DEAD] == NETWORK_CMD_DEAD)
			m_timeout = XPLICIT_PLAYER_COOLDOWN;
		
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

	void PlayerComponent::health(const int32_t& health) noexcept 
	{ 
		this->m_health = health; 
	}

	const int64_t PlayerComponent::health() noexcept 
	{ 
		return this->m_health; 
	}

	bool PlayerComponent::can_collide() noexcept 
	{
		return true; 
	}

	PlayerComponent::INSTANCE_TYPE PlayerComponent::type() noexcept { return INSTANCE_PLAYER; }

	const char* PlayerComponent::name() noexcept { return "Player"; }

	bool PlayerComponent::should_update() noexcept 
	{ 
		return true; 
	}

	NetworkPeer* PlayerComponent::get() noexcept 
	{ 
		return m_peer; 
	}

	void PlayerComponent::set(NetworkPeer* peer) noexcept 
	{
		XPLICIT_ASSERT(peer);
		m_peer = peer;
	}
}