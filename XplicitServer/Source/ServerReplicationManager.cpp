/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "ServerReplicationManager.h"

namespace Xplicit
{
	ServerReplicationManager* ServerReplicationManager::get_singleton_ptr() noexcept
	{
		static ServerReplicationManager* manager = nullptr;

		if (manager == nullptr)
			manager = new ServerReplicationManager();

		return manager;
	}

	/*
	 *
	 * These methods handles replication events.
	 * They fill a buffer to tell what class/asset to load.
	 *
	 */

	void ServerReplicationManager::create(const std::int32_t& id, const char* path, std::int64_t& public_hash) const noexcept
	{
		if (!mNetwork)
			return;

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			if (mNetwork->get(i)->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			if (mNetwork->get(i)->public_hash != public_hash)
				continue;

			mNetwork->get(i)->packet.cmd[XPLICIT_REPL_CREATE] = NETWORK_REPL_CMD_CREATE;
			mNetwork->get(i)->packet.id = id;

			memcpy(mNetwork->get(i)->packet.buffer, path, 64);

			break;
		}
	}

	void ServerReplicationManager::remove(const std::int32_t& id, const char* path, std::int64_t& public_hash) const noexcept
	{
		if (!mNetwork)
			return;

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			if (mNetwork->get(i)->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			if (mNetwork->get(i)->public_hash != public_hash)
				continue;

			mNetwork->get(i)->packet.cmd[XPLICIT_REPL_CREATE] = NETWORK_REPL_CMD_DESTROY;
			mNetwork->get(i)->packet.id = id;
			
			memcpy(mNetwork->get(i)->packet.buffer, path, 64);

			break;
		}
	}

	void ServerReplicationManager::update(const std::int32_t& id, const char* path, std::int64_t& public_hash) const noexcept
	{
		if (!mNetwork)
			return;

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			if (mNetwork->get(i)->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			if (mNetwork->get(i)->public_hash != public_hash)
				continue;

			mNetwork->get(i)->packet.cmd[XPLICIT_REPL_CREATE] = NETWORK_REPL_CMD_UPDATE;
			mNetwork->get(i)->packet.id = id;

			memcpy(mNetwork->get(i)->packet.buffer, path, 64);

			break;
		}
	}
}