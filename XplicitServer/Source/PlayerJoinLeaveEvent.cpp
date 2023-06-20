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
 
	@file PlayerJoinLeaveEvent.cpp

 */

#include "PlayerJoinLeaveEvent.h"

namespace Xplicit
{
	static void xplicit_create_notify_join(NetworkPeer* peer, PlayerComponent* player, NetworkServerComponent* server);

	static size_t xplicit_hash_from_uuid(const uuids::uuid& uuid);

	static size_t xplicit_hash_from_uuid(const uuids::uuid& uuid)
	{
		std::string uuid_str = uuids::to_string(uuid);
		auto hash = std::hash<std::string>();
		auto res = hash(uuid_str);

		return res;
	}

	static void xplicit_create_notify_join(NetworkPeer* peer, PlayerComponent* player, NetworkServerComponent* server)
	{
		auto hash = xplicit_hash_from_uuid(peer->unique_addr.get());

		// I use version 4, to avoid collisions.
		auto public_hash_uuid = UUIDFactory::version<4>();

		peer->public_hash = xplicit_hash_from_uuid(public_hash_uuid);
		peer->hash = hash;

		peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
		peer->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;

		peer->packet.size = sizeof(NetworkPacket);

		for (std::size_t peer_idx = 0; peer_idx < server->size(); ++peer_idx)
		{
			if (server->get(peer_idx)->hash != hash)
			{
				peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
				peer->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;

				peer->packet.public_hash = peer->public_hash;
			}
		}
	}

	PlayerJoinLeaveEvent::PlayerJoinLeaveEvent() 
		: mNetwork(ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")) 
	{
		for (std::size_t _ = 0UL; _ < XPLICIT_MAX_CONNECTIONS; ++_)
		{
			PlayerComponent* player = ComponentManager::get_singleton_ptr()->add<PlayerComponent>();
			XPLICIT_ASSERT(player);

			mPlayers.push_back(player);
		}
	}

	PlayerJoinLeaveEvent::~PlayerJoinLeaveEvent() = default;

	void PlayerJoinLeaveEvent::operator()()
	{
		for (std::size_t index = 0; index < mNetwork->size(); ++index)
		{
			//! joining
			if (mNetwork->get(index)->stat == NETWORK_STAT_DISCONNECTED)
			{
				if (mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN)
				{
					PlayerComponent* player = mPlayers[index];
					xplicit_create_notify_join(mNetwork->get(index), player, mNetwork);

					mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_INVALID;

					XPLICIT_INFO("Connected");

					mNetwork->get(index)->stat = NETWORK_STAT_CONNECTED;
				}
			}
			//! leaving
			else if (mNetwork->get(index)->stat == NETWORK_STAT_CONNECTED)
			{
				if (mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP ||
					mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
				{
					PlayerComponent* player = mPlayers[index];

					player->reset();

					mNetwork->get(index)->reset();
					mNetwork->get(index)->unique_addr.invalidate();

					mNetwork->get(index)->stat = NETWORK_STAT_INVALID;

					XPLICIT_INFO("Disconnected");

					if (mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP)
						mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_INVALID;

					if (mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
						mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_INVALID;
				}
			}
			else if (mNetwork->get(index)->stat == NETWORK_STAT_INVALID)
			{
				continue;
			}
		}
	}

	const size_t PlayerJoinLeaveEvent::size() noexcept { return mPlayers.size(); }

	const char* PlayerJoinLeaveEvent::name() noexcept { return ("PlayerJoinLeaveEvent"); }
}