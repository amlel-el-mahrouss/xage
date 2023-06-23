/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <NetworkComponent.h>
#include <Event.h>

namespace Xplicit::Player
{
	class XPLICIT_API LocalNetworkMonitorEvent final : public Event
	{
	public:
		LocalNetworkMonitorEvent() = delete;
		
		LocalNetworkMonitorEvent(int64_t hash);
		~LocalNetworkMonitorEvent() override;

		LocalNetworkMonitorEvent& operator=(const LocalNetworkMonitorEvent&) = default;
		LocalNetworkMonitorEvent(const LocalNetworkMonitorEvent&) = default;
		
		void operator()() override;
		const char* name() noexcept;

	private:
		NetworkComponent* mNetwork;
		
		std::int32_t mResetCount;
		std::int64_t mHash;

	};
}