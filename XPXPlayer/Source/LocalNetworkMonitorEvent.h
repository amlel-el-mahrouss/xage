/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <NetworkComponent.h>
#include <XHTTPManager.h>

#include <Event.h>

namespace XPX
{
	/*
	* 
	* @brief This is a network monitoring event
	* It keeps track of hash, resets and things like that.
	* if also owns an XHTTPManager.
	*/
	class LocalNetworkMonitorEvent final : public Event
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
		std::unique_ptr<XHTTPManager> HTTP; // HTTP
		String Endpoint; // Asset delivery url
		String ID; // player's XplicitID

	private:
		NetworkComponent* mNetwork;

	private:
		std::int32_t mResetCount;
		std::int64_t mPublicHash;
		std::int64_t mHash;

	};
}