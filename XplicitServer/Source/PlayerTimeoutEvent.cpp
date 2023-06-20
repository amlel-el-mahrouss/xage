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
		:
		mNetwork(ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
		mCounter(0UL)
	{}

	PlayerTimeoutEvent::~PlayerTimeoutEvent() = default;

	const char* PlayerTimeoutEvent::name() noexcept { return "PlayerTimeoutEvent"; }
	
	void PlayerTimeoutEvent::operator()()
	{
		if (mCounter < PlayerTimeoutEvent::cycles)
		{
			++mCounter;
		}
		else
		{
			mCounter = 0UL;

			for (std::size_t index = 0; index < mNetwork->size(); ++index)
			{
				if (mNetwork->get(index)->stat == NETWORK_STAT_DISCONNECTED)
					continue;

				if (mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] != NETWORK_CMD_ACK)
				{
					mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
				}
				else
				{
					mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_INVALID;
				}
			}
		}
	}
}