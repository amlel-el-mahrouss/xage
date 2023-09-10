/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "NetworkComponent.h"
#include "XplicitID.h"
#include "Xplicit.h"
#include "Event.h"

namespace XPX
{
	// @brief Maximum players in single party.
	constexpr std::int16_t XPLICIT_MAX_CONNECTIONS = 24;

	typedef std::vector<NetworkPeer*> NetworkVector;

	/*
	* @brief Server-sided Network component
	* @brief the one who takes care of peers and socket.
	*/

	class XPLICIT_API NetworkServerComponent final : public Component
	{
	public:
		NetworkServerComponent() = delete;
		
	public:
		explicit NetworkServerComponent(const char* ip, const char* port);
		~NetworkServerComponent() override;
		
	public:
		XPLICIT_COPY_DEFAULT(NetworkServerComponent);
		
	public:
		COMPONENT_TYPE type() noexcept;
		const char* name() noexcept;

	public:
		static bool should_update() noexcept { return false; }
		static void update(void* class_ptr) noexcept { }
		
	public:
		NetworkPeer* get(const std::size_t& idx) const noexcept;
		const std::uint16_t port() const noexcept;
		const char* dns() const noexcept;
		size_t size() const noexcept;

	private:
		NetworkVector mPeers;
		std::uint16_t mPort;
		Socket mSocket;
		String mDns;

	private:
		friend class NetworkServerContext;

	};

	class XPLICIT_API NetworkServerContext final
	{
		// try slots.
	public:
		static void send_all(const NetworkServerComponent* server) noexcept;
		static void recv_all(const NetworkServerComponent* server) noexcept;
		
		static void recv_from(const NetworkServerComponent* server, NetworkPeer* peer, NetworkPacket& packet) noexcept;
		static bool recv(const NetworkServerComponent* server, NetworkPeer* peer) noexcept;
		static void send(const NetworkServerComponent* server, NetworkPeer* peer) noexcept;
		
	};
}