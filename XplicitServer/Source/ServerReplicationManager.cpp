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

	void ServerReplicationManager::create(const std::int32_t& id, const std::int64_t& component_hash) noexcept
	{
		if (!mNetwork)
			return;

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			auto& packet = mNetwork->get(i)->packet;

			packet.cmd[XPLICIT_REPL_CREATE] = NETWORK_REPL_CMD_CREATE;
			packet.id = id;
			packet.public_hash = component_hash;
		}
	}

	void ServerReplicationManager::remove(const std::int32_t& id, const std::int64_t& component_hash) noexcept
	{
		if (!mNetwork)
			return;

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			auto& packet = mNetwork->get(i)->packet;

			packet.cmd[XPLICIT_REPL_CREATE] = NETWORK_REPL_CMD_DESTROY;
			packet.id = id;
			packet.public_hash = component_hash;
		}
	}

	void ServerReplicationManager::update(const std::int32_t& id, const std::int64_t& component_hash) noexcept
	{
		if (!mNetwork)
			return;

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			auto& packet = mNetwork->get(i)->packet;

			packet.cmd[XPLICIT_REPL_CREATE] = NETWORK_REPL_CMD_UPDATE;
			packet.id = id;
			packet.public_hash = component_hash;
		}
	}
}