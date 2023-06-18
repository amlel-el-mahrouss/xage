/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: ServerReplicationManager.h
 *			Purpose: Server replication management.
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
			mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			mNetwork->get(i)->packet.cmd[XPLICIT_REPL_CREATE] = NETWORK_REPL_CMD_CREATE;

			mNetwork->get(i)->packet.id = id; /* store entity id in packet. */
			mNetwork->get(i)->packet.public_hash = component_hash;
		}
	}

	void ServerReplicationManager::remove(const std::int32_t& id, const std::int64_t& component_hash) noexcept
	{
		if (!mNetwork)
			mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			mNetwork->get(i)->packet.cmd[XPLICIT_REPL_DESTROY] = NETWORK_REPL_CMD_DESTROY;

			mNetwork->get(i)->packet.id = id; /* store entity id in packet. */
			mNetwork->get(i)->packet.public_hash = component_hash;
		}
	}

	void ServerReplicationManager::update(const std::int32_t& id, const std::int64_t& component_hash) noexcept
	{
		if (!mNetwork)
			mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");

		for (size_t i = 0; i < mNetwork->size(); i++)
		{
			mNetwork->get(i)->packet.cmd[XPLICIT_REPL_UPDATE] = NETWORK_REPL_CMD_UPDATE;

			mNetwork->get(i)->packet.id = id; /* store entity id in packet. */
			mNetwork->get(i)->packet.public_hash = component_hash; /* see COMPONENT_ID_ENUM */
		}
	}

	std::int64_t ServerReplicationHash::operator()()
	{
		std::string uuid_str = uuids::to_string(UUIDFactory::version<4>());
		auto hash = std::hash<std::string>();
		auto res = hash(uuid_str);

		return res;
	}
}