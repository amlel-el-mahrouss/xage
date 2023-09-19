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
		HumanoidComponent* player, 
		const NetworkServerComponent* server) noexcept
	{
		if (!server ||
			!peer ||
			!player ||
			player->get_peer() != nullptr)
			return false;
		
		peer->public_hash = XplicitHash();
		peer->hash = XplicitHash();

		peer->status = NETWORK_STAT_CONNECTED;
		
		peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
		peer->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;

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

	LoginEvent::LoginEvent()
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

	static XPXLoginWatchService* XPX_LOGIN_WATCHER = nullptr;

	class XPXLoginWatchService
	{
	public:
		std::vector<lua_CFunction> mFuncs;

	public:
		static int connect(lua_State* L)
		{
			XPXLoginWatchService* service = (XPXLoginWatchService*)lua_touserdata(L, 1);

			if (!XPX_LOGIN_WATCHER)
				service = XPX_LOGIN_WATCHER;

			lua_CFunction func = lua_tocfunction(L, 2);

			if (func)
			{
				service->mFuncs.push_back(func);

				lua_pushboolean(L, true);
				return 1;
			}

			lua_pushboolean(L, false);
			return 1;
		}

	};

	LoginEvent::~LoginEvent()
	{
		RLua::RuntimeClass<XPXLoginWatchService> watcher;
		watcher.begin_class("LoginWatchService").append_proc("Connect", &XPXLoginWatchService::connect).end_class();
	}

	//! @brief Handle player log-in event
	//! @brief setup humanoid and more...
	
	void LoginEvent::HandleJoin() noexcept
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
					XplicitHandleJoin(mNetwork->get(peer_idx), player, mNetwork))
				{
					mNetwork->get(peer_idx)->ip_address = address_to_string(mNetwork->get(peer_idx));
					mNetwork->get(peer_idx)->status = NETWORK_STAT_CONNECTED;
					
					++mPlayerCount;

					memcpy(mNetwork->get(peer_idx)->packet.additional_data, 
						mNetwork->get(peer_idx)->xplicit_id.as_string().c_str(),
						mNetwork->get(peer_idx)->xplicit_id.as_string().size());

					memset(mNetwork->get(peer_idx)->packet.additional_data, 0, XPLICIT_NETWORK_BUF_SZ);

					XPLICIT_INFO("[LOGIN] IP: " + mNetwork->get(peer_idx)->ip_address);
					XPLICIT_INFO("[LOGIN] XPLICIT_ID: " + mNetwork->get(peer_idx)->xplicit_id.as_string());
					XPLICIT_INFO("[LOGIN] PLAYER COUNT: " + std::to_string(mPlayerCount));

					NetworkServerContext::send_all(mNetwork);

					Thread job([&](const size_t idx) {
						for (size_t network_peer_index = 0; network_peer_index < mNetwork->size(); ++network_peer_index)
						{
							if (mNetwork->get(network_peer_index) &&
								mNetwork->get(network_peer_index) != mNetwork->get(idx) &&
								mNetwork->get(network_peer_index)->status == NETWORK_STAT_CONNECTED)
							{
								mNetwork->get(idx)->packet.channel = XPLICIT_CHANNEL_DATA;

								mNetwork->get(idx)->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;
								memcpy(mNetwork->get(idx)->packet.additional_data, mNetwork->get(idx)->xplicit_id.as_string().c_str(), mNetwork->get(idx)->xplicit_id.as_string().size());
								mNetwork->get(idx)->packet.public_hash = mNetwork->get(network_peer_index)->public_hash;

								NetworkServerContext::send(mNetwork, mNetwork->get(idx));
							}
						}

						auto klasses = ComponentSystem::get_singleton_ptr()->all_of<ClassComponent>();

						for (auto* klass : klasses)
						{
							if (!klass)
								continue;

							if (auto kind = klass->index_as_string("ClassType");
								!kind.empty())
							{
								NetworkPacketRepl repl_packet{};

								// Standard RoXML type typechecker.
								if (kind == "Mesh")
									repl_packet.node_kind = XPX_MESH_ID;
								else if (kind == "Part")
									repl_packet.node_kind = XPX_PART_ID;
								else
									continue;

								repl_packet.channel = XPLICIT_CHANNEL_PHYSICS;
								repl_packet.version = XPLICIT_NETWORK_VERSION;

								repl_packet.magic[0] = XPLICIT_NETWORK_MAG_0;
								repl_packet.magic[1] = XPLICIT_NETWORK_MAG_1;
								repl_packet.magic[2] = XPLICIT_NETWORK_MAG_2;

								auto xasset = klass->index_as_string("Url");
								auto parent = klass->index_as_string("Parent");
								auto name = klass->index_as_string("ClassName");

								if (name.empty())
									continue;

								if (repl_packet.node_kind == XPX_MESH_ID 
									&& xasset.empty())
									continue;

								if (parent.empty())
									parent = "world";

								memcpy(repl_packet.node_parent, parent.c_str(), parent.size());
								memcpy(repl_packet.node_name, name.c_str(), parent.size());
								memcpy(repl_packet.node_path, xasset.c_str(), parent.size());

								NetworkServerContext::send_all(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent"),
									(NetworkPacket*)&repl_packet);
							}
						}

						for (auto* fn : XPX_LOGIN_WATCHER->mFuncs)
						{
							if (fn)
							{
								lua_pushcfunction(Lua::CLuaStateManager::get_singleton_ptr()->state(), fn);
								lua_pcall(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1, LUA_MULTRET, 0);
								lua_pop(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1);
							}
						}
					}, peer_idx);

					try
					{
						if (job.joinable())
							job.join();
						else
							job.detach();
					}
					catch (...)
					{
						XPLICIT_CRITICAL("Fatal error in thread execution, aborting thread.");
					}
				}
			}
		}

	}

	void LoginEvent::HandleLeave() noexcept
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

			//! If it is a kick or a stop or either a ban
			//! Do this action.
			if (mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP ||
				mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_BAN] == NETWORK_CMD_BAN)
			{
				const auto hash_lhs = mNetwork->get(peer_idx)->packet.hash;
				const auto hash_rhs = mNetwork->get(peer_idx)->hash;

				if (hash_lhs == hash_rhs)
				{
					--mPlayerCount;

					XPLICIT_INFO("[LOGOFF] IP: " + mNetwork->get(peer_idx)->ip_address);
					XPLICIT_INFO("[LOGOFF] XPLICIT_ID: " + mNetwork->get(peer_idx)->xplicit_id.as_string());
					XPLICIT_INFO("[LOGOFF] PLAYER COUNT: " + std::to_string(mPlayerCount));

					String path("_G.world.Players.");
					path += mNetwork->get(peer_idx)->xplicit_id.as_string();

					String fmt = fmt::format("world:Logoff({})", path);
					Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

					const auto public_hash = mNetwork->get(peer_idx)->public_hash;

					mNetwork->get(peer_idx)->reset(); // reset peer.
					mNetwork->get(peer_idx)->xplicit_id.generate(~0); // invalidate player id.

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
		this->HandleLeave();
		this->HandleJoin();
	}

	const size_t& LoginEvent::size() const noexcept { return mPlayerCount; }

	const char* LoginEvent::name() noexcept { return ("LoginEvent"); }
}