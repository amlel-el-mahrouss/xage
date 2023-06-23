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

#include "NetworkProtocol.h"
#include "NetworkServerComponent.h"

namespace Xplicit
{
    NetworkInstance::NetworkInstance()
	: 
        packet(),
		port(0),
		channel(XPLICIT_CHANNEL_DATA),
		hash(XPLICIT_INVALID_HASH),
		public_hash(XPLICIT_INVALID_HASH),
        address(),
        status(NETWORK_STAT_DISCONNECTED)
	{}

    NetworkInstance::~NetworkInstance() = default;

    void NetworkInstance::reset() noexcept
    {
        /* don't do anything if disconnected. */
        if (status == NETWORK_STAT_DISCONNECTED)
            return;

        for (size_t cmd_index = 0; cmd_index < XPLICIT_NETWORK_CMD_MAX; ++cmd_index)
        {
            this->packet.cmd[cmd_index] = NETWORK_CMD_INVALID;
        }

        this->ip_address.clear();
        this->status = NETWORK_STAT_DISCONNECTED;

        this->hash = XPLICIT_INVALID_HASH;
        this->public_hash = XPLICIT_INVALID_HASH;
    }

    void NetworkInstance::timeout() noexcept
    {
        this->status = NETWORK_STAT_INVALID;

		Thread timeout([&]() {
			// Sleep for tirty seconds, let the client be aware of our packet.
            std::this_thread::sleep_for(std::chrono::seconds(5));

            if (this->status == NETWORK_STAT_DISCONNECTED)
            {
#ifdef XPLICIT_DEBUG
                XPLICIT_INFO("Player disconnected before timeout, action canceled.");
#endif // ifdef XPLICIT_DEBUG
            }

            if (this->packet.cmd[XPLICIT_NETWORK_CMD_ACK] != NETWORK_CMD_ACK)
            {
#ifdef XPLICIT_DEBUG
                XPLICIT_INFO("Player will be disconnected.");
#endif // ifdef XPLICIT_DEBUG

                this->packet.hash = this->hash;
                this->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
            }
            else
            {
                this->status = NETWORK_STAT_CONNECTED;
            }
        });
    }

    NetworkInstance::UniqueAddress::UniqueAddress()
		: mUuid(UUIDFactory::version<4>())
	{}

    NetworkInstance::UniqueAddress::~UniqueAddress() = default;

    bool equals(const PrivateAddressData& lhs, const PrivateAddressData& rhs)
    {
        return lhs.sin_addr.S_un.S_addr == rhs.sin_addr.S_un.S_addr;
    }
}