/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalActor.cpp
 *			Purpose: Client-side Actors
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Application.h"
#include "LocalActor.h"
#include "CoreUI.h"

namespace Xplicit::Client
{
	LocalActor::LocalActor(const int64_t& public_hash)
		: Component(), MeshComponent("../Data/Studio/Character.dae"), m_packet(), m_camera(nullptr), m_public_hash(public_hash)
	{
		m_network = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(m_network);

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalActor::LocalActor");
#endif
	}

	LocalActor::~LocalActor()
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalActor::~LocalActor");
#endif

	}

	void LocalActor::update()
	{
		if (!m_network)
			return;

		auto& pckt = m_network->get();
		
		if (pckt.public_hash == m_public_hash)
		{
			if (pckt.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS)
			{
			}
		}
	}

	void LocalActor::attach(CameraComponent* cam) noexcept { m_camera = cam; }

	LocalMoveEvent::LocalMoveEvent(const int64_t& public_hash)
		: m_packet(), m_network(nullptr), m_public_hash(public_hash)
	{
		m_network = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(m_network);
	}

	LocalMoveEvent::~LocalMoveEvent() {}

	const char* LocalMoveEvent::name() noexcept { return ("LocalMoveEvent"); }

	void LocalMoveEvent::operator()()
	{
		if (!m_network)
			return;

		XPLICIT_ASSERT(m_public_hash != -1);
		m_packet.public_hash = m_public_hash;

		if (KB->key_down(Details::KEY_KEY_W) ||
			KB->key_down(Details::KEY_KEY_S) ||
			KB->key_down(Details::KEY_KEY_A) ||
			KB->key_down(Details::KEY_KEY_D))
		{
			m_packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
			m_network->send(m_packet);
		}

	}
}