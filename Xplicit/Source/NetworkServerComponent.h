/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
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
	// @brief Maximum players in single party.
	constexpr std::int16_t XPLICIT_MAX_CONNECTIONS = 24;

	typedef std::vector<NetworkInstance*> NetworkVector;

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
		void update()  override;

	public:
		bool should_update() noexcept override;
		
	public:
		NetworkInstance* get(const std::size_t& idx) const noexcept;
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
		
		static void recv_from(const NetworkServerComponent* server, NetworkInstance* peer, NetworkPacket& packet) noexcept;
		static bool recv(const NetworkServerComponent* server, NetworkInstance* peer) noexcept;
		static void send(const NetworkServerComponent* server, NetworkInstance* peer) noexcept;
		
	};
}