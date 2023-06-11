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
	static bool xplicit_join_event(NetworkPeer* peer, PlayerComponent* player, NetworkServerComponent* server);
	static bool xplicit_leave_event(NetworkPeer* peer, NetworkServerComponent* server);

	static size_t xplicit_hash_from_uuid(const uuids::uuid& uuid);

	static size_t xplicit_hash_from_uuid(const uuids::uuid& uuid)
	{
		std::string uuid_str = uuids::to_string(uuid);
		auto hash = std::hash<std::string>();
		auto res = hash(uuid_str);

		return res;
	}

	static bool xplicit_join_event(NetworkPeer* peer, PlayerComponent* player, NetworkServerComponent* server)
	{
		if (!peer ||
			!player ||
			!server)
			return false;

		auto hash = xplicit_hash_from_uuid(peer->unique_addr.get());

		// I use version 4, to avoid collisions.
		auto public_hash_uuid = UUIDFactory::version<4>();

		peer->public_hash = xplicit_hash_from_uuid(public_hash_uuid);
		peer->hash = hash;

		player->set(peer);

		peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
		peer->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;

		peer->packet.public_hash = peer->public_hash;
		peer->packet.hash = hash;

		peer->packet.size = sizeof(NetworkPacket);
		peer->stat = NETWORK_STAT_CONNECTED;

		NetworkServerTraits::send(server);

		return true;
	}

	static bool xplicit_leave_event(NetworkPeer* peer, NetworkServerComponent* server)
	{
		auto actors = ComponentManager::get_singleton_ptr()->all_of<PlayerComponent>("Player");

		for (size_t at = 0; at < actors.size(); ++at)
		{
			if (actors[at]->get() == peer)
			{
				for (size_t peer_idx = 0; peer_idx < server->size(); ++peer_idx)
				{
					if (server->get(peer_idx) != peer)
					{
						server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
						server->get(peer_idx)->packet.public_hash = actors[at]->get()->public_hash;
					}
				}

				ComponentManager::get_singleton_ptr()->remove<PlayerComponent>(actors[at]);
				peer->reset();

				return true;
			}
		}

		return false;
	}

	PlayerJoinLeaveEvent::PlayerJoinLeaveEvent() : m_player_size(0) {}
	PlayerJoinLeaveEvent::~PlayerJoinLeaveEvent() = default;

	void PlayerJoinLeaveEvent::operator()()
	{
		auto server = ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");
		XPLICIT_ASSERT(server);

		NetworkServerTraits::find_and_correct(server);

		for (std::size_t index = 0UL; index < server->size(); ++index)
		{
			if (server->get(index)->stat == NETWORK_STAT_DISCONNECTED)
				continue;

			if (server->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] != Xplicit::NETWORK_CMD_ACK &&
				server->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] != Xplicit::NETWORK_CMD_BEGIN)
			{
				server->get(index)->bad = true;
			}

			if (server->get(index)->bad)
				server->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = Xplicit::NETWORK_CMD_KICK;
		}


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
				continue;

			if (server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN &&
				server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] == NETWORK_CMD_ACK)
			{
				PlayerComponent* player = ComponentManager::get_singleton_ptr()->add<PlayerComponent>();
				XPLICIT_ASSERT(player);

				if (xplicit_join_event(server->get(peer_idx), player, server))
				{
					XPLICIT_INFO("[CONNECT] Player ID: " + uuids::to_string(server->get(peer_idx)->unique_addr.get()));
					++m_player_size;

					return true;
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
					XPLICIT_INFO("[DISCONNECT] Player ID: " + uuids::to_string(server->get(peer_idx)->unique_addr.get()));
					--m_player_size;

					return true;
				}
			}
		}

		return false;
	}

	const char* PlayerJoinLeaveEvent::name() noexcept { return ("PlayerJoinLeaveEvent"); }
}