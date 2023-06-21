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
#include "CommonEngine.h"

namespace Xplicit
{
	/// <summary>
	/// Utility function which removes the peer and actor from the server.
	/// </summary>
	/// <param name="peer">The NetworkInstance being targeted.</param>
	
	static void xplicit_invalidate_peer(NetworkInstance* peer)
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("[INVALIDATE] UUID: " + uuids::to_string(peer->unique_addr.get()));
#endif // ifdef XPLICIT_DEBUG

		peer->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
		peer->unique_addr.invalidate();

		peer->status = NETWORK_STAT_DISCONNECTED;
	}

	static void xplicit_set_ioctl(Socket sock)
	{
#ifdef XPLICIT_WINDOWS
		auto ul = 1UL;

		auto err = ioctlsocket(sock, FIONBIO, &ul);
		XPLICIT_ASSERT(err == NO_ERROR);

		err = ioctlsocket(sock, FIOASYNC, &ul);
		XPLICIT_ASSERT(err == NO_ERROR);
#else
#pragma error("ServerComponent.cpp")
#endif
	}

	NetworkServerComponent::NetworkServerComponent(const char* ip)
		:
		mSocket(INVALID_SOCKET), 
		mDns(ip)
	{
		XPLICIT_ASSERT(!mDns.empty());

		Xplicit::Utils::InternetProtocolChecker checker;

		XPLICIT_ASSERT(checker(ip));

		mSocket = XPLICIT_SOCKET(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		xplicit_set_ioctl(mSocket);

		struct sockaddr_in bindAddress;

		memset(&bindAddress, 0, sizeof(struct sockaddr_in));
		bindAddress.sin_port = AF_INET;

		inet_pton(AF_INET, mDns.c_str(), &bindAddress.sin_addr.S_un.S_addr);
		bindAddress.sin_port = htons(XPLICIT_NETWORK_PORT);

		auto bindedValue = bind(mSocket, reinterpret_cast<SOCKADDR*>(&bindAddress), sizeof(bindAddress));

		if (bindedValue == -1)
			throw NetworkError(NETWORK_ERR_INTERNAL_ERROR);

		// !Let's preallocate the clients.
		// !So we don't have to allocate them.

		for (size_t i = 0; i < XPLICIT_MAX_CONNECTIONS; i++)
		{
			NetworkInstance* inst = new NetworkInstance();
			XPLICIT_ASSERT(inst);

			mPeers.push_back(std::make_pair(Auth::XplicitID(XPLICIT_UNIVERSE_ID, xplicit_get_epoch()), inst));
		}
	}

	const char* NetworkServerComponent::name() noexcept { return ("NetworkServerComponent"); }

	COMPONENT_TYPE NetworkServerComponent::type() noexcept { return COMPONENT_NETWORK; }

	void NetworkServerComponent::update() {}

	bool NetworkServerComponent::should_update() noexcept { return false; }

	NetworkServerComponent::~NetworkServerComponent() {}

	const char* NetworkServerComponent::dns() noexcept { return mDns.c_str(); }

	NetworkInstance* NetworkServerComponent::get(const std::size_t& idx) noexcept
	{
		return mPeers.at(idx).second;
	}

	size_t NetworkServerComponent::size() noexcept
	{
		return mPeers.size();
	}

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
		NetworkInstance* peer,
		const std::size_t sz)
	{
		std::int32_t len = sizeof(PrivateAddressData);
		static NetworkPacket packet{};

		std::int32_t res = ::recvfrom(server->mSocket,
			reinterpret_cast<char*>(&packet),
			sz,
			0,
			reinterpret_cast<sockaddr*>(&peer->address),
			&len);

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

			return;
		}

		if (peer->packet.magic[0] == XPLICIT_NETWORK_MAG_0 &&
			peer->packet.magic[1] == XPLICIT_NETWORK_MAG_1 &&
			peer->packet.magic[2] == XPLICIT_NETWORK_MAG_2 &&
			peer->packet.version == XPLICIT_NETWORK_VERSION)
		{
			peer->packet = packet;
		}
		else
		{
			xplicit_invalidate_peer(peer);
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
					reinterpret_cast<sockaddr*>(&server->get(index)->address),
					&fromLen);

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

					continue;
				}

				if (!xplicit_recv_packet(server, index, packet))
					xplicit_invalidate_peer(server->get(index));
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
					reinterpret_cast<sockaddr*>(&server->get(i)->address),
					sizeof(PrivateAddressData));

			}
		}
	}

	void NetworkServerHelper::send_to(
		NetworkServerComponent* server,
		NetworkInstance* peer,
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
				reinterpret_cast<sockaddr*>(&peer->address),
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

					if (equals(server->get(second_peer_idx)->address, server->get(peer_idx)->address))
					{
						server->get(second_peer_idx)->reset();
						server->get(second_peer_idx)->unique_addr.invalidate();
					}
				}
			}
		}
	}
}