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

	NetworkInstance* NetworkServerComponent::get(const std::size_t& idx) const noexcept 
	{ 
		if (mPeers.empty())
			return nullptr;

		return mPeers.at(idx).second; 
	}

	size_t NetworkServerComponent::size() const noexcept { return mPeers.size(); }

#define XPLICIT_CONNRESET (1)

	std::int32_t NetworkServerContext::try_recv(NetworkServerComponent* server, NetworkInstance* peer) noexcept
	{
		if (!peer || !server)
			return 0;

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
				return XPLICIT_CONNRESET;
			}
			case WSAECONNRESET:
			{
				XPLICIT_INFO("WSAECONNABORTED: Socket' connection reset...");

				break;
			}
			default:
				break;
			}

			return 0;
		}

		peer->packet = packet;

		if (peer->packet.magic[0] != XPLICIT_NETWORK_MAG_0 ||
			peer->packet.magic[1] != XPLICIT_NETWORK_MAG_1 ||
			peer->packet.magic[2] != XPLICIT_NETWORK_MAG_2 ||
			peer->packet.version != XPLICIT_NETWORK_VERSION)
		{
			xplicit_invalidate_peer(peer);
		}

		return 0;
	}

	NetworkServerContext::NETWORK_CONTEXT NetworkServerContext::context = NetworkServerContext::NETWORK_CONTEXT::DISCONNECTED;

	void NetworkServerContext::accept_recv(NetworkServerComponent* server) noexcept
	{
		XPLICIT_ASSERT(server);


		/* We create recievers threads here. */

		for (std::size_t i = 0; i < server->size(); ++i)
		{
			Thread([&]() {
				const std::size_t peer_at = i;
				auto peer = server->get(peer_at);

				while (server)
				{
					if (!peer)
					{
						peer = server->get(peer_at);
						XPLICIT_ASSERT(peer != nullptr);

						continue;
					}

					if (NetworkServerContext::try_recv(server, peer) == XPLICIT_CONNRESET)
					{
						peer->reset();
						peer->unique_addr.invalidate();
					}

					// sleep for one tick
					std::this_thread::sleep_for(std::chrono::milliseconds(60));
				}
			});
		}
	}

	void NetworkServerContext::try_send(NetworkServerComponent* server, NetworkInstance* peer) noexcept
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

	void NetworkServerContext::accept_send(NetworkServerComponent* server) noexcept
	{
		if (server)
		{
			for (std::size_t i = 0; i < server->size(); ++i)
			{
				Thread([&]() {
					const std::size_t peer_at = i;
					auto peer = server->get(peer_at);

					while (server)
					{
						if (!peer)
						{
							peer = server->get(peer_at);
							continue;
						}

						NetworkServerContext::try_send(server, peer);

						// sleep for one tick
						std::this_thread::sleep_for(std::chrono::milliseconds(60));
					}
				});
			}

			// finally make it online!
			context = NETWORK_CONTEXT::ONLINE;
		}
	}
}