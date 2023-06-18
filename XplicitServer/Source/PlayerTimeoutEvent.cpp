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

#include "PlayerTimeoutEvent.h"

namespace Xplicit
{
	PlayerTimeoutEvent::PlayerTimeoutEvent()
		: mNetwork(ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent"))
	{}

	PlayerTimeoutEvent::~PlayerTimeoutEvent() = default;

	const char* PlayerTimeoutEvent::name() noexcept { return "PlayerTimeoutEvent"; }
	
	void PlayerTimeoutEvent::operator()()
	{
		for (std::size_t index = 0; index < mNetwork->size(); ++index)
		{
			if (mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] != NETWORK_CMD_ACK)
				mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
		}
	}
}