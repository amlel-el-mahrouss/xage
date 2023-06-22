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
		auto ul = 1UL;
		auto error = XPLICIT_IOCTL(sock, static_cast<long>(SOCKET_FLAG::NON_BLOCKING), &ul);
		
		XPLICIT_ASSERT(error == NO_ERROR);
	
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
		Utils::InternetProtocolChecker checker;
		XPLICIT_ASSERT(checker(ip));
#endif // ifdef XPLICIT_DEBUG

		mSocket = XPLICIT_SOCKET(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (mSocket == SOCKET_ERROR)
			throw NetworkError(NETWORK_ERR_INTERNAL_ERROR);

		xplicit_set_ioctl(mSocket);

		sockaddr_in bindAddress = {};
		memset(&bindAddress, 0, sizeof(struct sockaddr_in));
		
		inet_pton(AF_INET, this->mDns.c_str(), &bindAddress.sin_addr.S_un.S_addr);

		bindAddress.sin_family = AF_INET;
		bindAddress.sin_port = htons(XPLICIT_NETWORK_PORT);
		
		if (::bind(mSocket, reinterpret_cast<SOCKADDR*>(&bindAddress), sizeof(bindAddress)) == SOCKET_ERROR)
			throw NetworkError(NETWORK_ERR_INTERNAL_ERROR);

		// !Let's preallocate the clients.
		// !So we don't have to allocate them.

		for (std::size_t index = 0; index < XPLICIT_MAX_CONNECTIONS; index++)
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

	const char* NetworkServerComponent::dns() const noexcept { return mDns.c_str(); }

	NetworkInstance* NetworkServerComponent::get(const std::size_t& idx) const noexcept 
	{ 
		if (mPeers.empty())
			return nullptr;

		return mPeers.at(idx).second; 
	}

	size_t NetworkServerComponent::size() const noexcept { return mPeers.size(); }

	static void xplicit_register_packet(NetworkPacket& packet, NetworkInstance* peer)
	{
		if (packet.magic[0] != XPLICIT_NETWORK_MAG_0 ||
			packet.magic[1] != XPLICIT_NETWORK_MAG_1 ||
			packet.magic[2] != XPLICIT_NETWORK_MAG_2 ||
			packet.version != XPLICIT_NETWORK_VERSION)
		{
			xplicit_invalidate_peer(peer);
		}
		else if (packet.magic[0] == XPLICIT_NETWORK_MAG_0 &&
			packet.magic[1] == XPLICIT_NETWORK_MAG_1 &&
			packet.magic[2] == XPLICIT_NETWORK_MAG_2 &&
			packet.version == XPLICIT_NETWORK_VERSION)
		{
			peer->packet = packet;
		}
	}

	void NetworkServerContext::try_recv(NetworkServerComponent* server, NetworkInstance* peer) noexcept
	{
		if (!peer || !server)
			return;

		std::int32_t from_len = sizeof(PrivateAddressData);

		NetworkPacket packet{};
		sockaddr addr {};

		/*
		* 
		* Error handling algorithm
		* needed for a good winsock backend.
		* 
		* We handle sockets that didn't receive their data yet (WSAEWOULDBLOCK)
		* The connection aborted is not handled here.
		* 
		*/

		::accept(server->mSocket,
			reinterpret_cast<sockaddr*>(&peer->address),
			&from_len);

		if (::recvfrom(server->mSocket,
			reinterpret_cast<char*>(&packet),
			sizeof(NetworkPacket),
			0,
			reinterpret_cast<sockaddr*>(&peer->address),
			&from_len) == SOCKET_ERROR)
		{
			switch (WSAGetLastError())
			{
			case WSAEWOULDBLOCK:
			{
				fd_set fd;
				FD_ZERO(&fd);
				FD_SET(server->mSocket, &fd);

				static constexpr timeval timeout = { .tv_sec = 0, .tv_usec = 100000 };

				::select(0, &fd, nullptr, nullptr, &timeout);

				break;
			}
			case WSAECONNABORTED:
			{
				break;
			}
			default:
				break;
			}

			return;
		}

#define XPLICIT_INET_SIZE (14)

		/*
		 *
		 * Collision resolving algorithm
		 * If the ip matches assign to the matching ip it's respective packet.
		 * Otherwise don't do anything.
		 *
		 */

		for (std::size_t index = 0; index < server->size(); ++index)
		{
			if (server->get(index) == peer)
				continue;

			if (packet.hash == server->get(index)->hash)
				return;
		}

		xplicit_register_packet(packet, peer);
	}

	NetworkServerContext::NETWORK_CONTEXT NetworkServerContext::context = NetworkServerContext::NETWORK_CONTEXT::DISCONNECTED;

	void NetworkServerContext::accept_recv(NetworkServerComponent* server) noexcept
	{
		XPLICIT_ASSERT(server);


		/* We create receivers threads here. */

		for (std::size_t i = 0; i < server->size(); ++i)
		{
			Thread thrd([&](const std::size_t id) {
				const std::size_t peer_at = i;
				auto peer = server->get(peer_at);

				while (server)
				{
					XPLICIT_ASSERT(peer);
					NetworkServerContext::try_recv(server, peer);
				}
			}, i);
		}
	}

	void NetworkServerContext::try_send(NetworkServerComponent* server, NetworkInstance* peer) noexcept
	{
		if (peer->hash != peer->packet.hash)
			return;

		peer->packet.magic[0] = XPLICIT_NETWORK_MAG_0;
		peer->packet.magic[1] = XPLICIT_NETWORK_MAG_1;
		peer->packet.magic[2] = XPLICIT_NETWORK_MAG_2;

		peer->packet.version = XPLICIT_NETWORK_VERSION;

		::sendto(server->mSocket, reinterpret_cast<const char*>(&peer->packet),
			sizeof(NetworkPacket),
			0,
			reinterpret_cast<sockaddr*>(&peer->address),
			sizeof(PrivateAddressData));
	}

	void NetworkServerContext::accept_send(NetworkServerComponent* server) noexcept
	{
		if (server)
		{
			for (std::size_t i = 0; i < server->size(); ++i)
			{
				Thread thrd([&](const std::size_t id) {
					const std::size_t peer_at = i;
					auto peer = server->get(peer_at);

					XPLICIT_ASSERT(peer);

					while (server)
					{
						XPLICIT_ASSERT(peer);
						NetworkServerContext::try_send(server, peer);
					}
					}, i);
			}

			// finally make it online!
			context = NETWORK_CONTEXT::ONLINE;
		}
	}


	void NetworkServerContext::send(NetworkServerComponent* server, NetworkInstance* peer) noexcept
	{
		peer->packet.magic[0] = XPLICIT_NETWORK_MAG_0;
		peer->packet.magic[1] = XPLICIT_NETWORK_MAG_1;
		peer->packet.magic[2] = XPLICIT_NETWORK_MAG_2;

		peer->packet.version = XPLICIT_NETWORK_VERSION;

		::sendto(server->mSocket, reinterpret_cast<const char*>(&peer->packet),
			sizeof(NetworkPacket),
			0,
			reinterpret_cast<sockaddr*>(&peer->address),
			sizeof(PrivateAddressData));
	}

	void NetworkServerContext::recv(NetworkServerComponent* server, NetworkInstance* peer, NetworkPacket& packet) noexcept
	{
		XPLICIT_ASSERT(peer && server);

		std::int32_t from_len = sizeof(PrivateAddressData);

		::recvfrom(server->mSocket,
			reinterpret_cast<char*>(&packet),
			sizeof(NetworkPacket),
			0,
			reinterpret_cast<sockaddr*>(&peer->address),
			&from_len);
	}
}