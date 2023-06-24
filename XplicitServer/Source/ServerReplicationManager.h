/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "CommonInclude.h"

namespace Xplicit
{
	/*
	 *
	 *	Replication Manager for CLua, C#
	 *
	 */

	class ServerReplicationManager final
	{
		ServerReplicationManager() = default;

	public:
		~ServerReplicationManager() = default;
		
		XPLICIT_COPY_DEFAULT(ServerReplicationManager);
		
		static ServerReplicationManager* get_singleton_ptr() noexcept;
		
		void remove(const std::int32_t& id, const char* path, const std::int64_t& public_hash) noexcept;
		void create(const std::int32_t& id, const char* path, const std::int64_t& public_hash) noexcept;
		void update(const std::int32_t& id, const char* path, const std::int64_t& public_hash) noexcept;

	private:
		NetworkServerComponent* mNetwork{ nullptr };

	};
}
