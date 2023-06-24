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
        status(NETWORK_STAT_INVALID)
	{
        memset(&this->address, 0, sizeof(PrivateAddressData));
    }

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

        memset(&this->address, 0, sizeof(PrivateAddressData));

        this->ip_address.clear();

        this->status = NETWORK_STAT_DISCONNECTED;

        this->hash = XPLICIT_INVALID_HASH;
        this->public_hash = XPLICIT_INVALID_HASH;
    }

    constexpr std::int32_t XPLICIT_MAX_TIMEOUT = 25;

    void NetworkInstance::timeout() noexcept
    {
        if (this->status == NETWORK_STAT_INVALID)
            return;

        this->status = NETWORK_STAT_INVALID;

		Thread timeout([&]() {
			// Sleep for tirty seconds, let the client be aware of our packet.
            std::this_thread::sleep_for(std::chrono::seconds(XPLICIT_MAX_TIMEOUT));

            if (this->packet.cmd[XPLICIT_NETWORK_CMD_ACK] != NETWORK_CMD_ACK)
            {
                this->packet.hash = this->hash;
                this->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
            }

            this->status = NETWORK_STAT_CONNECTED;
        });

        timeout.detach();
    }

    NetworkInstance::UniqueAddress::UniqueAddress()
		: mUuid(UUIDFactory::version<4>())
	{}

    NetworkInstance::UniqueAddress::~UniqueAddress() = default;

    XPLICIT_API String address_to_string(NetworkInstance* instance)
    {
        return inet_ntoa(instance->address.sin_addr);
    }

    bool equals(const PrivateAddressData& lhs, const PrivateAddressData& rhs)
    {
        return lhs.sin_addr.S_un.S_addr == rhs.sin_addr.S_un.S_addr;
    }
}