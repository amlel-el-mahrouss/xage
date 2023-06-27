/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "NetworkServerComponent.h"

namespace Xplicit
{
	/// <summary>
	/// Utility function which removes the peer and actor from the server.
	/// </summary>
	/// <param name="sock">The NetworkInstance being targeted.</param>
	
	static void xplicit_set_ioctl(Socket sock)
	{
		auto ul = 1UL;
		auto error = XPLICIT_IOCTL(sock, static_cast<long>(SOCKET_FLAG::NON_BLOCKING), &ul);
		
		XPLICIT_ASSERT(error != SOCKET_ERROR);
	
#ifndef XPLICIT_DEBUG
		(void)error;
#endif
	}

	NetworkServerComponent::NetworkServerComponent(const char* ip)
		:
		Component(),
		mSocket(INVALID_SOCKET), 
		mDns(ip)
	{
		XPLICIT_ASSERT(!mDns.empty());

#ifdef XPLICIT_DEBUG
		/* checks for a	valid ip-4 address. */

		Utils::InternetProtocolChecker checker;
		XPLICIT_ASSERT(checker(ip));
#endif // ifdef XPLICIT_DEBUG

		mSocket = XPLICIT_SOCKET(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (mSocket == SOCKET_ERROR)
			throw NetworkError(NETWORK_ERR_INTERNAL_ERROR);

		xplicit_set_ioctl(mSocket);

		sockaddr_in bindAddress = {};
		memset(&bindAddress, 0, sizeof(sockaddr_in));
		
		inet_pton(AF_INET, this->mDns.c_str(), &bindAddress.sin_addr.S_un.S_addr);

		bindAddress.sin_family = AF_INET;
		bindAddress.sin_port = htons(XPLICIT_NETWORK_PORT);

		mPort = ntohs(bindAddress.sin_port);

		if (::bind(mSocket, reinterpret_cast<SOCKADDR*>(&bindAddress), sizeof(bindAddress)) == SOCKET_ERROR)
			throw NetworkError(NETWORK_ERR_INTERNAL_ERROR);

		// !Let's preallocate the clients.
		// !So we don't have to allocate them.

		for (std::size_t index = 0; index < XPLICIT_MAX_CONNECTIONS; index++)
		{
			auto inst = new NetworkInstance(Auth::XplicitID(index, xplicit_get_epoch()));
			XPLICIT_ASSERT(inst);

			mPeers.push_back(inst);
		}
	}

	const std::uint16_t NetworkServerComponent::port() const noexcept { return mPort; }

	const char* NetworkServerComponent::name() noexcept { return ("NetworkServerComponent"); }

	COMPONENT_TYPE NetworkServerComponent::type() noexcept { return COMPONENT_NETWORK; }

	void NetworkServerComponent::update() {}

	bool NetworkServerComponent::should_update() noexcept { return false; }

	NetworkServerComponent::~NetworkServerComponent() {}

	const char* NetworkServerComponent::dns() const noexcept { return mDns.c_str(); }

	NetworkInstance* NetworkServerComponent::get(const std::size_t& idx) const noexcept 
	{ 
		if (mPeers.empty())
			return nullptr;

		return mPeers.at(idx); 
	}

	size_t NetworkServerComponent::size() const noexcept { return mPeers.size(); }

	static bool xplicit_register_packet(const NetworkPacket& packet, NetworkInstance* peer)
	{
		if (packet.magic[0] != XPLICIT_NETWORK_MAG_0 ||
			packet.magic[1] != XPLICIT_NETWORK_MAG_1 ||
			packet.magic[2] != XPLICIT_NETWORK_MAG_2 ||
			packet.version != XPLICIT_NETWORK_VERSION)
		{
			return false;
		}

		if (packet.magic[0] == XPLICIT_NETWORK_MAG_0 &&
			packet.magic[1] == XPLICIT_NETWORK_MAG_1 &&
			packet.magic[2] == XPLICIT_NETWORK_MAG_2 &&
			packet.version == XPLICIT_NETWORK_VERSION)
		{
			peer->packet = packet;
			return true;
		}

		return false;
	}

	void NetworkServerContext::recv(const NetworkServerComponent* server, NetworkInstance* peer) noexcept
	{
		std::int32_t from_len = sizeof(PrivateAddressData);
		NetworkPacket packet{};

		sockaddr rhs;
		const sockaddr lhs = *reinterpret_cast<sockaddr*>(&peer->address);

		if (const auto ret = ::recvfrom(server->mSocket,
			reinterpret_cast<char*>(&packet),
			sizeof(NetworkPacket),
			0,
			&rhs,
			&from_len); ret == SOCKET_ERROR)
		{
			switch (WSAGetLastError())
			{
			case WSAEWOULDBLOCK:
			{
				fd_set fd;

				FD_ZERO(&fd);
				FD_SET(server->mSocket, &fd);
				
				constexpr timeval XPLICIT_TIME = { .tv_sec = 1, .tv_usec = 0 };

				::select(0, &fd, nullptr, nullptr, &XPLICIT_TIME);

				break;
			}
			default:
				break;
			}
		}

		/* just a precaution to prevent address 0.0.0.0 from getting in. */

		if (peer->status == NETWORK_STAT_CONNECTED)
		{
			if (memcmp(lhs.sa_data, rhs.sa_data, 14) > 0)
			{
				for (std::size_t i = 0; i < server->size(); ++i)
				{
					const std::size_t peer_at = i;
					const auto peer_lhs = server->get(peer_at);
					const sockaddr _lhs = *reinterpret_cast<sockaddr*>(&peer_lhs->address);

					if (memcmp(_lhs.sa_data, rhs.sa_data, 14) == 0)
					{
						(void)xplicit_register_packet(packet, peer_lhs);
						return;
					}
				}
			}
			else
			{
				(void)xplicit_register_packet(packet, peer);
			}
		}
		else
		{
			for (std::size_t i = 0; i < server->size(); ++i)
			{
				const std::size_t peer_at = i;
				const auto peer_lhs = server->get(peer_at);

				if (peer_lhs->ip_address == XPLICIT_BAD_ADDRESS_STR)
					continue;

				const sockaddr _lhs = *reinterpret_cast<sockaddr*>(&peer_lhs->address);

				if (memcmp(_lhs.sa_data, rhs.sa_data, 14) == 0)
				{
					(void)xplicit_register_packet(packet, peer_lhs);
					return;
				}
			}

			if (xplicit_register_packet(packet, peer))
			{
				const sockaddr_in in = *reinterpret_cast<sockaddr_in*>(&rhs);
				peer->address = in;
			}
		}
	}
	
	void NetworkServerContext::recv_all(const NetworkServerComponent* server) noexcept
	{
		XPLICIT_ASSERT(server);
		
		for (std::size_t i = 0; i < server->size(); ++i)
		{
			recv(server, server->get(i));
		}
	}
	
	void NetworkServerContext::send_all(const NetworkServerComponent* server) noexcept
	{
		XPLICIT_ASSERT(server);

		for (std::size_t i = 0; i < server->size(); ++i)
		{
			send(server, server->get(i));
		}
	}
	
	void NetworkServerContext::send(const NetworkServerComponent* server, NetworkInstance* peer) noexcept
	{
		peer->packet.magic[0] = XPLICIT_NETWORK_MAG_0;
		peer->packet.magic[1] = XPLICIT_NETWORK_MAG_1;
		peer->packet.magic[2] = XPLICIT_NETWORK_MAG_2;

		peer->packet.version = XPLICIT_NETWORK_VERSION;

		if (::sendto(server->mSocket, reinterpret_cast<const char*>(&peer->packet),
			sizeof(NetworkPacket),
			0,
			reinterpret_cast<sockaddr*>(&peer->address),
			sizeof(PrivateAddressData)) == SOCKET_ERROR)
		{
			switch (WSAGetLastError())
			{
			case WSAEWOULDBLOCK:
			{
				fd_set fd;

				FD_ZERO(&fd);
				FD_SET(server->mSocket, &fd);
				
				constexpr timeval XPLICIT_TIME = { .tv_sec = 1, .tv_usec = 0 };

				::select(0, &fd, nullptr, nullptr, &XPLICIT_TIME);
				
				break;
			}
			default:
				break;
			}
		}
	}

	void NetworkServerContext::recv_from(const NetworkServerComponent* server, NetworkInstance* peer, NetworkPacket& packet) noexcept
	{
		XPLICIT_ASSERT(peer && server);

		std::int32_t from_len = sizeof(PrivateAddressData);

		::recvfrom(server->mSocket,
			reinterpret_cast<char*>(&packet),
			sizeof(NetworkPacket),
			0,
			reinterpret_cast<sockaddr*>(&peer->address),
			&from_len);

		xplicit_register_packet(packet, peer);
	}
}