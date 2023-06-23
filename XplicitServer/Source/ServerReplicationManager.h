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

	public:
		static ServerReplicationManager* get_singleton_ptr() noexcept;
		
		void begin() noexcept
		{
			if (mNetwork)
				return;

			NetworkServerContext::accept_recv(mNetwork);
		}

		void remove(const std::int32_t& id, const char* path) const noexcept;
		void create(const std::int32_t& id, const char* path) const noexcept;
		void update(const std::int32_t& id, const char* path) const noexcept;

		void end() noexcept
		{
			if (mNetwork)
				return;

			NetworkServerContext::accept_send(mNetwork);
		}

	private:
		NetworkServerComponent* mNetwork{ nullptr };

	};
}
