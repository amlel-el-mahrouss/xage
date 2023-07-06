/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

 /**
 
	@file PlayerJoinEvent.cpp

 */

#include "PlayerLoginEvent.h"
#include <lua/CLua.hpp>

namespace Xplicit
{
	/**
	 * \brief Creates an hash from the GUID.
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

	static bool xplicit_on_join(NetworkInstance* peer, HumanoidComponent* player, const NetworkServerComponent* server)
	{
		if (!server ||
			!peer ||
			!player ||
			player->get_peer() != nullptr)
			return false;
		
		peer->public_hash = xplicit_hash();
		peer->hash = xplicit_hash();

		peer->status = NETWORK_STAT_CONNECTED;
		
		peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
		peer->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;

		peer->packet.hash = peer->hash;
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

	PlayerLoginEvent::PlayerLoginEvent()
		:
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
		mPlayerCount(0)
	{
		for (std::size_t index = 0UL; index < XPLICIT_MAX_CONNECTIONS; ++index)
		{
			HumanoidComponent* component = ComponentSystem::get_singleton_ptr()->add<HumanoidComponent>();
			XPLICIT_ASSERT(component);

			mPlayers.push_back(component);
		}
	}

	PlayerLoginEvent::~PlayerLoginEvent() = default;

	void PlayerLoginEvent::handle_join_event() noexcept
	{
		if (this->size() >= XPLICIT_MAX_CONNECTIONS)
			return;

		for (size_t peer_idx = 0; peer_idx < mNetwork->size(); ++peer_idx)
		{
			if (mNetwork->get(peer_idx)->status == NETWORK_STAT_CONNECTED ||
				mNetwork->get(peer_idx)->status == NETWORK_STAT_INVALID ||
				mNetwork->get(peer_idx)->status == NETWORK_STAT_STASIS)
				continue;

			if (mNetwork->get(peer_idx)->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			if (mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN &&
				mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] == NETWORK_CMD_ACK)
			{
				if (HumanoidComponent* player = mPlayers[mPlayerCount]; 
					xplicit_on_join(mNetwork->get(peer_idx), player, mNetwork))
				{
					// Send XplicitID to the matching client.
					memcpy(mNetwork->get(peer_idx)->packet.buffer, 
						mNetwork->get(peer_idx)->xplicit_id.as_string().c_str(),
						mNetwork->get(peer_idx)->xplicit_id.as_string().size());

					mNetwork->get(peer_idx)->ip_address = address_to_string(mNetwork->get(peer_idx));
					mNetwork->get(peer_idx)->status = NETWORK_STAT_CONNECTED;
					
					++mPlayerCount;

					NetworkServerContext::send(mNetwork, mNetwork->get(peer_idx));

#ifdef XPLICIT_DEBUG
					XPLICIT_INFO("[LOGIN] IP: " + mNetwork->get(peer_idx)->ip_address);
					XPLICIT_INFO("[LOGIN] XPLICIT_ID: " + mNetwork->get(peer_idx)->xplicit_id.as_string());
					XPLICIT_INFO("[LOGIN] PLAYER COUNT: " + std::to_string(mPlayerCount));
#endif

					XPLICIT_INFO("Game:Join [EVENT]");
					Lua::CLuaStateManager::get_singleton_ptr()->run_string("Game:Join()");

					//! Create the Lua Player table.
					//! Make these constexpr.
					constexpr const char* table = "_G.Game.Players.{}{}";
					constexpr const char* eq_table = " = {}";

					String fmt = std::format(table, player->get_peer()->xplicit_id.as_string(), eq_table);

					Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

				}
			}
		}

	}

	void PlayerLoginEvent::handle_leave_event() noexcept
	{
		if (this->size() < 1)
			return;

		for (size_t peer_idx = 0; peer_idx < mNetwork->size(); ++peer_idx)
		{
			if (mNetwork->get(peer_idx)->status == NETWORK_STAT_DISCONNECTED ||
				mNetwork->get(peer_idx)->status == NETWORK_STAT_INVALID)
				continue;

			if (mNetwork->get(peer_idx)->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			if (mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP ||
				mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
			{
				const auto hash_lhs = mNetwork->get(peer_idx)->packet.hash;
				const auto hash_rhs = mNetwork->get(peer_idx)->hash;

				if (hash_lhs == hash_rhs)
				{
					--mPlayerCount;

					mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_INVALID;
					mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_INVALID;

#ifdef XPLICIT_DEBUG

					XPLICIT_INFO("[LOGOFF] IP: " + mNetwork->get(peer_idx)->ip_address);
					XPLICIT_INFO("[LOGOFF] XPLICIT_ID: " + mNetwork->get(peer_idx)->xplicit_id.as_string());
					XPLICIT_INFO("[LOGOFF] PLAYER COUNT: " + std::to_string(mPlayerCount));

#endif

					const auto public_hash = mNetwork->get(peer_idx)->public_hash;

					// Create Player table.
					String fmt = "_G.Game.Players.";
					fmt += mNetwork->get(peer_idx)->xplicit_id.as_string();
					fmt += " = nil";

					Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

					mNetwork->get(peer_idx)->reset(); // reset peer.
					mNetwork->get(peer_idx)->xplicit_id.generate(~0); // invalidate player id.

					for (std::size_t player = 0; player < mPlayers.size(); ++player)
					{
						if (mPlayers[player]->get_peer() == mNetwork->get(peer_idx))
						{
							mPlayers[player]->set_peer(nullptr);
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

					XPLICIT_INFO("Game:Leave [EVENT]");
					Lua::CLuaStateManager::get_singleton_ptr()->run_string("Game:Leave()");
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