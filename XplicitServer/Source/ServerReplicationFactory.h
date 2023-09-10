/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ServerConfig.h"

namespace XPX
{
	class ServerReplicationFactory final
	{
	public:
		explicit ServerReplicationFactory();
		~ServerReplicationFactory() = default;

	public:
		XPLICIT_COPY_DEFAULT(ServerReplicationFactory);

	public:
		void send(const std::int32_t& id, const char* path, const std::int32_t& type, const std::int64_t& public_hash) const noexcept;

	private:
		NetworkServerComponent* mNetwork;

	};
}
