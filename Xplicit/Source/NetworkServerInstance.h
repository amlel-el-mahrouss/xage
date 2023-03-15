/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: NetworkServerInstance.h
 *			Purpose: XPX Protocol Server
 *
 * =====================================================================
 */

#pragma once

#include "NetworkInstance.h"
#include "Foundation.h"
#include "Event.h"

namespace Xplicit
{
	constexpr const int XPLICIT_MAX_CONNECTIONS = 30;

	// the main instance behind the networking.
	class XPLICIT_API NetworkServerInstance final : public Instance
	{
	public:
		explicit NetworkServerInstance(const char* ip);
		virtual ~NetworkServerInstance();

		NetworkServerInstance& operator=(const NetworkServerInstance&) = default;
		NetworkServerInstance(const NetworkServerInstance&) = default;

		virtual INSTANCE_TYPE type() noexcept;
		virtual const char* name() noexcept;
		virtual void update() override;
		const char* dns() noexcept;

	public:
		NetworkPeer* get(size_t idx) noexcept;
		size_t size() noexcept;

	private:
		std::vector<std::shared_ptr<NetworkPeer>> m_peers;

	private:
		PrivateAddressData m_server;
		std::string m_dns;
		Socket m_socket;

		friend class NetworkServerTraits;

	};

	class XPLICIT_API NetworkServerTraits
	{
	public:
		static void send(NetworkServerInstance* instance);
		static void recv(NetworkServerInstance* instance);

	};
}