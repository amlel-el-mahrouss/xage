/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ServerConfig.h"

namespace XPX
{
	class NetworkFileTransferFactory final
	{
	public:
		explicit NetworkFileTransferFactory();
		~NetworkFileTransferFactory();

	public:
		XPLICIT_COPY_DEFAULT(NetworkFileTransferFactory);

	public:
		void send(const std::int32_t& id, 
			const char* path, 
			const std::int64_t& public_hash) const noexcept;

	private:
		NetworkServerComponent* mNetwork;

	};
}
