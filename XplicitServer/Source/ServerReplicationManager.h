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
	class ServerReplicationManager final
	{
		ServerReplicationManager() = default;

	public:
		~ServerReplicationManager() = default;
		
		XPLICIT_COPY_DEFAULT(ServerReplicationManager);
		
		static ServerReplicationManager* get_singleton_ptr() noexcept;
		
		void remove(const std::int32_t& id, const std::int64_t& component_hash) noexcept;
		void create(const std::int32_t& id, const std::int64_t& component_hash) noexcept;
		void update(const std::int32_t& id, const std::int64_t& component_hash) noexcept;
		
	private:
		NetworkServerComponent* mNetwork{ nullptr };

	};
}
