/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: PlayerJoinLeaveEvent.h
 *			Purpose: Player Join and Leave Event (duh x2)
 *
 * =====================================================================
 */

#include "PlayerJoinLeaveEvent.h"

namespace Xplicit
{
	static size_t xplicit_hash_from_uuid(uuids::uuid& uuid)
	{
		std::string uuid_str = uuids::to_string(uuid);
		auto hash = std::hash<std::string>();
		auto res = hash(uuid_str);
		
		return res;
	}

	static void xplicit_join_event(NetworkPeer* cl, Actor* actor, NetworkServerInstance* server)
	{
		auto hash = xplicit_hash_from_uuid(cl->unique_addr.uuid);
		cl->hash = hash;

		actor->set(cl);

		cl->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;

		cl->packet.hash = hash;
		cl->stat = NETWORK_STAT_CONNECTED;
	}

	static bool xplicit_leave_event(NetworkPeer* cl, NetworkServerInstance* server)
	{
		auto actors = InstanceManager::get_singleton_ptr()->get_all<Actor>("Actor");

		for (size_t at = 0; at < actors.size(); ++at)
		{
			if (actors[at]->get() == cl)
			{
				cl->reset();
				return true;
			}
		}

		return false;
	}

	PlayerJoinLeaveEvent::PlayerJoinLeaveEvent() : m_size(0), m_locked(false) {}
	PlayerJoinLeaveEvent::~PlayerJoinLeaveEvent() {}

	void PlayerJoinLeaveEvent::operator()()
	{
		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");

		if (!server)
			return;

		for (size_t peer = 0; peer < server->size(); ++peer)
		{
			if (this->size() > XPLICIT_MAX_CONNECTIONS)
				break;

			this->join_event(server, peer);
		}

		this->leave_event(server);
	}

	const size_t& PlayerJoinLeaveEvent::size() noexcept { return m_size; }

	bool PlayerJoinLeaveEvent::join_event(NetworkServerInstance* server, size_t peer_idx) noexcept
	{
		if (!server) return false;
		if (m_locked) return false;

		m_locked = true;

		if (server->get(peer_idx)->stat == NETWORK_STAT_CONNECTED)
		{
			m_locked = false;
			return false;
		}

		if (server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN &&
			server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] == NETWORK_CMD_ACK)
		{
			auto actor = InstanceManager::get_singleton_ptr()->add<Actor>();

			if (!actor)
				throw EngineError();

			++m_size;
			xplicit_join_event(server->get(peer_idx), actor, server);

			XPLICIT_INFO("[CONNECT] Unique ID: " + uuids::to_string(server->get(peer_idx)->unique_addr.uuid));

			m_locked = false;

			return true;
		}

		m_locked = false;

		return false;
	}

	bool PlayerJoinLeaveEvent::leave_event(NetworkServerInstance* server) noexcept
	{
		if (this->size() < 0)
			return false;

		if (!server)
			return false;

		for (size_t peer_idx = 0; peer_idx < server->size(); ++peer_idx)
		{
			if (server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP ||
				server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
			{
				if (xplicit_leave_event(server->get(peer_idx), server))
				{
					--m_size;
					XPLICIT_INFO("[DISCONNECT] Unique ID: " + uuids::to_string(server->get(peer_idx)->unique_addr.uuid));
				}
			}
		}

		return false;
	}

	const char* PlayerJoinLeaveEvent::name() noexcept { return ("PlayerJoinLeaveEvent"); }
}