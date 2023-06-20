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

	NetworkServerComponent::NetworkServerComponent(const char* ip)
		:
		mSocket(INVALID_SOCKET), 
		mAddress(ip),
		mXnetServer(nullptr)
	{
#ifndef _NDEBUG
		std::string message;
		message += "Class NetworkServerComponent, Epoch: ";
		message += std::to_string(xplicit_get_epoch());

		XPLICIT_INFO(message);
#endif

		enet_address_set_host(&mXnetAddress, ip);
		mXnetAddress.port = XPLICIT_NETWORK_PORT;

		mXnetServer = enet_host_create(&mXnetAddress, 
			XPLICIT_MAX_CONNECTIONS, 
			XPLICIT_NUM_CHANNELS,
			0, 
			0);

		if (!mXnetServer)
			throw NetworkError(NETWORK_ERR_INTERNAL_ERROR);

		// Let's preallocate the clients.
		// So we don't have to allocate them.

		for (size_t i = 0; i < XPLICIT_MAX_CONNECTIONS; i++)
		{
			NetworkInstance* inst = new NetworkInstance();
			XPLICIT_ASSERT(inst);

			mPeers.push_back(std::make_pair(Auth::XplicitID(XPLICIT_UNIVERSE_ID, xplicit_get_epoch()), inst));
		}

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("[SERVER] IP Address Version 4: " + mAddress);
#endif
	}

	const char* NetworkServerComponent::name() noexcept { return ("NetworkServerComponent"); }

	COMPONENT_TYPE NetworkServerComponent::type() noexcept { return COMPONENT_NETWORK; }

	void NetworkServerComponent::update() 
	{
		ENetEvent event;

		/* Wait up to 1000 milliseconds for an event. */
		while (enet_host_service(mXnetServer, &event, XPLICIT_WAIT_TIME) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				for (std::size_t index = 0; index < mPeers.size(); index++)
				{
					if (mPeers[index].second->address == XPLICIT_BAD_ADDRESS)
					{
						mPeers[index].second->address = event.peer->address.host;
						mPeers[index].second->port = event.peer->address.port;
						
						event.peer->data = (void*)mPeers[index].first.as_string().c_str();
					
						mPeers[index].second->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
						mPeers[index].second->packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

						break;
					}
				}

				break;
			}
			case ENET_EVENT_TYPE_RECEIVE:
				for (std::size_t index = 0; index < mPeers.size(); index++)
				{
					if (mPeers[index].second->address == event.peer->address.host &&
						mPeers[index].second->port == event.peer->address.port)
					{
						mPeers[index].second->packet = *(NetworkPacket*)event.packet->data;
						
						mPeers[index].second->channel = event.channelID;

						break;
					}
				}

				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				for (std::size_t index = 0; index < mPeers.size(); index++)
				{
					if (mPeers[index].second->address == event.peer->address.host &&
						mPeers[index].second->port == event.peer->address.port)
					{
						mPeers[index].second->status = NETWORK_STAT_DISCONNECTED;
						mPeers[index].second->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;

						break;
					}
				}

				/*!
				   Reset the peer's client information. 
				   It removes the XplicitID associated with the player.
				   */
				event.peer->data = nullptr;
			}
		}
	}

	bool NetworkServerComponent::should_update() noexcept { return true; }

	NetworkServerComponent::~NetworkServerComponent()
	{
		// don't print that in release builds.
#ifndef _NDEBUG
		std::string message;
		message += "Class NetworkServerComponent::~NetworkServerComponent(), Epoch: ";
		message += std::to_string(xplicit_get_epoch());

		XPLICIT_INFO(message);
#endif

		if (mXnetServer)
			enet_host_destroy(mXnetServer);
	}

	const char* NetworkServerComponent::dns() noexcept { return mAddress.c_str(); }
}