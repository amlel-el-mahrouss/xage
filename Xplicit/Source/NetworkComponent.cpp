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
		mHost(nullptr)
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

		if (mHost)
			enet_host_destroy(mHost);
	}

	bool NetworkComponent::connect(const char* ip)
	{
		if (mSocket == SOCKET_ERROR)
			return false;

		mHost = enet_host_create(nullptr, 1, 2, 0, 0);

		if (mHost == nullptr)
			throw NetworkError(NETWORK_ERR_BAD_CHALLENGE);

		ENetAddress address;

		enet_address_set_host(&address, ip);
		address.port = XPLICIT_NETWORK_PORT;

		mXnetHost = enet_host_connect(mHost, &address, XPLICIT_NUM_CHANNELS, 0);

		if (mXnetHost == nullptr)
			throw NetworkError(NETWORK_ERR_BAD_CHALLENGE);

		return true;
	}

	bool NetworkComponent::channel(const std::uint32_t& channelId) noexcept
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

		ENetPacket* pckt = enet_packet_create((const void*)&packet, 
			sizeof(NetworkPacket), 
			ENET_PACKET_FLAG_RELIABLE);

		if (enet_peer_send(mXnetHost, mChannelID, pckt) == 0)
			enet_packet_destroy(pckt);

		return true;
	}

	void NetworkComponent::update()
	{
		static ENetEvent env;
	
		while (enet_host_service(mHost, &env, XPLICIT_WAIT_TIME) > 0)
		{
			switch (env.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
			{
				if (env.packet->dataLength != sizeof(NetworkPacket))
					break;

				mChannelID = env.channelID;
				mPacket = *(NetworkPacket*)env.packet->data;

				enet_packet_destroy(env.packet);

				break;
			}
			}
		}
	}

	bool NetworkComponent::read(NetworkPacket& packet, const std::size_t sz)
	{
		//! we gotta clear this one as we don't know if RST was sent.
		mReset = false;

		if (mPacket.magic[0] == XPLICIT_NETWORK_MAG_0 && mPacket.magic[1] == XPLICIT_NETWORK_MAG_1 &&
			mPacket.magic[2] == XPLICIT_NETWORK_MAG_2 && mPacket.version == XPLICIT_NETWORK_VERSION)
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