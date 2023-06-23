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
#include "CommonEngine.h"

namespace Xplicit
{
    NetworkInstance::NetworkInstance()
	: 
        packet(), 
		hash(XPLICIT_INVALID_HASH),
		public_hash(XPLICIT_INVALID_HASH),
        address(),
        status(NETWORK_STAT_DISCONNECTED)
	{}

    NetworkInstance::~NetworkInstance() = default;

    void NetworkInstance::reset() noexcept
    {
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
        if (mTimeout)
            return;

        mTimeout = true;

        Thread timeout([&]() {
            this->packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_INVALID;

            // Sleep for tirty seconds, let the client be aware of our packet.
            std::this_thread::sleep_for(std::chrono::seconds(5));

            if (this->status == NETWORK_STAT_DISCONNECTED)
            {
                mTimeout = false;

#ifdef XPLICIT_DEBUG
                XPLICIT_INFO("Player disconnected before timeout, action canceled.");
#endif // ifdef XPLICIT_DEBUG

                return;
            }

            if (this->packet.cmd[XPLICIT_NETWORK_CMD_ACK] != NETWORK_CMD_ACK)
                this->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;

            mTimeout = false;
        });

        timeout.detach();
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