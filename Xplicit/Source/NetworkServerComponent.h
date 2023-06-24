/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "NetworkComponent.h"
#include "XplicitID.h"
#include "Xplicit.h"
#include "Event.h"

namespace Xplicit
{
	constexpr const std::int16_t XPLICIT_MAX_CONNECTIONS = 16;

	typedef std::vector<std::pair<Auth::XplicitID, NetworkInstance*>> NetworkVector;

	// the main instance behind the networking.
	class XPLICIT_API NetworkServerComponent final : public Component
	{
	public:
		explicit NetworkServerComponent() = default;
		
		explicit NetworkServerComponent(const char* ip);
		~NetworkServerComponent() override;
		
		XPLICIT_COPY_DEFAULT(NetworkServerComponent);
		
		COMPONENT_TYPE type() noexcept;
		const char* name() noexcept;
		void update()  override;

		bool should_update() noexcept override;
		
		NetworkInstance* get(const std::size_t& idx) const noexcept;
		const std::uint16_t port() const noexcept;
		const char* dns() const noexcept;
		size_t size() const noexcept;

	private:
		NetworkVector mPeers;
		std::uint16_t mPort;
		Socket mSocket;
		String mDns;
		
		friend class NetworkServerContext;

	};

	class XPLICIT_API NetworkServerContext final
	{
		// try slots.
		static void try_recv(NetworkServerComponent* server, NetworkInstance* peer) noexcept;
		static void try_send(NetworkServerComponent* server, NetworkInstance* peer) noexcept;

	public:
		static void send_all(NetworkServerComponent* server) noexcept;
		static void recv_all(NetworkServerComponent* server) noexcept;

	public:
		static void send(const NetworkServerComponent* server, NetworkInstance* peer) noexcept;
		static void recv(const NetworkServerComponent* server, NetworkInstance* peer, NetworkPacket& packet) noexcept;
		
	};
}