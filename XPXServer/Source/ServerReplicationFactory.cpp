/*
 * =====================================================================
 *
 *			XPXServer
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

	void ServerReplicationFactory::send(const std::int32_t& id, const char* path, const std::int64_t& public_hash) const noexcept
	{
		if (!mNetwork)
			return;

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			if (!mNetwork->get(i) ||
				mNetwork->get(i)->public_hash != public_hash)
				continue;

			mNetwork->get(i)->packet.channel = XPLICIT_CHANNEL_DATA;

			mNetwork->get(i)->packet.cmd[XPLICIT_REPL_CREATE] = NETWORK_CMD_DOWNLOAD;
			mNetwork->get(i)->packet.id = id;

			memset(mNetwork->get(i)->packet.replicas[XPLICIT_REPLICA_PLAYER], 0, XPLICIT_NETWORK_BUF_SZ);
			memcpy(mNetwork->get(i)->packet.replicas[XPLICIT_REPLICA_PLAYER], path, strlen(path));

			NetworkServerContext::send(mNetwork, mNetwork->get(i));

			break;
		}
	}
}