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
    NetworkInstance::NetworkInstance()
	: 
        packet(), 
		hash(XPLICIT_INVALID_NET_DATA),
		public_hash(-1),
        address(),
        status(NETWORK_STAT_DISCONNECTED)
	{}

    NetworkInstance::~NetworkInstance() = default;
    
    NetworkInstance::UniqueAddress::UniqueAddress()
		: mUuid(UUIDFactory::version<4>())
	{}

    NetworkInstance::UniqueAddress::~UniqueAddress() = default;

    bool equals(PrivateAddressData& lhs, PrivateAddressData& rhs)
    {
        return lhs.sin_addr.S_un.S_addr == rhs.sin_addr.S_un.S_addr;
    }
}