/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
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
		: std::runtime_error("Xplicit did ran into network issues, we're sorry!")
#ifdef XPLICIT_WINDOWS
		, mErr(WSAGetLastError())
#endif
	{
#ifdef XPLICIT_DEBUG
		String err = "Error code: ";
		err += std::to_string(mErr);

		XPLICIT_CRITICAL(err);

#ifdef XPLICIT_WINDOWS
		OutputDebugStringA(err.c_str());
#endif // XPLICIT_WINDOWS
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
		mTargetAddress(),
		mPacket(),
		mHash(-1),
		mSslCtx(SSL_CTX_new(DTLSv1_client_method())),
		mSsl(nullptr)
	{
		if (!mSocket)
			throw NetworkError(NETWORK_ERR_BAD_CHALLENGE);
		
		xplicit_set_ioctl(mSocket.PublicSocket);
	}

	NetworkComponent::~NetworkComponent()
	{
		if (XPLICIT_SHUTDOWN(mSocket.PublicSocket, SD_BOTH) == SOCKET_ERROR)
			XPLICIT_CLOSE(mSocket.PublicSocket);

		SSL_free(mSsl);
		SSL_CTX_free(mSslCtx);
	}

	const char* NetworkComponent::name() noexcept { return ("NetworkComponent"); }

	COMPONENT_TYPE NetworkComponent::type() noexcept { return COMPONENT_NETWORK; }

	bool NetworkComponent::should_update() noexcept { return true; }

	bool NetworkComponent::connect(const char* ip, const char* port) noexcept
	{
		memset(&mTargetAddress, 0, sizeof(sockaddr_in));

		mTargetAddress.sin_addr.S_un.S_addr = inet_addr(ip);
		mTargetAddress.sin_family = AF_INET;
		mTargetAddress.sin_port = htons(std::atoi(port));
		
		if (::connect(mSocket.PublicSocket, reinterpret_cast<struct sockaddr*>(&mTargetAddress), sizeof(sockaddr_in)) != SOCKET_ERROR)
		{
			/* Set new fd and set BIO to connected */
			mBio = SSL_get_rbio(mSsl);
			BIO_set_fd(mBio, mSocket.PublicSocket, BIO_NOCLOSE);
			BIO_ctrl(mBio, BIO_CTRL_DGRAM_SET_CONNECTED, 0, &mTargetAddress);

			/* Finish handshake */
			SSL_accept(mSsl);
		}
	}

	bool NetworkComponent::set_hash(const std::int64_t& hash) noexcept
	{
		if (mHash == -1)
		{
			mHash = hash;
			return true;
		}

		return false;
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
		packet.hash = mHash;
		packet.channel = mChannelID;

		packet.size = sizeof(NetworkPacket);

		packet.magic[0] = XPLICIT_NETWORK_MAG_0;
		packet.magic[1] = XPLICIT_NETWORK_MAG_1;
		packet.magic[2] = XPLICIT_NETWORK_MAG_2;

		packet.version = XPLICIT_NETWORK_VERSION;

		packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;
		
		if (SSL_write(mSsl,
			reinterpret_cast<const char*>(&packet), 
			sizeof(NetworkPacket)) <= 0)
		{
			return false;
		}
		
		return true;
	}

	void NetworkComponent::update()
	{
		static NetworkPacket ack;
		this->send(ack);
	}

	bool NetworkComponent::read(NetworkPacket& packet)
	{
		mReset = false;

		
		
		if (const std::int32_t err = SSL_read(mSsl,
			reinterpret_cast<char*>(&mPacket),
			sizeof(NetworkPacket)); err <= 0)
		{
			return false;
		}

		if (mPacket.magic[0] == XPLICIT_NETWORK_MAG_0 && 
			mPacket.magic[1] == XPLICIT_NETWORK_MAG_1 &&
			mPacket.magic[2] == XPLICIT_NETWORK_MAG_2 && 
			mPacket.version == XPLICIT_NETWORK_VERSION)
		{
			//! packet valid, validate.

			packet = mPacket;
			return true;
		}

		return false;
	}

	bool NetworkComponent::is_reset() const noexcept { return mReset; }

	NetworkPacket& NetworkComponent::get() noexcept { return mPacket; }
}