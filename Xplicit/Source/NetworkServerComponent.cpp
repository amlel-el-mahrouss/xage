/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
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
		mDns(ip),
		mSslCtx(SSL_CTX_new(DTLSv1_server_method())),
		mSsl(nullptr),
		mBio(nullptr)
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

		mBio = BIO_new_dgram(mSocket, BIO_NOCLOSE);

		mSsl = SSL_new(mSslCtx);
		SSL_set_bio(mSsl, mBio, mBio);

		/* Enable cookie exchange */
		SSL_set_options(mSsl, SSL_OP_COOKIE_EXCHANGE);

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

	NetworkServerComponent::~NetworkServerComponent() 
	{
		if (XPLICIT_SHUTDOWN(mSocket, SD_BOTH) <= 0)
			XPLICIT_CLOSE(mSocket);

		SSL_free(mSsl);
		SSL_CTX_free(mSslCtx);
	}

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
		sockaddr rhs;

		if (!DTLSv1_listen(server->mSsl, (BIO_ADDR*)&rhs))
			return;

		NetworkPacket packet{};

		const sockaddr lhs = *reinterpret_cast<sockaddr*>(&peer->address);

		if (const auto ret = SSL_read(server->mSsl,
			reinterpret_cast<char*>(&packet),
			sizeof(NetworkPacket)); ret <= 0)
		{
			switch (SSL_get_error(server->mSsl, ret))
			{
			case SSL_ERROR_WANT_READ:
			case SSL_ERROR_WANT_WRITE:
			case SSL_ERROR_WANT_CONNECT:
			case SSL_ERROR_WANT_ACCEPT:
			case SSL_ERROR_ZERO_RETURN:
			case SSL_ERROR_SSL:
				return;
			case SSL_ERROR_SYSCALL:
			{
				int err = errno;
				switch (err)
				{
				case EWOULDBLOCK:
				{
					constexpr timeval XPLICIT_TIME = { .tv_sec = 1, .tv_usec = 0 };
					BIO_ctrl(server->mBio, BIO_CTRL_DGRAM_SET_RECV_TIMEOUT, 0, (void*)&XPLICIT_TIME);

					break;
				}
				}
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

		if (const auto ret = SSL_write(server->mSsl, reinterpret_cast<const char*>(&peer->packet),
			sizeof(NetworkPacket)); ret <= 0)
		{
			switch (SSL_get_error(server->mSsl, ret))
			{
			case SSL_ERROR_WANT_READ:
			case SSL_ERROR_WANT_WRITE:
			case SSL_ERROR_WANT_CONNECT:
			case SSL_ERROR_WANT_ACCEPT:
			case SSL_ERROR_ZERO_RETURN:
			case SSL_ERROR_SSL:
				return;
			case SSL_ERROR_SYSCALL:
			{
				int err = errno;
				switch (err)
				{
				case EWOULDBLOCK:
				{
					constexpr timeval XPLICIT_TIME = { .tv_sec = 1, .tv_usec = 0 };
					BIO_ctrl(server->mBio, BIO_CTRL_DGRAM_SET_RECV_TIMEOUT, 0, (void*) &XPLICIT_TIME);

					break;
				}
				}
			}
			default:
				break;
			}
		}
	}
}