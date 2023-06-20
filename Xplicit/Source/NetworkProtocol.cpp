/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Protocol.cpp
 *			Purpose: Game Network Protocol implementation
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "NetworkProtocol.h"
#include "CommonEngine.h"

namespace Xplicit
{
    NetworkInstance::NetworkInstance() : 
        packet(), 
        unique_addr(), 
		hash(-1),
		public_hash(-1),
        taken(false),
        status(NETWORK_STAT_DISCONNECTED)
	{}

    NetworkInstance::~NetworkInstance() {}
    
    NetworkInstance::UniqueAddress::UniqueAddress()
		: mUuid(UUIDFactory::version<4>())
	{}

    NetworkInstance::UniqueAddress::~UniqueAddress() = default;

    void NetworkInstance::reset() noexcept
    {
        for (size_t cmd_index = 0; cmd_index < XPLICIT_NETWORK_CMD_MAX; ++cmd_index)
        {
            packet.cmd[cmd_index] = NETWORK_CMD_INVALID;
        }

        status = NETWORK_STAT_INVALID;

        hash = -1;
        public_hash = -1;
    }
}