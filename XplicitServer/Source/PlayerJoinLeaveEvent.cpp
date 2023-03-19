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

 /**
 @file
 */

#include "PlayerJoinLeaveEvent.h"

namespace Xplicit
{
	static size_t xplicit_hash_from_uuid(const uuids::uuid& uuid)
	{
		std::string uuid_str = uuids::to_string(uuid);
		auto hash = std::hash<std::string>();
		auto res = hash(uuid_str);

		return res;
	}

	static bool xplicit_join_event(NetworkPeer* cl, Actor* actor, NetworkServerComponent* server)
	{
		if (!cl ||
			!actor ||
			!server)
			return false;

		auto hash = xplicit_hash_from_uuid(cl->unique_addr.get());

		// I use version 4, to avoid collisions.
		auto public_hash_uuid = UUIDFactory::version<4>();
		cl->public_hash = xplicit_hash_from_uuid(public_hash_uuid);

		cl->hash = hash;

		actor->set(cl);

		cl->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
		cl->packet.public_hash = cl->public_hash;
		cl->packet.hash = hash;
		cl->packet.size = sizeof(UDPNetworkPacket);

		cl->stat = NETWORK_STAT_CONNECTED;

		return true;
	}

	static bool xplicit_leave_event(NetworkPeer* cl, NetworkServerComponent* server)
	{
		auto actors = ComponentManager::get_singleton_ptr()->all_of<Actor>("Actor");

		for (size_t at = 0; at < actors.size(); ++at)
		{
			if (actors[at]->get() == cl)
			{
				ComponentManager::get_singleton_ptr()->remove<Actor>(actors[at]);
				cl->reset();

				return true;
			}
		}

		return false;
	}

	PlayerJoinLeaveEvent::PlayerJoinLeaveEvent() : m_player_size(0) {}
	PlayerJoinLeaveEvent::~PlayerJoinLeaveEvent() {}

	void PlayerJoinLeaveEvent::operator()()
	{
		auto server = ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");

		if (!server)
			return;

		NetworkServerTraits::correct_collisions(server);

		this->on_join(server);
		this->on_leave(server);
	}

	const size_t& PlayerJoinLeaveEvent::size() noexcept { return m_player_size; }

	bool PlayerJoinLeaveEvent::on_join(NetworkServerComponent* server) noexcept
	{
		if (this->size() > XPLICIT_MAX_CONNECTIONS)
			return false;

		if (!server)
			return false;

		for (size_t peer_idx = 0; peer_idx < server->size(); ++peer_idx)
		{
			if (server->get(peer_idx)->stat == NETWORK_STAT_CONNECTED)
				continue;

			if (server->get(peer_idx)->packet.size < 1)
			{
				server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
				continue;
			}

			if (server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN &&
				server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] == NETWORK_CMD_ACK)
			{
				auto actor = ComponentManager::get_singleton_ptr()->add<Actor>();

				if (!actor)
					return false;

				if (xplicit_join_event(server->get(peer_idx), actor, server))
				{
					XPLICIT_INFO("[CONNECT] Unique ID: " + uuids::to_string(server->get(peer_idx)->unique_addr.get()));
					++m_player_size;
				}

			}
		}

		return false;
	}

	bool PlayerJoinLeaveEvent::on_leave(NetworkServerComponent* server) noexcept
	{
		if (this->size() <= 0)
			return false;

		if (!server)
			return false;

		for (size_t peer_idx = 0; peer_idx < server->size(); ++peer_idx)
		{
			if (server->get(peer_idx)->stat == NETWORK_STAT_DISCONNECTED)
				continue;

			if (server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP ||
				server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
			{
				if (xplicit_leave_event(server->get(peer_idx), server))
				{
					XPLICIT_INFO("[DISCONNECT] Unique ID: " + uuids::to_string(server->get(peer_idx)->unique_addr.get()));
					--m_player_size;
				}
			}
		}

		return false;
	}

	const char* PlayerJoinLeaveEvent::name() noexcept { return ("PlayerJoinLeaveEvent"); }
}