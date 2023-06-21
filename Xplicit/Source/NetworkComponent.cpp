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
	static void xplicit_set_ioctl(Socket sock)
	{
		auto ul = 1UL;
		const auto error = XPLICIT_IOCTL(sock, static_cast<long>(SOCKET_FLAG::NON_BLOCKING), &ul);
		(void)error;

		XPLICIT_ASSERT(error == NO_ERROR);
	}


	NetworkError::NetworkError(const int what) 
		: std::runtime_error("Network error, XplicitNgine encountered an unrecoverable error.")
#ifdef XPLICIT_WINDOWS
		, mErr(WSAGetLastError())
#endif
	{
#ifdef XPLICIT_DEBUG
		std::string err = "Error code: ";
		err += std::to_string(mErr);

		XPLICIT_CRITICAL(err);
#endif
	}

	int NetworkError::error() const noexcept { return mErr; }

	// NetworkComponent Constructor
	NetworkComponent::NetworkComponent()
		:
		Component(),
		mReset(false),
		mChannelID(XPLICIT_CHANNEL_DATA),
		mSocket(Network::SOCKET_TYPE::UDP),
		mSockAddrIn(),
		mPacket()
	{
		if (!mSocket)
			throw NetworkError(NETWORK_ERR_BAD_CHALLENGE);
		
		xplicit_set_ioctl(mSocket.PublicSocket);
	}

	NetworkComponent::~NetworkComponent()
	{
		if (XPLICIT_SHUTDOWN(mSocket.PublicSocket, SD_BOTH) == SOCKET_ERROR)
			XPLICIT_CLOSE(mSocket.PublicSocket);
	}

	const char* NetworkComponent::name() noexcept { return ("NetworkComponent"); }

	COMPONENT_TYPE NetworkComponent::type() noexcept { return COMPONENT_NETWORK; }

	bool NetworkComponent::should_update() noexcept { return false; }

	bool NetworkComponent::connect(const char* ip) noexcept
	{
		memset(&mSockAddrIn, 0, sizeof(sockaddr_in));

		mSockAddrIn.sin_addr.S_un.S_addr = inet_addr(ip);
		mSockAddrIn.sin_family = AF_INET;
		mSockAddrIn.sin_port = htons(XPLICIT_NETWORK_PORT);
		
		return ::connect(mSocket.PublicSocket, reinterpret_cast<struct sockaddr*>(&mSockAddrIn), sizeof(sockaddr_in)) != SOCKET_ERROR;
	}

	bool NetworkComponent::set_channel(const std::uint32_t& channelId) noexcept
	{
		if (channelId > XPLICIT_NUM_CHANNELS)
			return false;

		mChannelID = channelId;

		return true;
	}

	bool NetworkComponent::send(NetworkPacket& packet)
	{
		packet.magic[0] = XPLICIT_NETWORK_MAG_0;
		packet.magic[1] = XPLICIT_NETWORK_MAG_1;
		packet.magic[2] = XPLICIT_NETWORK_MAG_2;

		packet.channel = mChannelID;
		packet.version = XPLICIT_NETWORK_VERSION;
		
		if (const auto res = ::sendto(mSocket,
			reinterpret_cast<const char*>(&packet), 
			sizeof(NetworkPacket), 
			0,
			reinterpret_cast<struct sockaddr*>(&mSockAddrIn), 
			sizeof(mSockAddrIn)) == SOCKET_ERROR)
		{
			const auto err = WSAGetLastError();
			return res != SOCKET_ERROR || err != WSAEWOULDBLOCK;
		}
		else
		{
			return true;
		}
	}

	void NetworkComponent::update() {}

	bool NetworkComponent::read(NetworkPacket& packet)
	{
		//! we gotta clear this one as we don't know if RST was sent.
		mReset = false;

		std::int32_t len = sizeof(struct sockaddr_in);

		const std::int32_t err = ::recvfrom(mSocket.PublicSocket, 
			reinterpret_cast<char*>(&mPacket), 
			sizeof(NetworkPacket), 
			0, 
			reinterpret_cast<struct sockaddr*>(&mSockAddrIn), 
			&len);

		if (len < 1)
			return false;
		
		if (err == SOCKET_ERROR)
		{
			switch (WSAGetLastError())
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

	bool NetworkComponent::is_reset() const noexcept { return mReset; }

	NetworkPacket& NetworkComponent::get() noexcept { return mPacket; }
}