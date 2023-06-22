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
	/**
	 * \brief creates an hash from the GUID.
	 * \param uuid GUID (version 4 recommended)
	 * \return the hashed GUID.
	 */
	static size_t xplicit_hash_from_uuid(const uuids::uuid& uuid)
	{
		const std::string uuid_str = uuids::to_string(uuid);

		constexpr auto hash = std::hash<std::string>();
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

	PlayerJoinLeaveEvent::PlayerJoinLeaveEvent() 
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

	PlayerJoinLeaveEvent::~PlayerJoinLeaveEvent() = default;

	void PlayerJoinLeaveEvent::operator()()
	{
		this->handle_join_event();
		this->handle_leave_event();
	}

	const size_t& PlayerJoinLeaveEvent::size() const noexcept { return mPlayerCount; }

	bool PlayerJoinLeaveEvent::handle_join_event() noexcept
	{
		String addr = "";

		for (size_t peer_idx = 0; peer_idx < mNetwork->size(); ++peer_idx)
		{
			addr = inet_ntoa(mNetwork->get(peer_idx)->address.sin_addr);

			if (mNetwork->get(peer_idx)->status == NETWORK_STAT_CONNECTED)
				continue;

			if (mNetwork->get(peer_idx)->packet.size < 1)
				continue;

			/* if everything is ok, reserve new player. */
			if (mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN &&
				mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] == NETWORK_CMD_ACK)
			{
				/* the reason i didn't place that earlier, is because the full may be full yes, but we could target the wrong player. */
				if (this->size() > XPLICIT_MAX_CONNECTIONS)
				{
					mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
					return false;
				}

				/* place this after kick algorithm. */
				mNetwork->get(peer_idx)->str_address = addr;

				PlayerComponent* player = mPlayers[mPlayerCount];
				xplicit_on_join(mNetwork->get(peer_idx), player, mNetwork);

#ifdef XPLICIT_DEBUG
				XPLICIT_INFO("[CONNECT] UUID: " + uuids::to_string(mNetwork->get(peer_idx)->unique_addr.get()));
#endif // XPLICIT_DEBUG

				++mPlayerCount;
			}
		}

		return true;
	}

	//! Decreases and free player resources.
	/** Used by server to hook join and leave events. */

	bool PlayerJoinLeaveEvent::handle_leave_event() noexcept
	{
		String addr = "";

		if (this->size() == 0) return false;
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
#ifdef XPLICIT_DEBUG
					XPLICIT_INFO("[DISCONNECT] UUID: " + uuids::to_string(mNetwork->get(peer_idx)->unique_addr.get()));
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

		return true;
	}

	const char* PlayerJoinLeaveEvent::name() noexcept { return ("PlayerJoinLeaveEvent"); }
}