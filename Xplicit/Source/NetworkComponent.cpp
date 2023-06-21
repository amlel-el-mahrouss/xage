/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "NetworkComponent.h"
#include "CommonEngine.h"

namespace Xplicit 
{
	NetworkError::NetworkError(const int what) 
		: std::runtime_error("Xplicit Network Error")
#ifdef XPLICIT_WINDOWS
		, mErr(WSAGetLastError())
#endif
	{
#ifdef XPLICIT_DEBUG
		std::string err = "NetworkError, error code: ";
		err += std::to_string(mErr);

		XPLICIT_CRITICAL(err);
#endif
	}

	int NetworkError::error() const noexcept { return mErr; }

	// NetworkComponent Constructor
	NetworkComponent::NetworkComponent()
		: Component(),
		mReset(false),
		mChannelID(XPLICIT_CHANNEL_DATA),
		mSocket(Network::SOCKET_TYPE::UDP),
		mSockAddrIn(),
		mPacket()
	{
#ifndef _NDEBUG
		XPLICIT_INFO("Created NetworkComponent");
#endif
	}

	NetworkComponent::~NetworkComponent()
	{
#ifndef _NDEBUG
		XPLICIT_INFO("~NetworkComponent, Epoch: " + std::to_string(xplicit_get_epoch()));
#endif
	}

	bool NetworkComponent::connect(const char* ip)
	{
		memset(&mSockAddrIn, 0, sizeof(sockaddr_in));

		mSockAddrIn.sin_family = AF_INET;
		inet_pton(AF_INET, ip, &mSockAddrIn.sin_addr);
		mSockAddrIn.sin_port = htons(XPLICIT_NETWORK_PORT);

		::connect(mSocket.PublicSocket, reinterpret_cast<struct sockaddr*>(&mSockAddrIn), sizeof(sockaddr));

		return true;
	}

	bool NetworkComponent::set_channel(const std::uint32_t& channelId) noexcept
	{
		if (channelId > XPLICIT_NUM_CHANNELS)
			return false;

		mChannelID = channelId;

		return true;
	}

	bool NetworkComponent::send(NetworkPacket& packet, const std::size_t sz)
	{
		packet.magic[0] = XPLICIT_NETWORK_MAG_0;
		packet.magic[1] = XPLICIT_NETWORK_MAG_1;
		packet.magic[2] = XPLICIT_NETWORK_MAG_2;

		packet.version = XPLICIT_NETWORK_VERSION;

		int res = ::sendto(mSocket, reinterpret_cast<const char*>(&packet), sz, 0,
			reinterpret_cast<struct sockaddr*>(&mSockAddrIn), sizeof(sockaddr_in));

		if (res == SOCKET_ERROR)
			throw NetworkError(NETWORK_ERR_INTERNAL_ERROR);

		return true;
	}

	void NetworkComponent::update() {}

	bool NetworkComponent::read(NetworkPacket& packet, const std::size_t sz)
	{
		//! we gotta clear this one as we don't know if RST was sent.
		mReset = false;

		std::int32_t len = sizeof(struct sockaddr_in);

		std::int32_t err = ::recvfrom(mSocket.PublicSocket, 
			reinterpret_cast<char*>(&mPacket), sz, 0, 
			reinterpret_cast<struct sockaddr*>(&mSockAddrIn), 
			&len);

		if (len < 1)
			return false;

		if (err == SOCKET_ERROR)
		{
			auto errWsa = WSAGetLastError();

			switch (errWsa)
			{
			case WSAECONNRESET:
			{
				mReset = true;
				break;
			}
			default:
				break;
			}

			return false;
		}

		if (mPacket.magic[0] == XPLICIT_NETWORK_MAG_0 && 
			mPacket.magic[1] == XPLICIT_NETWORK_MAG_1 &&
			mPacket.magic[2] == XPLICIT_NETWORK_MAG_2 && 
			mPacket.version == XPLICIT_NETWORK_VERSION)
		{
			mPacket = packet;

			//! return true here, the packet is valid!
			return true;
		}

		return false;
	}

	bool NetworkComponent::is_reset() noexcept { return mReset; }

	NetworkPacket& NetworkComponent::get() noexcept { return mPacket; }
}