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

		XPLICIT_SEND_POS_CMD(XPLICIT_NETWORK_CMD_BACKWARD, NETWORK_CMD_BACKWARD);
		XPLICIT_SEND_POS_CMD(XPLICIT_NETWORK_CMD_BACKWARD, NETWORK_CMD_BACKWARD);
		XPLICIT_SEND_POS_CMD(XPLICIT_NETWORK_CMD_BACKWARD, NETWORK_CMD_BACKWARD);
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
				f32 delta = (IRR->getTimer()->getTime() - m_then) / XPLICIT_DELTA_SECOND;

				auto pos = m_pNode->getPosition();

				if (m_packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
					pos.Y -= m_packet.speed[XPLICIT_NETWORK_X] * delta;

				if (m_packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
					pos.Y += m_packet.speed[XPLICIT_NETWORK_X] * delta;

				if (m_packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
					pos.X -= m_packet.speed[XPLICIT_NETWORK_Y] * delta;

				if (m_packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
					pos.X += m_packet.speed[XPLICIT_NETWORK_Y];

				if (m_packet.cmd[XPLICIT_NETWORK_CMD_JUMP] == NETWORK_CMD_JUMP)
					pos.Z += m_packet.speed[XPLICIT_NETWORK_Z] * delta;

				m_packet.public_hash = -1;
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
		if (!m_network ||
			m_public_hash == -1)
			return;

	}
}

#undef XPLICIT_SEND_POS_CMD