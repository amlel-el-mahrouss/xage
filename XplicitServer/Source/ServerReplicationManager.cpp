/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "ServerReplicationManager.h"
#include <lua/lua.hpp>

static int lua_Fire(lua_State* L)
{
	auto string = lua_tostring(L, 1);
	int32_t repl_id = lua_tointeger(L, 2);
	int32_t id = lua_tointeger(L, 3);

	if (string == nullptr ||
		*string == 0 ||
		repl_id > Xplicit::COMPONENT_ID_COUNT)
		return 0;

	Xplicit::ServerReplicationManager::get_singleton_ptr()->update(repl_id, string, id);

	return 0;
}

namespace Xplicit
{
	static void XplicitOpenReplicationLua()
	{
		lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_Fire);
		lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "Engine_Fire");
		Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.NetworkService.Fire = Engine_Fire");
	}

	ServerReplicationManager::ServerReplicationManager()
		:
		mNetwork(ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent"))
	{
		XplicitOpenReplicationLua();
	}


	ServerReplicationManager* ServerReplicationManager::get_singleton_ptr() noexcept
	{
		static ServerReplicationManager* manager = nullptr;

		if (manager == nullptr)
			manager = new ServerReplicationManager();

		return manager;
	}

	/*
	 * These methods handles replication events.
	 * It fills a buffer to tell what class/asset to load.
	 */

	void ServerReplicationManager::create(const std::int32_t& id, const char* path, const std::int64_t& public_hash) const noexcept
	{
		if (!mNetwork)
			return;

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			if (mNetwork->get(i)->public_hash != public_hash)
				continue;

			mNetwork->get(i)->packet.channel = XPLICIT_CHANNEL_DATA;

			mNetwork->get(i)->packet.cmd[XPLICIT_REPL_CREATE] = NETWORK_REPL_CMD_CREATE;
			mNetwork->get(i)->packet.id = id;

			memcpy(mNetwork->get(i)->packet.buffer, path, XPLICIT_NETWORK_BUF_SZ);

			break;
		}
	}

	void ServerReplicationManager::remove(const std::int32_t& id, const char* path, const std::int64_t& public_hash) const noexcept
	{
		if (!mNetwork)
			return;

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			if (mNetwork->get(i)->public_hash != public_hash)
				continue;

			mNetwork->get(i)->packet.channel = XPLICIT_CHANNEL_DATA;

			mNetwork->get(i)->packet.cmd[XPLICIT_REPL_DESTROY] = NETWORK_REPL_CMD_DESTROY;
			mNetwork->get(i)->packet.id = id;
			
			memcpy(mNetwork->get(i)->packet.buffer, path, XPLICIT_NETWORK_BUF_SZ);

			break;
		}
	}

	void ServerReplicationManager::update(const std::int32_t& id, const char* path, const std::int64_t& public_hash) const noexcept
	{
		if (!mNetwork)
			return;

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			if (mNetwork->get(i)->public_hash != public_hash)
				continue;

			mNetwork->get(i)->packet.channel = XPLICIT_CHANNEL_DATA;

			mNetwork->get(i)->packet.cmd[XPLICIT_REPL_FIRE] = NETWORK_REPL_CMD_FIRE;
			mNetwork->get(i)->packet.id = id;

			memcpy(mNetwork->get(i)->packet.buffer, path, XPLICIT_NETWORK_BUF_SZ);

			break;
		}
	}
}