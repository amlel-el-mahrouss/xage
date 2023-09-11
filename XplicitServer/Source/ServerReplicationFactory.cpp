/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "ServerReplicationFactory.h"

namespace XPX
{
	ServerReplicationFactory::ServerReplicationFactory()
		: mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent"))
	{
		XPLICIT_ASSERT(mNetwork);
	}

	/*
	 * These methods handles replication events.
	 * It fills a buffer to tell what class/asset to load.
	 */

	void ServerReplicationFactory::send(const std::int32_t& id, const char* path, const std::int32_t& type, const std::int64_t& public_hash) const noexcept
	{
		if (!mNetwork)
			return;

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			if (!mNetwork->get(i) ||
				mNetwork->get(i)->public_hash != public_hash)
				continue;

			mNetwork->get(i)->packet.channel = XPLICIT_CHANNEL_DATA;

			if (type == NETWORK_REPL_CMD_UPDATE)
				mNetwork->get(i)->packet.cmd[XPLICIT_REPL_UPDATE] = NETWORK_REPL_CMD_UPDATE;
			else if (type == NETWORK_REPL_CMD_CREATE)
				mNetwork->get(i)->packet.cmd[XPLICIT_REPL_CREATE] = NETWORK_REPL_CMD_CREATE;
			else if (type == NETWORK_REPL_CMD_DESTROY)
				mNetwork->get(i)->packet.cmd[XPLICIT_REPL_DESTROY] = NETWORK_REPL_CMD_DESTROY;
			
			mNetwork->get(i)->packet.id = id;

			memset(mNetwork->get(i)->packet.replicas[XPLICIT_REPLICA_PLAYER], 0, XPLICIT_NETWORK_BUF_SZ);
			memcpy(mNetwork->get(i)->packet.replicas[XPLICIT_REPLICA_PLAYER], path, strlen(path));

			return;
		}
	}
}