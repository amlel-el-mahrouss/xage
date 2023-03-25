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
				auto x = m_packet.speed[XPLICIT_NETWORK_X];
				auto y = m_packet.speed[XPLICIT_NETWORK_Y];
				auto z = m_packet.speed[XPLICIT_NETWORK_Z];

				m_packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_INVALID;
				m_packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_INVALID;

				auto forward = m_packet.cmd[XPLICIT_NETWORK_CMD_FORWARD];
				auto backward = m_packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD];
				auto left = m_packet.cmd[XPLICIT_NETWORK_CMD_LEFT];
				auto right = m_packet.cmd[XPLICIT_NETWORK_CMD_RIGHT];
				auto jump = m_packet.cmd[XPLICIT_NETWORK_CMD_JUMP];

				m_packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_INVALID;
				m_packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_INVALID;
				m_packet.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;
				m_packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_INVALID;
				m_packet.cmd[XPLICIT_NETWORK_CMD_JUMP] = NETWORK_CMD_INVALID;

				m_network->send(m_packet);

				f32 delta = (IRR->getTimer()->getTime() - m_then) / XPLICIT_DELTA_SECOND;
				auto pos = m_pNode->getPosition();

				if (forward == NETWORK_CMD_FORWARD)
				{
					pos.Z -= z * delta;
					m_pNode->setPosition(pos);

					return;
				}

				if (backward == NETWORK_CMD_BACKWARD)
				{
					pos.Z += z * delta;
					m_pNode->setPosition(pos);

					return;
				}

				if (left == NETWORK_CMD_LEFT)
				{
					pos.X -= y * delta;
					m_pNode->setPosition(pos);

					return;
				}

				if (right == NETWORK_CMD_RIGHT)
				{
					pos.X += y * delta;
					m_pNode->setPosition(pos);

					return;
				}

				if (jump == NETWORK_CMD_JUMP)
				{
					pos.Y += x * delta;
					m_pNode->setPosition(pos);

					return;
				}
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

		if (KB->key_down(Details::KEY_KEY_W))
		{

			XPLICIT_SEND_POS_CMD(XPLICIT_NETWORK_CMD_FORWARD, NETWORK_CMD_FORWARD);
		}

		if (KB->key_down(Details::KEY_KEY_S))
		{
			XPLICIT_SEND_POS_CMD(XPLICIT_NETWORK_CMD_BACKWARD, NETWORK_CMD_BACKWARD);
		}

		if (KB->key_down(Details::KEY_KEY_D))
		{
			XPLICIT_SEND_POS_CMD(XPLICIT_NETWORK_CMD_RIGHT, NETWORK_CMD_RIGHT);
		}

		if (KB->key_down(Details::KEY_KEY_A))
		{
			XPLICIT_SEND_POS_CMD(XPLICIT_NETWORK_CMD_LEFT, NETWORK_CMD_LEFT);
		}
	}
}

#undef XPLICIT_SEND_POS_CMD