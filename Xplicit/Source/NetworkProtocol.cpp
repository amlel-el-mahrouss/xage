/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Protocol.cpp
 *			Purpose: Game Network Protocol
 *
 * =====================================================================
 */

#include "NetworkProtocol.h"

namespace Xplicit
{
    NetworkPeer::NetworkPeer() : packet(), addr(), stat(NETWORK_STAT_COUNT), unique_addr(), bad(false) {}
    NetworkPeer::~NetworkPeer() {}
    
    bool NetworkPeer::operator==(const NetworkPeer& cl)
    {
        return cl.addr.sin_addr.S_un.S_addr == this->addr.sin_addr.S_un.S_addr;
    }

    bool NetworkPeer::operator!=(const NetworkPeer& cl)
    {
        return cl.addr.sin_addr.S_un.S_addr != this->addr.sin_addr.S_un.S_addr;
    }

    void NetworkPeer::reset() noexcept
    {
        for (size_t cmd_index = 0; cmd_index < XPLICIT_NETWORK_CMD_MAX; cmd_index++)
        {
            packet.cmd[cmd_index] = NETWORK_CMD_INVALID;
        }

        stat = NETWORK_STAT_DISCONNECTED;
        memset(&addr, 0, sizeof(PrivateAddressData));
    }

    bool equals(PrivateAddressData& lhs, PrivateAddressData& rhs)
    {
#ifdef XPLICIT_WINDOWS
        return lhs.sin_addr.S_un.S_addr == rhs.sin_addr.S_un.S_addr;
#endif
    }
}