/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 
	@file PlayerLeaveEvent.cpp

 */

#include "PlayerLeaveEvent.h"

namespace Xplicit
{
	PlayerLeaveEvent::PlayerLeaveEvent() 
		:
		mNetwork(ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
		mPlayerCount(0)
	{
		for (std::size_t index = 0UL; index < XPLICIT_MAX_CONNECTIONS; ++index)
		{
			PlayerComponent* component = ComponentManager::get_singleton_ptr()->add<PlayerComponent>();
			XPLICIT_ASSERT(component);

			mPlayers.push_back(component);
		}
	}

	PlayerLeaveEvent::~PlayerLeaveEvent() = default;

	void PlayerLeaveEvent::operator()()
	{
		String addr = "";

		for (size_t peer_idx = 0; peer_idx < mNetwork->size(); ++peer_idx)
		{
			if (mNetwork->get(peer_idx)->status == NETWORK_STAT_DISCONNECTED ||
				mNetwork->get(peer_idx)->status == NETWORK_STAT_INVALID)
				continue;

			addr = inet_ntoa(mNetwork->get(peer_idx)->address.sin_addr);

			if (addr == mNetwork->get(peer_idx)->str_address)
			{
				if (mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP ||
					mNetwork->get(peer_idx)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
				{
					if (mNetwork->get(peer_idx)->packet.hash == mNetwork->get(peer_idx)->hash)
					{
#ifdef XPLICIT_DEBUG

						XPLICIT_INFO("[DISCONNECT] IP: " + mNetwork->get(peer_idx)->str_address);

#endif // XPLICIT_DEBUG

						const auto public_hash = mNetwork->get(peer_idx)->public_hash;

						mNetwork->get(peer_idx)->unique_addr.invalidate();
						mNetwork->get(peer_idx)->reset();

						for (std::size_t index = 0; index < mNetwork->size(); ++index)
						{
							if (mNetwork->get(index)->status == NETWORK_STAT_CONNECTED)
							{
								mNetwork->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
								mNetwork->get(index)->packet.public_hash = public_hash;
							}
						}

						--mPlayerCount;
					}
				}
			}
		}
	}

	const size_t& PlayerLeaveEvent::size() const noexcept { return mPlayerCount; }

	const char* PlayerLeaveEvent::name() noexcept { return ("PlayerLeaveEvent"); }
}