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

#pragma once

#include "CommonInc.h"

namespace Xplicit
{
	class ServerReplicationManager final
	{
	private:
		ServerReplicationManager() = default;

	public:
		~ServerReplicationManager() = default;

		XPLICIT_COPY_DEFAULT(ServerReplicationManager);

	public:
		static ServerReplicationManager* get_singleton_ptr() noexcept
		{
			static ServerReplicationManager* manager = nullptr;
			
			if (manager == nullptr)
				manager = new ServerReplicationManager();

			return manager;
		}

	public:
		void create(const std::int32_t& id, const std::int64_t& component_hash) noexcept;
		void destroy(const std::int32_t& id, const std::int64_t& component_hash) noexcept;
		void update(const std::int32_t& id, const std::int64_t& component_hash) noexcept;

	private:
		NetworkServerComponent* mNetwork{ nullptr };

	};

	enum class COMPONENT_BASE_LIST : std::int32_t
	{
		COMPONENT_SCRIPT = 100,
		COMPONENT_MODEL,
		COMPONENT_SOUND,
		COMPONENT_TEXTURE,
		COMPONENT_INVALID = 0xFFF,
	};

	class ServerReplicationHash final
	{
	public:
		ServerReplicationHash() = default;
		~ServerReplicationHash() = default;

		std::int64_t operator()(const String& name);

	};
}

#define XPLICIT_INVALID_REPL (~0x0)