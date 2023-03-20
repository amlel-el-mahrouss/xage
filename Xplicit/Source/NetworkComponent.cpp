/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: NetworkComponent.cpp
 *			Purpose: XDP client
 *
 * =====================================================================
 */

#include "NetworkComponent.h"

namespace Xplicit 
{
	// common operations for NetworkComponent.
	static void xplicit_set_ioctl(SOCKET sock)
	{
#ifdef XPLICIT_WINDOWS
		ULONG ul = 1;
		auto err = ioctlsocket(sock, FIONBIO, &ul);

		XPLICIT_ASSERT(err == NO_ERROR);
#else
#pragma error("DEFINE ME NetworkComponent.cpp")
#endif
	}

	// NetworkComponent Constructor
	NetworkComponent::NetworkComponent()
		: Component(), m_packet(), m_addr(), m_reset(false)
	{
#ifdef XPLICIT_WINDOWS
		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (m_socket == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);

		xplicit_set_ioctl(m_socket);
#else
#pragma error("DEFINE ME NetworkComponent.cpp")
#endif

#ifndef _NDEBUG
		XPLICIT_INFO("Created NetworkComponent");
#endif

	}

	NetworkComponent::~NetworkComponent()
	{
		if (reset())
			closesocket(m_socket);

#ifndef _NDEBUG
		XPLICIT_INFO("~NetworkComponent, Epoch: " + std::to_string(xplicit_get_epoch()));
#endif

		WSACleanup();
	}

	bool NetworkComponent::reset() noexcept
	{
#ifdef XPLICIT_WINDOWS
		return shutdown(m_socket, SD_BOTH) != SOCKET_ERROR;
#else
#pragma error("DEFINE ME NetworkComponent.cpp")
#endif
	}

	bool NetworkComponent::connect(const char* ip)
	{
		if (m_socket == SOCKET_ERROR)
			return false;

#ifdef XPLICIT_WINDOWS
		memset(&m_addr, 0, sizeof(SOCKADDR_IN));

		m_addr.sin_family = AF_INET;
		inet_pton(AF_INET, ip, &m_addr.sin_addr);
		m_addr.sin_port = htons(XPLICIT_UDP_PORT);

		int result = ::connect(m_socket, reinterpret_cast<SOCKADDR*>(&m_addr), 
			sizeof(PrivateAddressData));

		if (result == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);
#else
#pragma error("DEFINE ME NetworkComponent.cpp")
#endif

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("[NetworkComponent] Connected!");
#endif

		return true;
	}

	bool NetworkComponent::send(UDPNetworkPacket& packet, const size_t sz)
	{
		packet.magic[0] = XPLICIT_NETWORK_MAG_0;
		packet.magic[1] = XPLICIT_NETWORK_MAG_1;
		packet.magic[2] = XPLICIT_NETWORK_MAG_2;
		packet.version = XPLICIT_NETWORK_VERSION;

#ifdef XPLICIT_WINDOWS
		int res = ::sendto(m_socket, reinterpret_cast<const char*>(&packet), sz, 0,
			reinterpret_cast<SOCKADDR*>(&m_addr), sizeof(m_addr));

		if (res == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);
#else
#pragma error("DEFINE ME NetworkComponent.cpp")
#endif

		return true;
	}

	void NetworkComponent::update() 
	{
		this->read(m_packet);
	}

	bool NetworkComponent::read(UDPNetworkPacket& packet, const size_t sz)
	{
		m_reset = false; // we gotta clear this one, we don't know if RST was sent.

		int length{ sizeof(struct sockaddr_in) };

#ifdef XPLICIT_WINDOWS
		int res = ::recvfrom(m_socket, reinterpret_cast<char*>(&packet), sz, 0,
			(struct sockaddr*)&m_addr, &length);
#else
#pragma error("DEFINE ME NetworkComponent.cpp")
#endif

		if (length > 0)
		{
			m_packet = packet;

			if (res == SOCKET_ERROR)
			{
				int err = WSAGetLastError();

				switch (err)
				{
				case WSAECONNRESET:
				{
					m_reset = true;
					break;
				}
				}

				return false;
			}

		}

		return packet.magic[0] == XPLICIT_NETWORK_MAG_0 && packet.magic[1] == XPLICIT_NETWORK_MAG_1 &&
				packet.magic[2] == XPLICIT_NETWORK_MAG_2 && packet.version == XPLICIT_NETWORK_VERSION;
	}

	bool NetworkComponent::is_reset() noexcept { return m_reset; }

	UDPNetworkPacket& NetworkComponent::get() noexcept { return m_packet; }
}