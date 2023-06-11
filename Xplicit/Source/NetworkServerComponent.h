/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: NetworkServerComponent.h
 *			Purpose: XDP Server
 *
 * =====================================================================
 */

#pragma once

#include "NetworkComponent.h"
#include "Xplicit.h"
#include "Event.h"

namespace Xplicit
{
	constexpr const int XPLICIT_MAX_CONNECTIONS = 30;

	// the main instance behind the networking.
	class XPLICIT_API NetworkServerComponent final : public Component
	{
	public:
		explicit NetworkServerComponent(const char* ip);
		virtual ~NetworkServerComponent();

		NetworkServerComponent& operator=(const NetworkServerComponent&) = default;
		NetworkServerComponent(const NetworkServerComponent&) = default;

		virtual INSTANCE_TYPE type() noexcept;
		virtual const char* name() noexcept;
		virtual void update() override;
		const char* dns() noexcept;

		virtual bool should_update() noexcept override;

	public:
		NetworkPeer* get(size_t idx) noexcept;
		size_t size() noexcept;

	private:
		std::vector<std::shared_ptr<NetworkPeer>> m_peers;
		PrivateAddressData m_server;
		std::string m_dns;
		Socket m_socket;

		friend class NetworkServerTraits;

	};

	class XPLICIT_API NetworkServerTraits final
	{
	public:
		static void send(NetworkServerComponent* server, const size_t sz = sizeof(NetworkPacket));
		static void recv(NetworkServerComponent* server, const size_t sz = sizeof(NetworkPacket));
		static void find_and_correct(NetworkServerComponent* server);

	};
}