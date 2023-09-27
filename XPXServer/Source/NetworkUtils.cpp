/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "NetworkUtils.h"

namespace XPX
{
	NetworkFileTransferFactory::NetworkFileTransferFactory()
		:
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent"))
	{
		XPLICIT_ASSERT(mNetwork);
	}

	NetworkFileTransferFactory::~NetworkFileTransferFactory() = default;

	void NetworkFileTransferFactory::send(const std::int32_t& id, const char* path, const std::int64_t& public_hash) const noexcept
	{
		if (!mNetwork)
			return;

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			if (!mNetwork->get(i) ||
				mNetwork->get(i)->public_hash != public_hash)
				continue;

			mNetwork->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_DOWNLOAD] = NETWORK_CMD_DOWNLOAD;
			mNetwork->get(i)->packet.id = id;

			memset(mNetwork->get(i)->packet.replicas[XPLICIT_REPLICA_1], 0, XPLICIT_NETWORK_BUF_SZ);
			memcpy(mNetwork->get(i)->packet.replicas[XPLICIT_REPLICA_1], path, strlen(path));

			NetworkServerContext::send(mNetwork, mNetwork->get(i));

			break;
		}
	}
}