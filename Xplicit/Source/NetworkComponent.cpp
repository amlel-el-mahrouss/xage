/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: NetworkComponent.cpp
 *			Purpose: XDP client
 *
 * =====================================================================
 */

#include "NetworkComponent.h"

namespace Xplicit 
{
	NetworkError::NetworkError(const int what) 
		: std::runtime_error("Xplicit Network Error")
#ifdef XPLICIT_WINDOWS
		, m_iErr(WSAGetLastError())
#endif
	{
#ifdef XPLICIT_DEBUG
		std::string err = "NetworkError, error code: ";
		err += std::to_string(m_iErr);

		XPLICIT_CRITICAL(err);
#endif
	}

	int NetworkError::error() const noexcept { return m_iErr; }

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
		: Component(),  mAddr(), mReset(false)
	{
#ifdef XPLICIT_WINDOWS
		mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (mSocket == SOCKET_ERROR)
			throw NetworkError(NETWORK_ERR_INTERNAL_ERROR);

		xplicit_set_ioctl(mSocket);
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
			closesocket(mSocket);

#ifndef _NDEBUG
		XPLICIT_INFO("~NetworkComponent, Epoch: " + std::to_string(xplicit_get_epoch()));
#endif

		WSACleanup();
	}

	bool NetworkComponent::reset() noexcept
	{
#ifdef XPLICIT_WINDOWS
		return shutdown(mSocket, SD_BOTH) != SOCKET_ERROR;
#else
#pragma error("DEFINE ME NetworkComponent.cpp")
#endif
	}

	bool NetworkComponent::connect(const char* ip)
	{
		if (mSocket == SOCKET_ERROR)
			return false;

#ifdef XPLICIT_WINDOWS
		memset(&mAddr, 0, sizeof(SOCKADDR_IN));

		mAddr.sin_family = AF_INET;
		inet_pton(AF_INET, ip, &mAddr.sin_addr);
		mAddr.sin_port = htons(XPLICIT_UDP_PORT);

		int result = ::connect(mSocket, reinterpret_cast<SOCKADDR*>(&mAddr), 
			sizeof(PrivateAddressData));

		if (result == SOCKET_ERROR)
			throw NetworkError(NETWORK_ERR_INTERNAL_ERROR);
#else
#pragma error("DEFINE ME NetworkComponent.cpp")
#endif

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("[NetworkComponent] Connected!");
#endif

		return true;
	}

	bool NetworkComponent::send(NetworkPacket& packet, const std::size_t sz)
	{
		packet.magic[0] = XPLICIT_NETWORK_MAG_0;
		packet.magic[1] = XPLICIT_NETWORK_MAG_1;
		packet.magic[2] = XPLICIT_NETWORK_MAG_2;
		packet.version = XPLICIT_NETWORK_VERSION;

#ifdef XPLICIT_WINDOWS
		int res = ::sendto(mSocket, reinterpret_cast<const char*>(&packet), sz, 0,
			reinterpret_cast<SOCKADDR*>(&mAddr), sizeof(mAddr));

		if (res == SOCKET_ERROR)
			throw NetworkError(NETWORK_ERR_INTERNAL_ERROR);
#else
#pragma error("DEFINE ME NetworkComponent.cpp")
#endif

		return true;
	}

	void NetworkComponent::update() 
	{

	}

	bool NetworkComponent::read(NetworkPacket& packet, const std::size_t sz)
	{
		mReset = false; // we gotta clear this one, we don't know if RST was sent.

		int length{ sizeof(struct sockaddr_in) };

#ifdef XPLICIT_WINDOWS
		int res = ::recvfrom(mSocket, reinterpret_cast<char*>(&packet), sz, 0,
			(struct sockaddr*)&mAddr, &length);
#else
#pragma error("DEFINE ME NetworkComponent.cpp")
#endif

		if (length > 0)
		{
			if (res == SOCKET_ERROR)
			{
				int err = WSAGetLastError();

				switch (err)
				{
				case WSAECONNRESET:
				{
					mReset = true;
					break;
				}
				}

				return false;
			}
		}

		if (packet.magic[0] == XPLICIT_NETWORK_MAG_0 && packet.magic[1] == XPLICIT_NETWORK_MAG_1 &&
			packet.magic[2] == XPLICIT_NETWORK_MAG_2 && packet.version == XPLICIT_NETWORK_VERSION)
		{
			mPacket = packet;
			return true;
		}

		return false;
	}

	bool NetworkComponent::is_reset() noexcept { return mReset; }

	NetworkPacket& NetworkComponent::get() noexcept { return mPacket; }
}