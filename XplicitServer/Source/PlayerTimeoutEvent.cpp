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
		NetworkServerContext::accept_recv(mNetwork);

		for (std::size_t index = 0; index < mNetwork->size(); ++index)
		{
			if (mNetwork->get(index)->status == NETWORK_STAT_DISCONNECTED ||
				mNetwork->get(index)->status == NETWORK_STAT_INVALID)
				continue;

			mNetwork->get(index)->timeout();
		}

		NetworkServerContext::accept_send(mNetwork);
	}
}