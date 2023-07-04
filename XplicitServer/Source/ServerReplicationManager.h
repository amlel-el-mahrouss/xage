/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
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
		explicit ServerReplicationManager();

	public:
		~ServerReplicationManager() = default;
		
	public:
		XPLICIT_COPY_DEFAULT(ServerReplicationManager);
		
	public:
		static ServerReplicationManager* get_singleton_ptr() noexcept;
		
	public:
		void remove(const std::int32_t& id, const char* path, const std::int64_t& public_hash) const noexcept;
		void create(const std::int32_t& id, const char* path, const std::int64_t& public_hash) const noexcept;
		void update(const std::int32_t& id, const char* path, const std::int64_t& public_hash) const noexcept;

	private:
		NetworkServerComponent* mNetwork;

	};
}
