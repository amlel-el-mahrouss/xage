/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: NetworkServerComponent.cpp
 *			Purpose: xconnect server protocol
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
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("[INVALIDATE] UUID: " + uuids::to_string(peer->unique_addr.get()));
#endif // ifdef XPLICIT_DEBUG

		peer->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
		peer->unique_addr.invalidate();

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
		:
		mSocket(INVALID_SOCKET), 
		mAddress(ip),
		mPrivate()
	{
#ifndef _NDEBUG
		std::string message;
		message += "Class NetworkServerComponent, Epoch: ";
		message += std::to_string(xplicit_get_epoch());

		XPLICIT_INFO(message);
#endif

#ifdef XPLICIT_WINDOWS
		// create ipv4 U.D.P socket.
		mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (mSocket == SOCKET_ERROR)
			throw NetworkError(NETWORK_ERR_INTERNAL_ERROR);

		xplicit_set_ioctl(mSocket);

		memset(&mPrivate, 0, sizeof(struct sockaddr_in));

		/* internet packet */
		mPrivate.sin_family = AF_INET;

		inet_pton(AF_INET, ip, &mPrivate.sin_addr.S_un.S_addr);
		mPrivate.sin_port = htons(XPLICIT_UDP_PORT);

		auto ret_bind = bind(mSocket, reinterpret_cast<SOCKADDR*>(&mPrivate), sizeof(mPrivate));
#else
#pragma error("ServerComponent.cpp")
#endif

		if (ret_bind == SOCKET_ERROR)
			throw NetworkError(NETWORK_ERR_INTERNAL_ERROR);
		
		// Let's preallocate the clients.
		// So we don't have to allocate them.

		for (size_t i = 0; i < XPLICIT_MAX_CONNECTIONS; i++)
		{
			NetworkPeer* cl = new NetworkPeer();
			mPeers.push_back(cl);
		}

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("[SERVER] IP Address Version 4: " + mAddress);
#endif
	}

	size_t NetworkServerComponent::size() noexcept 
	{ 
		return mPeers.size(); 
	}

	NetworkPeer* NetworkServerComponent::get(const std::size_t& idx) noexcept 
	{
		return mPeers.at(idx); 
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
		if (shutdown(mSocket, SD_BOTH) == SOCKET_ERROR)
			closesocket(mSocket);
#else
#pragma error("ServerComponent.cpp")
#endif

		WSACleanup();
	}

	const char* NetworkServerComponent::dns() noexcept { return mAddress.c_str(); }

	static bool xplicit_recv_packet(
		NetworkServerComponent* server,
		const std::size_t& i,
		NetworkPacket& packet)
	{
		if (server->get(i)->packet.magic[0] == XPLICIT_NETWORK_MAG_0 &&
			server->get(i)->packet.magic[1] == XPLICIT_NETWORK_MAG_1 &&
			server->get(i)->packet.magic[2] == XPLICIT_NETWORK_MAG_2 &&
			server->get(i)->packet.version == XPLICIT_NETWORK_VERSION)
		{
			server->get(i)->packet = packet;
			return true;
		}

		return false;
	}

	void NetworkServerHelper::recv_from(
		NetworkServerComponent* server,
		NetworkPeer* peer,
		const std::size_t sz)
	{
		std::int32_t fromLen = sizeof(PrivateAddressData);
		static NetworkPacket packet{};

		std::int32_t res = ::recvfrom(server->mSocket,
			reinterpret_cast<char*>(&packet),
			sz,
			0,
			reinterpret_cast<sockaddr*>(&peer->addr),
			&fromLen);

		if (peer->packet.magic[0] == XPLICIT_NETWORK_MAG_0 &&
			peer->packet.magic[1] == XPLICIT_NETWORK_MAG_1 &&
			peer->packet.magic[2] == XPLICIT_NETWORK_MAG_2 &&
			peer->packet.version == XPLICIT_NETWORK_VERSION)
		{
			if (peer->hash == packet.hash)
				peer->packet = std::move(packet);
		}
		else
		{
			xplicit_invalidate_peer(peer);
		}

		if (res == SOCKET_ERROR)
		{
			std::int32_t reason = WSAGetLastError();

			switch (reason)
			{
			case WSAECONNABORTED:
				break;
			case WSAECONNRESET:
				break;
			}
		}
	}

	void NetworkServerHelper::recv(NetworkServerComponent* server, const std::size_t sz)
	{
		if (server)
		{
			for (std::size_t index = 0; index < server->size(); ++index)
			{
				std::int32_t fromLen = sizeof(PrivateAddressData);
				static NetworkPacket packet{};

				std::int32_t res = ::recvfrom(server->mSocket,
					reinterpret_cast<char*>(&packet),
					sz,
					0,
					reinterpret_cast<sockaddr*>(&server->get(index)->addr),
					&fromLen);

				if (!xplicit_recv_packet(server, index, packet))
				{
					xplicit_invalidate_peer(server->get(index));
				}
			}
		}
	}

	void NetworkServerHelper::send(NetworkServerComponent* server, const std::size_t sz)
	{
		if (server)
		{
			for (std::size_t i = 0; i < server->size(); i++)
			{
				server->get(i)->packet.magic[0] = XPLICIT_NETWORK_MAG_0;
				server->get(i)->packet.magic[1] = XPLICIT_NETWORK_MAG_1;
				server->get(i)->packet.magic[2] = XPLICIT_NETWORK_MAG_2;

				server->get(i)->packet.hash = server->get(i)->hash;

				server->get(i)->packet.version = XPLICIT_NETWORK_VERSION;

				::sendto(server->mSocket, reinterpret_cast<const char*>(&server->get(i)->packet),
					sz,
					0, 
					reinterpret_cast<sockaddr*>(&server->get(i)->addr),
					sizeof(PrivateAddressData));

			}
		}
	}

	void NetworkServerHelper::send_to(
		NetworkServerComponent* server, 
		NetworkPeer* peer,
		const std::size_t sz)
	{
		if (peer && server)
		{
			peer->packet.magic[0] = XPLICIT_NETWORK_MAG_0;
			peer->packet.magic[1] = XPLICIT_NETWORK_MAG_1;
			peer->packet.magic[2] = XPLICIT_NETWORK_MAG_2;

			peer->packet.version = XPLICIT_NETWORK_VERSION;

			::sendto(server->mSocket, reinterpret_cast<const char*>(&peer->packet),
				sz,
				0,
				reinterpret_cast<sockaddr*>(&peer->addr),
				sizeof(PrivateAddressData));
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