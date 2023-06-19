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

#include "CommonInclude.h"
#include <CommonEngine.h>

namespace Xplicit
{
	class ServerReplicationManager final
	{
	private:
		ServerReplicationManager() = default;

	public:
		~ServerReplicationManager() = default;

	public:
		XPLICIT_COPY_DEFAULT(ServerReplicationManager);

	public:
		static ServerReplicationManager* get_singleton_ptr() noexcept;

	public:
		void remove(const std::int32_t& id, const std::int64_t& component_hash) noexcept;
		void create(const std::int32_t& id, const std::int64_t& component_hash) noexcept;
		void update(const std::int32_t& id, const std::int64_t& component_hash) noexcept;

	private:
		NetworkServerComponent* mNetwork{ nullptr };

	};

	class ServerReplicationHash final
	{
	public:
		ServerReplicationHash() = default;
		~ServerReplicationHash() = default;

	public:
		XPLICIT_COPY_DEFAULT(ServerReplicationHash);

	public:
		std::int64_t operator()() noexcept;

	};
}

#define XPLICIT_INVALID_REPL (~0x0)