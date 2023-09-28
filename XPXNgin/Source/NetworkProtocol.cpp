/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "NetworkProtocol.h"
#include "NetworkServerComponent.h"

namespace XPX
{
    NetworkPeer::NetworkPeer(const XPX::Auth::XplicitID& id)
	:
        address(),
        xplicit_id(id),
        packet(),
		port(0),
		channel(XPLICIT_CHANNEL_DATA),
		hash(XPLICIT_INVALID_HASH),
		public_hash(XPLICIT_INVALID_HASH),
        status(NETWORK_STAT_DISCONNECTED)
	{
        memset(&this->address, 0, sizeof(PrivateAddressData));
    }

    NetworkPeer::~NetworkPeer() = default;

    void NetworkPeer::reset() noexcept
    {
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

    constexpr auto XPLICIT_MAX_TIMEOUT = std::chrono::seconds(5);

    void NetworkPeer::timeout() noexcept
    {
        if (this->status == NETWORK_STAT_STASIS ||
            this->status == NETWORK_STAT_DISCONNECTED)
            return;

        Thread timeout([&]() {
            this->status = NETWORK_STAT_STASIS;

            if (this->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN &&
                this->status == NETWORK_STAT_CONNECTED)
            {
                this->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
                return;
            }

            // Sleep for five seconds, let the client be aware of our packet.
            std::this_thread::sleep_for(XPLICIT_MAX_TIMEOUT);

            // check for either no acknowledge
            if (this->packet.cmd[XPLICIT_NETWORK_CMD_ACK] != NETWORK_CMD_ACK)
            {
                if (this->status == NETWORK_STAT_STASIS)
                {
                    XPLICIT_INFO("[TIMEOUT] IP: " + this->ip_address);

                    this->packet.hash = this->hash;
                    this->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
                }
            }
            else
            {
                if (this->ip_address.empty())
                {
                    XPLICIT_INFO("[INVALID] XPX_ID: " + this->xplicit_id.as_string());

                    this->packet.hash = this->hash;
                    this->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;

                    return;
                }

                XPLICIT_INFO("[CHECK] IP: " + this->ip_address);

                this->packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_INVALID;
                this->status = NETWORK_STAT_CONNECTED;
            }

            });

        timeout.detach();
    }

    NetworkPeer::UniqueAddress::UniqueAddress()
		: mUuid(UUIDFactory::version<4>())
	{}

    NetworkPeer::UniqueAddress::~UniqueAddress() = default;

    const UUID& NetworkPeer::UniqueAddress::get() noexcept { return mUuid; }

    void NetworkPeer::UniqueAddress::invalidate() noexcept
    {
        mPublicUuid = UUIDFactory::version<4>();
        mUuid = UUIDFactory::version<4>();
    }

    const UUID& NetworkPeer::UniqueAddress::get_public_uuid() noexcept { return this->get(); }

    const UUID& NetworkPeer::UniqueAddress::get_private_uuid() noexcept { return mUuid; }

    String address_to_string(const PrivateAddressData& ip) { return inet_ntoa(ip.sin_addr); }

    String address_to_string(NetworkPeer* instance) { return address_to_string(instance->address); }

    const bool equals(const PrivateAddressData& lhs, const PrivateAddressData& rhs) { return lhs.sin_addr.s_addr == rhs.sin_addr.s_addr; }

    XPLICIT_API NetworkPeer* find_peer(uuids::uuid& uuid)
    {
        NetworkServerComponent* server = ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");

        for (size_t i = 0; i < server->size(); ++i)
        {
            auto peer = server->get(i);

            XPLICIT_ASSERT(peer);

            if (peer->unique_addr.get_private_uuid() == uuid)
                return peer;
        }

        return nullptr;
    }
}