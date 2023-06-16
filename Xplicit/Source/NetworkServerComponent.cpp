/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: NetworkServerComponent.cpp
 *			Purpose: XDP Server
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
	/// <param name="peer">The NetworkPeer being targeted.</param>
	static void xplicit_invalidate_peer(NetworkPeer* peer)
	{
		peer->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
		peer->stat = NETWORK_STAT_DISCONNECTED;
	}

	static void xplicit_set_ioctl(SOCKET sock)
	{
#ifdef XPLICIT_WINDOWS
		auto ul = 1UL;

		auto err = ioctlsocket(sock, FIONBIO, &ul);
		XPLICIT_ASSERT(err == NO_ERROR);
#else
#pragma error("ServerComponent.cpp")
#endif
	}

	NetworkServerComponent::NetworkServerComponent(const char* ip)
		: m_socket(INVALID_SOCKET), m_dns(ip), m_server()
	{
#ifndef _NDEBUG
		std::string message;
		message += "Class NetworkServerComponent, Epoch: ";
		message += std::to_string(xplicit_get_epoch());

		XPLICIT_INFO(message);
#endif

#ifdef XPLICIT_WINDOWS
		// create ipv4 U.D.P socket.
		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (m_socket == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);

		xplicit_set_ioctl(m_socket);

		memset(&m_server, 0, sizeof(struct sockaddr_in));

		m_server.sin_family = AF_INET;
		inet_pton(AF_INET, ip, &m_server.sin_addr.S_un.S_addr);
		m_server.sin_port = htons(XPLICIT_UDP_PORT);

		auto ret_bind = bind(m_socket, reinterpret_cast<SOCKADDR*>(&m_server), sizeof(m_server));
#else
#pragma error("ServerComponent.cpp")
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

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("[SERVER] IP: " + m_dns);
#endif
	}

	size_t NetworkServerComponent::size() noexcept 
	{ 
		return m_peers.size(); 
	}

	NetworkPeer* NetworkServerComponent::get(size_t idx) noexcept 
	{
		XPLICIT_ASSERT(m_peers[idx]);
		return m_peers[idx].get(); 
	}

	const char* NetworkServerComponent::name() noexcept { return ("NetworkServerComponent"); }

	NetworkServerComponent::INSTANCE_TYPE NetworkServerComponent::type() noexcept { return INSTANCE_NETWORK; }

	void NetworkServerComponent::update() {}

	bool NetworkServerComponent::should_update() noexcept { return false; }

	NetworkServerComponent::~NetworkServerComponent()
	{
		// don't print that in release builds.
#ifndef _NDEBUG
		std::string message;
		message += "Class NetworkServerComponent::~NetworkServerComponent(), Epoch: ";
		message += std::to_string(xplicit_get_epoch());

		XPLICIT_INFO(message);
#endif

#ifdef XPLICIT_WINDOWS
		if (shutdown(m_socket, SD_BOTH) == SOCKET_ERROR)
			closesocket(m_socket);
#else
#pragma error("ServerComponent.cpp")
#endif

		WSACleanup();
	}

	const char* NetworkServerComponent::dns() noexcept { return m_dns.c_str(); }

	static bool xplicit_recv_packet(NetworkServerComponent* server,
		const std::size_t& i,
		NetworkPacket& packet)
	{
		if (server->get(i)->packet.magic[0] == XPLICIT_NETWORK_MAG_0 &&
			server->get(i)->packet.magic[1] == XPLICIT_NETWORK_MAG_1 &&
			server->get(i)->packet.magic[2] == XPLICIT_NETWORK_MAG_2 &&
			server->get(i)->packet.version == XPLICIT_NETWORK_VERSION)
		{
			server->get(i)->packet = std::move(packet);

			return true;
		}

		return false;
	}

	void NetworkServerHelper::recv(NetworkServerComponent* server, const size_t sz)
	{
		if (server)
		{
			for (std::size_t i = 0; i < server->size(); ++i)
			{
				std::int32_t fromLen = sizeof(PrivateAddressData);
				static NetworkPacket packet{};

				struct sockaddr addrLhs { 0 };

				std::int32_t res = ::recvfrom(server->m_socket, 
					reinterpret_cast<char*>(&packet),
					sz, 
					0,
					reinterpret_cast<sockaddr*>(&server->get(i)->addr),
					&fromLen);

				if (res == SOCKET_ERROR) continue;

				if (!xplicit_recv_packet(server, i, packet))
					xplicit_invalidate_peer(server->get(i));
			}
		}
	}

	void NetworkServerHelper::send(NetworkServerComponent* server, const size_t sz)
	{
		if (server)
		{
			for (size_t i = 0; i < server->size(); i++)
			{
				auto peer = server->get(i);

				peer->packet.magic[0] = XPLICIT_NETWORK_MAG_0;
				peer->packet.magic[1] = XPLICIT_NETWORK_MAG_1;
				peer->packet.magic[2] = XPLICIT_NETWORK_MAG_2;

				peer->packet.version = XPLICIT_NETWORK_VERSION;

				::sendto(server->m_socket, reinterpret_cast<const char*>(&peer->packet),
					sz, 
					0, 
					reinterpret_cast<sockaddr*>(&peer->addr), 
					sizeof(PrivateAddressData));
			}
		}
	}

	void NetworkServerHelper::correct(NetworkServerComponent* server)
	{
		if (server)
		{
			for (size_t peer_idx = 0; peer_idx < server->size(); ++peer_idx)
			{
				for (size_t second_peer_idx = 0; second_peer_idx < server->size(); ++second_peer_idx)
				{
					if (server->get(second_peer_idx) == server->get(peer_idx))
						continue;

					if (equals(server->get(second_peer_idx)->addr, server->get(peer_idx)->addr))
						server->get(second_peer_idx)->reset();
				}
			}
		}
	}
}