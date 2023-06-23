/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 
	@file PlayerJoinEvent.cpp

 */

#include "PlayerLoginEvent.h"

#include "ServerReplicationManager.h"

namespace Xplicit
{
	/**
	 * \brief creates an hash from the GUID.
	 * \param uuid GUID (version 4 recommended)
	 * \return the hashed GUID.
	 */

	static size_t xplicit_hash()
	{
		const auto tim = xplicit_get_epoch();
		const auto hash = std::hash<time_t>();
		const auto res = hash(tim);

		return res;
	}

	/**
	 * \brief Setups and prepares the player for a join event.
	 * \param peer Network instance
	 * \param player Player component
	 * \param server Server component
	 */

	static bool xplicit_on_join(NetworkInstance* peer, PlayerComponent* player, const NetworkServerComponent* server)
	{
		if (!server ||
			!peer ||
			!player ||
			player->get() != nullptr)
			return false;
		
		peer->public_hash = xplicit_hash();
		peer->hash = xplicit_hash();

		peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
		peer->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;

		peer->packet.hash = peer->hash;
		peer->packet.size = sizeof(NetworkPacket);
		
		player->set_peer(peer);
		
		return true;
	}

	PlayerLoginEvent::PlayerLoginEvent()
		:
		mNetwork(ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
		mPlayerCount(0)
	{
		for (std::size_t index = 0UL; index < XPLICIT_MAX_CONNECTIONS; ++index)
		{
			PlayerComponent* component = ComponentManager::get_singleton_ptr()->add<PlayerComponent>();
			XPLICIT_ASSERT(component);

			mPlayers.push_back(component);
		}
	}

	PlayerLoginEvent::~PlayerLoginEvent() = default;

	void PlayerLoginEvent::handle_join_event() noexcept
	{
		if (this->size() > XPLICIT_MAX_CONNECTIONS)
			return;

		for (size_t peer_idx = 0; peer_idx < mNetwork->size(); ++peer_idx)
		{
			if (mNetwork->get(peer_idx)->status == NETWORK_STAT_CONNECTED)
				continue;

			if (mNetwork->get(peer_idx)->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			if (mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN &&
				mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] == NETWORK_CMD_ACK)
			{
				if (PlayerComponent* player = mPlayers[mPlayerCount]; 
					xplicit_on_join(mNetwork->get(peer_idx), player, mNetwork))
				{
					memcpy(mNetwork->get(peer_idx)->packet.buffer, XPLICIT_XASSET_ENDPOINT, strlen(XPLICIT_XASSET_ENDPOINT));

					mNetwork->get(peer_idx)->ip_address = inet_ntoa(mNetwork->get(peer_idx)->address.sin_addr);
					mNetwork->get(peer_idx)->status = NETWORK_STAT_CONNECTED;

#ifdef XPLICIT_DEBUG
					XPLICIT_INFO("[LOGIN] IP: " + mNetwork->get(peer_idx)->ip_address);
					XPLICIT_INFO("[LOGIN] PLAYER COUNT: " + std::to_string(mPlayerCount));
#endif // XPLICIT_DEBUG

					ServerReplicationManager::get_singleton_ptr()->create(COMPONENT_ID_SCRIPT, "xasset://xplicit-client.lua", mNetwork->get(peer_idx)->public_hash);
					NetworkServerContext::send(mNetwork, mNetwork->get(peer_idx));

					++mPlayerCount;

					mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_INVALID;
				}
			}
		}

	}

	void PlayerLoginEvent::handle_leave_event() noexcept
	{
		if (this->size() <= 0) 
			return;
		
		for (size_t peer_idx = 0; peer_idx < mNetwork->size(); ++peer_idx)
		{
			if (mNetwork->get(peer_idx)->status == NETWORK_STAT_DISCONNECTED)
				continue;

			if (mNetwork->get(peer_idx)->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			if (mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP ||
				mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
			{
				if (mNetwork->get(peer_idx)->hash == mNetwork->get(peer_idx)->packet.hash)
				{
					NetworkServerContext::send(mNetwork, mNetwork->get(peer_idx));

#ifdef XPLICIT_DEBUG
					XPLICIT_INFO("[LOGOFF] IP: " + mNetwork->get(peer_idx)->ip_address);
					XPLICIT_INFO("[LOGOFF] PLAYER COUNT: " + std::to_string(mPlayerCount));
#endif // XPLICIT_DEBUG

					const auto public_hash = mNetwork->get(peer_idx)->public_hash;

					mNetwork->get(peer_idx)->unique_addr.invalidate();
					mNetwork->get(peer_idx)->reset();

					/* we still want to notify players about our departure. */

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
	}

	void PlayerLoginEvent::operator()()
	{
		this->handle_leave_event();
		this->handle_join_event();
	}

	const size_t& PlayerLoginEvent::size() const noexcept { return mPlayerCount; }

	const char* PlayerLoginEvent::name() noexcept { return ("PlayerLoginEvent"); }
}