/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalActor.cpp
 *			Purpose: Client-side Actor Instance
 *
 * =====================================================================
 */

#include "Application.h"
#include "LocalActor.h"
#include "CoreUI.h"

namespace Xplicit::Client
{
	LocalActor::LocalActor(const int64_t& id, const int64_t& public_id)
		: Instance(), m_packet(), m_id(-1), m_camera(nullptr), m_public_id(-1)
	{
		m_id = id;
		m_public_id = public_id;

		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");

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
		// TODO:
	}

	void LocalActor::attach(CameraInstance* cam) noexcept { m_camera = cam; }

	LocalMoveEvent::LocalMoveEvent(const int64_t& id, const int64_t& public_id)
		: m_packet(), m_network(nullptr), m_id(id), m_public_id(public_id)
	{
		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");
		XPLICIT_ASSERT(m_network);
	}

	LocalMoveEvent::~LocalMoveEvent() {}

	const char* LocalMoveEvent::name() noexcept { return ("LocalMoveEvent"); }

	void LocalMoveEvent::operator()()
	{
		if (!m_network)
			return;

		XPLICIT_ASSERT(m_public_id != -1);
		m_packet.public_hash = m_public_id;

		if (KB->key_down(KEY_KEY_W))
			m_packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_FORWARD;

		if (KB->key_down(KEY_KEY_S))
			m_packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_BACKWARDS;

		if (KB->key_down(KEY_KEY_A))
			m_packet.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_LEFT;

		if (KB->key_down(KEY_KEY_D))
			m_packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_RIGHT;

		m_network->send(m_packet);
	}
}