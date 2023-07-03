/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "LocalHTTPManager.h"

#include <NetworkComponent.h>
#include <Event.h>

namespace Xplicit::Player
{
	/*
	* 
	* @brief This is a network monitoring event
	* It keeps track of hash, resets and things like that.
	* if also owns the HTTPManager.
	*/
	class XPLICIT_API LocalNetworkMonitorEvent final : public Event
	{
	public:
		LocalNetworkMonitorEvent() = delete;
		
	public:
		LocalNetworkMonitorEvent(const std::int64_t& private_hash, const std::int64_t& public_hash);
		~LocalNetworkMonitorEvent() override;

	public:
		LocalNetworkMonitorEvent& operator=(const LocalNetworkMonitorEvent&) = default;
		LocalNetworkMonitorEvent(const LocalNetworkMonitorEvent&) = default;
		
	public:
		void operator()() override;
		const char* name() noexcept override;

	public:
		std::unique_ptr<LocalHTTPManager> HTTP; // HTTPService
		String Endpoint; // Asset Delivery service
		String ID; // XplicitID

	private:
		NetworkComponent* mNetwork;

	private:
		std::int32_t mResetCount;
		std::int64_t mPublicHash;
		std::int64_t mHash;

	};
}