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
				if (mNetwork->get(index)->status == NETWORK_STAT_DISCONNECTED ||
					mNetwork->get(index)->status == NETWORK_STAT_INVALID)
					continue;

				auto& packet = mNetwork->get(index)->packet;

				mNetwork->get(index)->done = false;

				if (packet.cmd[XPLICIT_NETWORK_CMD_ACK] != NETWORK_CMD_ACK)
				{
					packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
				}
				else
				{
					packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_INVALID;
				}

				mNetwork->get(index)->done = true;
			}
		}
	}
}