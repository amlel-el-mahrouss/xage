/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: PlayerSpawnDeathEvent.h
 *			Purpose: Spawn and Death handling
 *
 * =====================================================================
 */

#pragma once

#include "CommonInc.h"

namespace Xplicit
{
	class ReplicationManager final
	{
	private:
		ReplicationManager() = default;

	public:
		~ReplicationManager() = default;

		XPLICIT_COPY_DEFAULT(ReplicationManager);

	public:
		static ReplicationManager* get_singleton_ptr() noexcept
		{
			static ReplicationManager* manager = nullptr;
			
			if (manager == nullptr)
				manager = new ReplicationManager();

			return manager;
		}

	public:
		void update(const std::int32_t& id, const std::int64_t& component_hash) noexcept
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

		void update(const std::int32_t& id, const std::int64_t& component_hash) noexcept
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

		void update(const std::int32_t& id, const std::int64_t& component_hash) noexcept
		{
			if (!mNetwork)
				mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");

			for (size_t i = 0; i < mNetwork->size(); i++)
			{
				mNetwork->get(i)->packet.cmd[XPLICIT_REPL_UPDATE] = NETWORK_REPL_CMD_UPDATE;

				mNetwork->get(i)->packet.id = id; /* store entity id in packet. */
				mNetwork->get(i)->packet.public_hash = component_hash;
			}
		}

	private:
		NetworkServerComponent* mNetwork{ nullptr };

	};

	enum class ENTITY_ID : std::int32_t
	{
		COMPONENT_SCRIPT = 100,
		COMPONENT_MODEL,
		COMPONENT_SOUND,
		COMPONENT_TEXTURE,
		COMPONENT_INVALID = 0xFFF,
	};
}