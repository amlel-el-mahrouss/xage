/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ServerConfig.h"

namespace Xplicit
{
	class ServerReplicationFactory final
	{
	public:
		explicit ServerReplicationFactory();
		~ServerReplicationFactory() = default;

	public:
		XPLICIT_COPY_DEFAULT(ServerReplicationFactory);

	public:
		void remove(const std::int32_t& id, const char* path, const std::int64_t& public_hash) const noexcept;
		void create(const std::int32_t& id, const char* path, const std::int64_t& public_hash) const noexcept;
		void update(const std::int32_t& id, const char* path, const std::int64_t& public_hash) const noexcept;

	private:
		NetworkServerComponent* mNetwork;

	};
}
