/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 
	@file PlayerJoinEvent.cpp

 */

#include "LoginEvent.h"
#include <RLua.hpp>

namespace XPX
{
	/**
	 * \brief Creates an hash from the GUID.
	 * \return the hashed GUID.
	 */

	static size_t XplicitHash() noexcept
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

	static bool XplicitHandleJoin(NetworkPeer* peer, 
		CharacterComponent* player, 
		const NetworkServerComponent* server) noexcept
	{
		if (!server ||
			!peer ||
			!player ||
			player->get_peer() != nullptr)
			return false;
		
		player->set_health(0.0);

		peer->public_hash = XplicitHash();
		peer->hash = XplicitHash();

		peer->status = NETWORK_STAT_CONNECTED;
		
		peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
		peer->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;

		peer->packet.hash = peer->hash;
		peer->packet.hash = peer->public_hash;

		peer->packet.size = sizeof(NetworkPacket);

		for (std::size_t index = 0; index < server->size(); ++index)
		{
			if (server->get(index)->hash == peer->hash)
				continue;

			if (server->get(index)->status == NETWORK_STAT_CONNECTED)
			{
				server->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;
				server->get(index)->packet.public_hash = peer->public_hash;
			}
		}

		peer->xplicit_id.generate(peer->public_hash);
		player->set_peer(peer);
		
		return true;
	}

	LoginEvent::LoginEvent()
		:
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
		mPlayerCount(0)
	{
		for (std::size_t index = 0UL; index < XPLICIT_MAX_CONNECTIONS; ++index)
		{
			CharacterComponent* component = ComponentSystem::get_singleton_ptr()->add<CharacterComponent>();
			XPLICIT_ASSERT(component);

			mPlayers.push_back(component);
		}
	}

	LoginEvent::~LoginEvent() = default;

	//! @brief Handle player log-in event
	//! @brief setup humanoid and more...
	
	void LoginEvent::on_join() noexcept
	{
		if (this->size() >= XPLICIT_MAX_CONNECTIONS)
			return;

		for (size_t peer_idx = 0; peer_idx < mNetwork->size(); ++peer_idx)
		{
			if (mNetwork->get(peer_idx)->status == NETWORK_STAT_CONNECTED ||
				mNetwork->get(peer_idx)->status == NETWORK_STAT_INVALID ||
				mNetwork->get(peer_idx)->status == NETWORK_STAT_STASIS)
				continue;

			if (mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN &&
				mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] == NETWORK_CMD_ACK)
			{
				if (CharacterComponent* player = mPlayers[mPlayerCount]; 
					XplicitHandleJoin(mNetwork->get(peer_idx), player, mNetwork))
				{
					mNetwork->get(peer_idx)->ip_address = address_to_string(mNetwork->get(peer_idx));
					
					memset(mNetwork->get(peer_idx)->packet.additional_data, 0, XPLICIT_NETWORK_BUF_SZ);

					memcpy(mNetwork->get(peer_idx)->packet.additional_data,
						mNetwork->get(peer_idx)->xplicit_id.as_string().c_str(),
						mNetwork->get(peer_idx)->xplicit_id.as_string().size());

					++mPlayerCount;

					XPLICIT_INFO("[LOGIN] IP: " + mNetwork->get(peer_idx)->ip_address);
					XPLICIT_INFO("[LOGIN] XPX_ID: " + mNetwork->get(peer_idx)->xplicit_id.as_string());
					XPLICIT_INFO("[LOGIN] PLAYER COUNT: " + std::to_string(mPlayerCount));

					NetworkServerContext::send_all(mNetwork);
				}
			}
		}

	}

	void LoginEvent::on_leave() noexcept
	{
		if (this->size() < 1)
			return;

		for (size_t peer_idx = 0; peer_idx < mNetwork->size(); ++peer_idx)
		{
			if (mNetwork->get(peer_idx)->status == NETWORK_STAT_DISCONNECTED ||
				mNetwork->get(peer_idx)->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			//! If it is a kick or a stop or either a ban
			//! Do this action.
			if (mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP ||
				mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK ||
				mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_BAN] == NETWORK_CMD_BAN)
			{
				const auto hash_lhs = mNetwork->get(peer_idx)->packet.hash;
				const auto hash_rhs = mNetwork->get(peer_idx)->hash;

				if (hash_lhs == hash_rhs)
				{
					mNetwork->get(peer_idx)->status = NETWORK_STAT_DISCONNECTED;

					--mPlayerCount;

					XPLICIT_INFO("[LOGOFF] IP: " + mNetwork->get(peer_idx)->ip_address);
					XPLICIT_INFO("[LOGOFF] XPX_ID: " + mNetwork->get(peer_idx)->xplicit_id.as_string());
					XPLICIT_INFO("[LOGOFF] PLAYER COUNT: " + std::to_string(mPlayerCount));

					const auto public_hash = mNetwork->get(peer_idx)->public_hash;

					mNetwork->get(peer_idx)->reset(); // reset peer.
					mNetwork->get(peer_idx)->xplicit_id.generate(UINT32_MAX); // invalidate player id.

					for (std::size_t player = 0; player < mPlayers.size(); ++player)
					{
						if (mPlayers[player]->get_peer() == mNetwork->get(peer_idx))
						{
							mPlayers[player]->set_peer(nullptr);
							break;
						}
					}

					/* we still want to notify players about our departure. */

					for (std::size_t index = 0; index < mNetwork->size(); ++index)
					{
						if (mNetwork->get(index)->status == NETWORK_STAT_CONNECTED)
						{
							mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
							mNetwork->get(index)->packet.public_hash = public_hash;
						}
					}

					NetworkServerContext::send_all(mNetwork);
				}
			}
		}
	}

	void LoginEvent::operator()()
	{
		this->on_leave();
		this->on_join();
	}

	const size_t& LoginEvent::size() const noexcept { return mPlayerCount; }

	const char* LoginEvent::name() noexcept { return ("LoginEvent"); }
}