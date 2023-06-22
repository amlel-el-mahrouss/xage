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

#include "PlayerJoinEvent.h"

namespace Xplicit
{
	/**
	 * \brief creates an hash from the GUID.
	 * \param uuid GUID (version 4 recommended)
	 * \return the hashed GUID.
	 */
	static size_t xplicit_hash_from_uuid(const uuids::uuid& uuid)
	{
		const String uuid_str = uuids::to_string(uuid);

		constexpr auto hash = std::hash<String>();
		const auto res = hash(uuid_str);

		return res;
	}

	/**
	 * \brief Setups and prepares the player for a join event.
	 * \param peer Network instance
	 * \param player Player component
	 * \param server Server component
	 */

	static void xplicit_on_join(NetworkInstance* peer, PlayerComponent* player, const NetworkServerComponent* server)
	{
		// just in case.
		peer->unique_addr.invalidate();

		const auto hash = xplicit_hash_from_uuid(peer->unique_addr.get());
		const auto public_hash_uuid = UUIDFactory::version<4>();

		peer->public_hash = xplicit_hash_from_uuid(public_hash_uuid);
		peer->hash = hash;

		peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
		peer->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;

		peer->packet.hash = peer->hash;
		peer->packet.size = sizeof(NetworkPacket);

		peer->status = NETWORK_STAT_CONNECTED;

		player->set(peer);

		for (std::size_t peer_idx = 0; peer_idx < server->size(); ++peer_idx)
		{
			if (server->get(peer_idx)->hash != hash)
			{
				server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
				server->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;

				server->get(peer_idx)->packet.public_hash = peer->public_hash;
			}
		}
	}

	PlayerJoinEvent::PlayerJoinEvent()
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

	PlayerJoinEvent::~PlayerJoinEvent() = default;

	void PlayerJoinEvent::handle_join_event() noexcept
	{
		if (this->size() > XPLICIT_MAX_CONNECTIONS)
			return;

		for (size_t peer_idx = 0; peer_idx < mNetwork->size(); ++peer_idx)
		{
			if (mNetwork->get(peer_idx)->status == NETWORK_STAT_CONNECTED)
				continue;

			if (mNetwork->get(peer_idx)->packet.size < 1)
				continue;

			if (mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN &&
				mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] == NETWORK_CMD_ACK)
			{
				/* OK. reserve player now. */
				PlayerComponent* player = mPlayers[mPlayerCount];
				xplicit_on_join(mNetwork->get(peer_idx), player, mNetwork);

#ifdef XPLICIT_DEBUG
				String addr = "";
				addr = inet_ntoa(mNetwork->get(peer_idx)->address.sin_addr);

				XPLICIT_INFO("[CONNECT] IP: " + addr);
#endif // XPLICIT_DEBUG

				++mPlayerCount;
			}
		}
	}

	void PlayerJoinEvent::handle_leave_event() noexcept
	{
		String addr = "";

		for (size_t peer_idx = 0; peer_idx < mNetwork->size(); ++peer_idx)
		{
			if (mNetwork->get(peer_idx)->status == NETWORK_STAT_DISCONNECTED)
				continue;

			if (mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP ||
				mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
			{
#ifdef XPLICIT_DEBUG
				String addr = "";
				addr = inet_ntoa(mNetwork->get(peer_idx)->address.sin_addr);

				if (mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
					XPLICIT_INFO("[KICK] IP: " + addr);
				else
					XPLICIT_INFO("[DISCONNECT] IP: " + addr);

#endif // XPLICIT_DEBUG

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

	void PlayerJoinEvent::operator()()
	{
		this->handle_leave_event();
		this->handle_join_event();
	}

	const size_t& PlayerJoinEvent::size() const noexcept { return mPlayerCount; }

	const char* PlayerJoinEvent::name() noexcept { return ("PlayerJoinEvent"); }
}