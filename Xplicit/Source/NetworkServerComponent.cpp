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
		const auto error = XPLICIT_IOCTL(sock, static_cast<long>(SOCKET_FLAG::NON_BLOCKING), &ul);
		(void)error;

		XPLICIT_ASSERT(error == NO_ERROR);
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

	NetworkInstance* NetworkServerComponent::get(const std::size_t& idx) const noexcept { return mPeers.at(idx).second; }

	size_t NetworkServerComponent::size() const noexcept { return mPeers.size(); }

	void NetworkServerHelper::recv(NetworkServerComponent* server, NetworkInstance* peer)
	{
		if (!peer || !server)
			return;

		std::int32_t from_len = sizeof(PrivateAddressData);
		NetworkPacket packet{};

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

				static constexpr timeval timeout = { .tv_sec = 1, .tv_usec = 0 };

				::select(0, &fd, nullptr, nullptr, &timeout);
#ifdef XPLICIT_DEBUG
				XPLICIT_INFO("EWOULDBLOCK: Socket is blocked...");
#endif // ifdef XPLICIT_DEBUG

				break;
			}
			case WSAECONNABORTED:
			{
				XPLICIT_INFO("WSAECONNABORTED: Socket' connection aborted...");
				break;
			}
			case WSAECONNRESET:
			{
				XPLICIT_INFO("WSAECONNRESET: Socket' connection reset...");
				break;
			}
			default:
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

	void NetworkServerHelper::accept(NetworkServerComponent* server)
	{
		XPLICIT_ASSERT(server);

		for (std::size_t i = 0; i < server->size(); ++i)
		{
			Thread listeners([&]() {
				const std::size_t peer_at = i;

				while (server)
				{
					const auto peer = server->get(peer_at);

					NetworkServerHelper::recv(server, peer);

					std::this_thread::sleep_for(std::chrono::milliseconds(60));
				}
				});
		}
	}

	void NetworkServerHelper::send(NetworkServerComponent* server)
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
					sizeof(NetworkPacket),
					0,
					reinterpret_cast<sockaddr*>(&server->get(i)->address),
					sizeof(PrivateAddressData));

			}
		}
	}

	void NetworkServerHelper::send(NetworkServerComponent* server, NetworkInstance* peer)
	{
		if (peer && server)
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
	}
}