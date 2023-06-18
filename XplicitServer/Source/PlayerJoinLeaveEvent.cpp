/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
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
	static void xplicit_on_join(NetworkPeer* peer, PlayerComponent* player, NetworkServerComponent* server);
	static void xplicit_on_leave(NetworkPeer* peer, NetworkServerComponent* server);

	static size_t xplicit_hash_from_uuid(const uuids::uuid& uuid);

	static size_t xplicit_hash_from_uuid(const uuids::uuid& uuid)
	{
		std::string uuid_str = uuids::to_string(uuid);
		auto hash = std::hash<std::string>();
		auto res = hash(uuid_str);

		return res;
	}

	static void xplicit_on_join(NetworkPeer* peer, PlayerComponent* player, NetworkServerComponent* server)
	{
		auto hash = xplicit_hash_from_uuid(peer->unique_addr.get());

		// I use version 4, to avoid collisions.
		auto public_hash_uuid = UUIDFactory::version<4>();

		peer->public_hash = xplicit_hash_from_uuid(public_hash_uuid);
		peer->hash = hash;

		peer->packet.public_hash = peer->public_hash;
		peer->packet.hash = hash;

		peer->packet.size = sizeof(NetworkPacket);

		peer->stat = NETWORK_STAT_CONNECTED;

		player->set(peer);

		peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
		peer->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;

		NetworkServerHelper::send(server);
	}

	static void xplicit_on_leave(NetworkPeer* peer, NetworkServerComponent* server)
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

				break;
			}
		}

		peer->stat = NETWORK_STAT_DISCONNECTED;
	}

	PlayerJoinLeaveEvent::PlayerJoinLeaveEvent() : mPlayerCount(0) {}
	PlayerJoinLeaveEvent::~PlayerJoinLeaveEvent() = default;

	void PlayerJoinLeaveEvent::operator()()
	{
		auto server = ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");
		XPLICIT_ASSERT(server);

		this->handle_join_event(server);
		this->handle_leave_event(server);
	}

	const size_t& PlayerJoinLeaveEvent::size() noexcept { return mPlayerCount; }

	bool PlayerJoinLeaveEvent::handle_join_event(NetworkServerComponent* server) noexcept
	{
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
				if (this->size() > XPLICIT_MAX_CONNECTIONS)
				{
					server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
					continue;
				}

				NetworkServerHelper::correct(server);

				PlayerComponent* player = ComponentManager::get_singleton_ptr()->add<PlayerComponent>();
				XPLICIT_ASSERT(player);

				xplicit_on_join(server->get(peer_idx), player, server);
				std::cout << server->get(peer_idx)->hash << std::endl;

				XPLICIT_INFO("[CONNECT] UUID: " + uuids::to_string(server->get(peer_idx)->unique_addr.get()));

				++mPlayerCount;
			}
		}

		return true;
	}

	bool PlayerJoinLeaveEvent::handle_leave_event(NetworkServerComponent* server) noexcept
	{
		if (this->size() < 1)
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
				if (server->get(peer_idx)->hash == server->get(peer_idx)->packet.hash)
				{
					std::cout << server->get(peer_idx)->hash << std::endl;

					xplicit_on_leave(server->get(peer_idx), server);

					XPLICIT_INFO("[DISCONNECT] UUID: " + uuids::to_string(server->get(peer_idx)->unique_addr.get()));

					--mPlayerCount;

					server->get(peer_idx)->reset();
				}
			}
		}

		return true;
	}

	const char* PlayerJoinLeaveEvent::name() noexcept { return ("PlayerJoinLeaveEvent"); }
}