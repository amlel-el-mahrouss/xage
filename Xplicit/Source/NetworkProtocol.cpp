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
}