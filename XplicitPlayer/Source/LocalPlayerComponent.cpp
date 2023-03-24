/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalActor.cpp
 *			Purpose: Client-side Actor
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Application.h"
#include "LocalPlayerComponent.h"
#include "CoreUI.h"

namespace Xplicit::Client
{
	LocalPlayerComponent::LocalPlayerComponent(const int64_t& public_hash)
		: Component(), MeshComponent("Character.dae"), m_packet(), m_camera(nullptr), m_public_hash(public_hash)
	{
		m_network = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(m_network);

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalActor::LocalActor");
#endif
	}

	LocalPlayerComponent::~LocalPlayerComponent()
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalActor::~LocalActor");
#endif

	}

	LocalPlayerComponent::INSTANCE_TYPE LocalPlayerComponent::type() noexcept { return INSTANCE_ACTOR; }

	const char* LocalPlayerComponent::name() noexcept { return ("LocalPlayerComponent"); }

	void LocalPlayerComponent::update()
	{
		if (!m_network)
			return;

		m_packet = m_network->get();

		if (m_packet.public_hash == m_public_hash)
		{
			if (m_packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT &&
				m_packet.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS)
			{
				auto pos = m_pNode->getPosition();

				if (m_packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
					pos.X += m_packet.speed[XPLICIT_NETWORK_X];

				if (m_packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
					pos.X -= m_packet.speed[XPLICIT_NETWORK_X];

				if (m_packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
					pos.Y -= m_packet.speed[XPLICIT_NETWORK_Y];

				if (m_packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
					pos.Y += m_packet.speed[XPLICIT_NETWORK_Y];

				if (m_packet.cmd[XPLICIT_NETWORK_CMD_JUMP] == NETWORK_CMD_JUMP)
					pos.Z += m_packet.speed[XPLICIT_NETWORK_Z];

				m_pNode->setPosition(pos);
			}
		}
	}

	void LocalPlayerComponent::attach(CameraComponent* cam) noexcept { m_camera = cam; }

	LocalMoveEvent::LocalMoveEvent(const int64_t& public_hash)
		: m_packet(), m_network(nullptr), m_public_hash(public_hash)
	{
		m_network = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(m_network);
	}

	LocalMoveEvent::~LocalMoveEvent() {}

	const char* LocalMoveEvent::name() noexcept { return ("LocalMoveEvent"); }

#define XPLICIT_SEND_NETWORK_CMD(SLOT, CMD)\
	XPLICIT_ASSERT(m_public_hash != -1);\
	m_packet.public_hash = m_public_hash;\
\
	m_packet.cmd[SLOT] = CMD;\
	m_network->send(m_packet);\



	void LocalMoveEvent::operator()()
	{
		if (!m_network)
			return;

		if (KB->key_down(Details::KEY_KEY_W))
		{
			XPLICIT_SEND_NETWORK_CMD(XPLICIT_NETWORK_CMD_FORWARD, NETWORK_CMD_FORWARD);
		}
		else if (KB->key_down(Details::KEY_KEY_S))
		{
			XPLICIT_SEND_NETWORK_CMD(XPLICIT_NETWORK_CMD_BACKWARD, NETWORK_CMD_BACKWARD);
		}
		else if (KB->key_down(Details::KEY_KEY_A))
		{
			XPLICIT_SEND_NETWORK_CMD(XPLICIT_NETWORK_CMD_LEFT, NETWORK_CMD_LEFT);
		}
		else if (KB->key_down(Details::KEY_KEY_D))
		{
			XPLICIT_SEND_NETWORK_CMD(XPLICIT_NETWORK_CMD_RIGHT, NETWORK_CMD_RIGHT);
		}
	}

#undef XPLICIT_SEND_NETWORK_CMD
}
