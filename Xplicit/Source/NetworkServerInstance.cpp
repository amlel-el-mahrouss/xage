/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: NetworkServerInstance.cpp
 *			Purpose: XPX Protocol Server
 * 
 * =====================================================================
 */

#include "NetworkServerInstance.h"

namespace Xplicit
{
	static void xplicit_set_ioctl(SOCKET sock)
	{
#ifdef XPLICIT_WINDOWS
		u_long ul = 1;
		auto err = ioctlsocket(sock, FIONBIO, &ul);

		XPLICIT_ASSERT(err == NO_ERROR);
#else
#pragma error("DEFINE ME ServerInstance.cpp")
#endif
	}

	NetworkServerInstance::NetworkServerInstance(const char* ip)
		: m_socket(INVALID_SOCKET), m_dns(ip), m_server()
	{
#ifndef _NDEBUG
		std::string message;
		message += "Class NetworkServerInstance, Epoch: ";
		message += std::to_string(xplicit_get_epoch());

		XPLICIT_INFO(message);
#endif

#ifdef XPLICIT_WINDOWS
		// create ipv4 udp socket.
		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (m_socket == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);

		xplicit_set_ioctl(m_socket);

		memset(&m_server, 0, sizeof(struct sockaddr_in));

		m_server.sin_family = AF_INET;
		inet_pton(AF_INET, ip, &m_server.sin_addr.S_un.S_addr);
		m_server.sin_port = htons(XPLICIT_NETWORK_PORT);

		auto ret_bind = bind(m_socket, reinterpret_cast<SOCKADDR*>(&m_server), sizeof(m_server));
#else
#pragma error("DEFINE ME ServerInstance.cpp")
#endif

		if (ret_bind == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);
		
		// Let's pre-allocate the clients.
		// So we don't have to allocate them.
		for (size_t i = 0; i < XPLICIT_MAX_CONNECTIONS; i++)
		{
			NetworkPeer* cl = new NetworkPeer();
			XPLICIT_ASSERT(cl);

			cl->stat = NETWORK_STAT_DISCONNECTED;
			cl->bad = false;
			cl->hash = 0;

			m_peers.push_back(std::shared_ptr<NetworkPeer>(cl));
		}

		XPLICIT_INFO("[SERVER] IP: " + m_dns);
	}

	size_t NetworkServerInstance::size() noexcept 
	{ 
		return m_peers.size(); 
	}

	NetworkPeer* NetworkServerInstance::get(size_t idx) noexcept 
	{
		XPLICIT_ASSERT(m_peers[idx]);
		return m_peers[idx].get(); 
	}

	const char* NetworkServerInstance::name() noexcept { return ("NetworkServerInstance"); }

	NetworkServerInstance::INSTANCE_TYPE NetworkServerInstance::type() noexcept { return INSTANCE_NETWORK; }

	void NetworkServerInstance::update() 
	{

	}

	NetworkServerInstance::~NetworkServerInstance()
	{
		// don't print that in release builds.
#ifndef _NDEBUG
		std::string message;
		message += "Class NetworkServerInstance::~NetworkServerInstance(), Epoch: ";
		message += std::to_string(xplicit_get_epoch());

		XPLICIT_INFO(message);
#endif

#ifdef XPLICIT_WINDOWS
		if (shutdown(m_socket, SD_BOTH) == SOCKET_ERROR)
			closesocket(m_socket);
#else
#pragma error("DEFINE ME ServerInstance.cpp")
#endif

		WSACleanup();
	}

	const char* NetworkServerInstance::dns() noexcept { return m_dns.c_str(); }

	// we need a way to tell which client is who.
	void NetworkServerTraits::recv(NetworkServerInstance* server)
	{
		if (server)
		{
			for (size_t i = 0; i < server->size(); ++i)
			{
				NetworkPacket tmp{};
				int fromLen = sizeof(struct sockaddr_in);

				int res = ::recvfrom(server->m_socket, (char*)&tmp, sizeof(NetworkPacket), 0,
					reinterpret_cast<sockaddr*>(&server->get(i)->addr), &fromLen);

				if (res == SOCKET_ERROR)
					break;

				if (tmp.magic[0] == XPLICIT_NETWORK_MAG_0 &&
					tmp.magic[1] == XPLICIT_NETWORK_MAG_1 &&
					tmp.magic[2] == XPLICIT_NETWORK_MAG_2 && server->get(i)->hash == tmp.hash)
				{
					server->get(i)->packet = tmp;
				}
			}
		}
	}

	void NetworkServerTraits::send(NetworkServerInstance* server)
	{
		if (server)
		{
			for (size_t i = 0; i < server->size(); i++)
			{
				auto peer = server->get(i);

				peer->packet.magic[0] = XPLICIT_NETWORK_MAG_0;
				peer->packet.magic[1] = XPLICIT_NETWORK_MAG_1;
				peer->packet.magic[2] = XPLICIT_NETWORK_MAG_2;

				sendto(server->m_socket, reinterpret_cast<char*>(&peer->packet), sizeof(NetworkPacket), 0, reinterpret_cast<sockaddr*>(&peer->addr), sizeof(PrivateAddressData));
			}
		}
	}
}