/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "ServerReplicationManager.h"

namespace Xplicit
{
	ServerReplicationManager::ServerReplicationManager()
		:
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent"))
	{
		XPLICIT_ASSERT(mNetwork);
	}

	//! get_singleton_ptr takes the singleton of this class.
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

			NetworkServerContext::send(mNetwork, mNetwork->get(i));

			mNetwork->get(i)->packet.channel = XPLICIT_CHANNEL_DATA;

			mNetwork->get(i)->packet.cmd[XPLICIT_REPL_CREATE] = NETWORK_REPL_CMD_CREATE;
			mNetwork->get(i)->packet.id = id;

			memset(mNetwork->get(i)->packet.buffer, 0, XPLICIT_NETWORK_BUF_SZ);
			memcpy(mNetwork->get(i)->packet.buffer, path, strlen(path));

			NetworkServerContext::send(mNetwork, mNetwork->get(i));

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

			NetworkServerContext::send(mNetwork, mNetwork->get(i));

			mNetwork->get(i)->packet.channel = XPLICIT_CHANNEL_DATA;

			mNetwork->get(i)->packet.cmd[XPLICIT_REPL_DESTROY] = NETWORK_REPL_CMD_DESTROY;
			mNetwork->get(i)->packet.id = id;

			memset(mNetwork->get(i)->packet.buffer, 0, XPLICIT_NETWORK_BUF_SZ);
			memcpy(mNetwork->get(i)->packet.buffer, path, XPLICIT_NETWORK_BUF_SZ);

			NetworkServerContext::send(mNetwork, mNetwork->get(i));

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

			NetworkServerContext::send(mNetwork, mNetwork->get(i));

			mNetwork->get(i)->packet.channel = XPLICIT_CHANNEL_DATA;

			mNetwork->get(i)->packet.cmd[XPLICIT_REPL_UPDATE] = NETWORK_REPL_CMD_UPDATE;
			mNetwork->get(i)->packet.id = id;

			memset(mNetwork->get(i)->packet.buffer, 0, XPLICIT_NETWORK_BUF_SZ);
			memcpy(mNetwork->get(i)->packet.buffer, path, XPLICIT_NETWORK_BUF_SZ);

			NetworkServerContext::send(mNetwork, mNetwork->get(i));

			break;
		}
	}
}