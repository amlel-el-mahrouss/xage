/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "LocalHTTPComponent.h"

#include <NetworkComponent.h>
#include <Event.h>

namespace Xplicit::Player
{
	class XPLICIT_API LocalNetworkMonitorEvent final : public Event
	{
	public:
		LocalNetworkMonitorEvent() = delete;
		
		LocalNetworkMonitorEvent(const std::int64_t& private_hash, const std::int64_t& public_hash);
		~LocalNetworkMonitorEvent() override;

		LocalNetworkMonitorEvent& operator=(const LocalNetworkMonitorEvent&) = default;
		LocalNetworkMonitorEvent(const LocalNetworkMonitorEvent&) = default;
		
		void operator()() override;
		const char* name() noexcept override;

		std::unique_ptr<LocalHTTPComponent> HTTP;
		String Endpoint;

	private:
		NetworkComponent* mNetwork;
		std::int32_t mResetCount;
		std::int64_t mPublicHash;
		std::int64_t mHash;

	};
}