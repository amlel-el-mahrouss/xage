/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalActor.cpp
 *			Purpose: Client-side Player
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalPlayerComponent.h"
#include "Application.h"
#include "CoreUI.h"

#include <Common.h>

#define XPLICIT_SEND_POS_CMD(SLOT, CMD)\
	m_packet.public_hash = m_public_hash;\
	m_packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;\
	m_packet.cmd[SLOT] = CMD;\
	m_network->send(m_packet);\


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

		m_then = IRR->getTimer()->getTime();
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
		if (!m_network ||
			m_public_hash == -1)
			return;

		m_packet = m_network->get();

		if (m_packet.public_hash == m_public_hash)
		{
			if (m_packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT &&
				m_packet.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS)
			{
				auto x_speed = m_packet.speed[XPLICIT_NETWORK_X];
				auto z_speed = m_packet.speed[XPLICIT_NETWORK_Z];

				m_packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_INVALID;

				m_network->send(m_packet);

				f32 delta = (IRR->getTimer()->getTime() - m_then) / XPLICIT_DELTA_TIME;

				auto pos = m_pNode->getAbsolutePosition();

				if (m_packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
					pos.Z -= z_speed * delta;
				
				if (m_packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
					pos.Z += z_speed * delta;
				
				if (m_packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
					pos.X -= x_speed * delta;
				
				if (m_packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
					pos.X += x_speed * delta;

#ifdef XPLICIT_DEBUG
				XPLICIT_INFO("Z:" + std::to_string(pos.Z));
				XPLICIT_INFO("X:" + std::to_string(pos.X));

				XPLICIT_INFO("NETWORK_CMD_RIGHT:" + std::to_string(m_packet.cmd[XPLICIT_NETWORK_CMD_RIGHT]));
				XPLICIT_INFO("NETWORK_CMD_LEFT:" + std::to_string(m_packet.cmd[XPLICIT_NETWORK_CMD_LEFT]));
				XPLICIT_INFO("NETWORK_CMD_BACKWARD:" + std::to_string(m_packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD]));
				XPLICIT_INFO("NETWORK_CMD_FORWARD:" + std::to_string(m_packet.cmd[XPLICIT_NETWORK_CMD_FORWARD]));
#endif // XPLICIT_DEBUG

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

	void LocalMoveEvent::operator()()
	{
		if (!m_network || m_public_hash == -1)
			return;

		m_packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_INVALID;
		m_packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_INVALID;
		m_packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_INVALID;
		m_packet.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;

		if (KB->key_down(Details::KEY_KEY_W))
		{
			XPLICIT_SEND_POS_CMD(XPLICIT_NETWORK_CMD_FORWARD, NETWORK_CMD_FORWARD);
			return;
		}

		if (KB->key_down(Details::KEY_KEY_S))
		{
			XPLICIT_SEND_POS_CMD(XPLICIT_NETWORK_CMD_BACKWARD, NETWORK_CMD_BACKWARD);
			return;
		}

		if (KB->key_down(Details::KEY_KEY_D))
		{
			XPLICIT_SEND_POS_CMD(XPLICIT_NETWORK_CMD_RIGHT, NETWORK_CMD_RIGHT);
			return;
		}

		if (KB->key_down(Details::KEY_KEY_A))
		{
			XPLICIT_SEND_POS_CMD(XPLICIT_NETWORK_CMD_LEFT, NETWORK_CMD_LEFT);
			return;
		}
	}
}

#undef XPLICIT_SEND_POS_CMD