/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 
	@file PlayerJoinLeaveEvent.cpp

 */

#include "PlayerJoinLeaveEvent.h"

namespace Xplicit
{
	//!forward decls
	static void xplicit_on_join(NetworkInstance* peer, PlayerComponent* player, NetworkServerComponent* server);
	static size_t xplicit_hash_from_uuid(const uuids::uuid& uuid);

	static size_t xplicit_hash_from_uuid(const uuids::uuid& uuid)
	{
		std::string uuid_str = uuids::to_string(uuid);
		auto hash = std::hash<std::string>();
		auto res = hash(uuid_str);

		return res;
	}

	static void xplicit_on_join(NetworkInstance* peer, PlayerComponent* player, NetworkServerComponent* server)
	{
		auto hash = xplicit_hash_from_uuid(peer->unique_addr.get());

		auto public_hash_uuid = UUIDFactory::version<4>();

		peer->public_hash = xplicit_hash_from_uuid(public_hash_uuid);
		peer->hash = hash;

		peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
		peer->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;

		peer->packet.size = sizeof(NetworkPacket);
		peer->status = NETWORK_STAT_CONNECTED;

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
		: 
		mPlayerCount(0), 
		mNetwork(ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")) 
	{
		for (std::size_t index = 0UL; index < XPLICIT_MAX_CONNECTIONS; ++index)
		{
			PlayerComponent* compPly = ComponentManager::get_singleton_ptr()->add<PlayerComponent>();
			XPLICIT_ASSERT(compPly);

			mPlayers.push_back(compPly);
		}
	}

	PlayerJoinLeaveEvent::~PlayerJoinLeaveEvent() = default;

	void PlayerJoinLeaveEvent::operator()()
	{
		if (!mNetwork)
			mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");

		this->handle_join_event();
		this->handle_leave_event();
	}

	const size_t& PlayerJoinLeaveEvent::size() noexcept { return mPlayerCount; }

	bool PlayerJoinLeaveEvent::handle_join_event() noexcept
	{
		for (size_t peer_idx = 0; peer_idx < mNetwork->size(); ++peer_idx)
		{
			if (mNetwork->get(peer_idx)->status == NETWORK_STAT_CONNECTED)
				continue;

			if (mNetwork->get(peer_idx)->packet.size < 1)
				continue;

			if (mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN &&
				mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] == NETWORK_CMD_ACK)
			{
				if (this->size() > XPLICIT_MAX_CONNECTIONS)
				{
					mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
				}
				else
				{
					PlayerComponent* player = mPlayers[mPlayerCount];

					xplicit_on_join(mNetwork->get(peer_idx), player, mNetwork);
					player->set(mNetwork->get(peer_idx));

					XPLICIT_INFO("[CONNECT] UUID: " + uuids::to_string(mNetwork->get(peer_idx)->unique_addr.get()));

					++mPlayerCount;
				}
			}
		}

		return true;
	}

	//! Decreases and free player resources.
	/** Used by server to hook join and leave events. */

	bool PlayerJoinLeaveEvent::handle_leave_event() noexcept
	{
		if (this->size() < 1) return false;
		if (!mNetwork) return false;

		for (size_t peer_idx = 0; peer_idx < mNetwork->size(); ++peer_idx)
		{
			if (mNetwork->get(peer_idx)->status == NETWORK_STAT_DISCONNECTED)
				continue;

			if (mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP ||
				mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
			{
				if (mNetwork->get(peer_idx)->packet.hash == mNetwork->get(peer_idx)->hash)
				{
					XPLICIT_INFO("[DISCONNECT] UUID: " + uuids::to_string(mNetwork->get(peer_idx)->unique_addr.get()));

					const auto public_hash = mNetwork->get(peer_idx)->public_hash;

					mNetwork->get(peer_idx)->unique_addr.invalidate();
					mNetwork->get(peer_idx)->reset();

					for (std::size_t index = 0; index < mNetwork->size(); ++index)
					{
						if (mNetwork->get(index)->status == NETWORK_STAT_CONNECTED)
						{
							mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
							mNetwork->get(index)->packet.public_hash = public_hash;
						}
					}

					--mPlayerCount;
				}
			}
		}

		return true;
	}

	const char* PlayerJoinLeaveEvent::name() noexcept { return ("PlayerJoinLeaveEvent"); }
}