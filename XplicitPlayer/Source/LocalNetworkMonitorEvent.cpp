/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: LocalNetworkMonitorEvent.cpp
 *			Purpose: Network Monitor event.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalNetworkMonitorEvent.h"
#include "Application.h"
#include "GameMenuUI.h"

#include <CommonEngine.h>

namespace Xplicit::Player
{
	constexpr const int XPLICIT_MAX_RESETS = 150; // Max resets allowed before connection drop

	LocalNetworkMonitorEvent::LocalNetworkMonitorEvent(int64_t hash)
		: mNetwork(nullptr), mResetCnt(0), mHash(hash)
	{
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);
	}

	LocalNetworkMonitorEvent::~LocalNetworkMonitorEvent() {}

	const char* LocalNetworkMonitorEvent::name() noexcept { return ("LocalResetEvent"); }

	void LocalNetworkMonitorEvent::operator()()
	{
		if (!mNetwork) return;

		NetworkPacket& packet = mNetwork->get();

		/* try send the acknowledge packet for the server. */
		packet.cmd[XPLICIT_NETWORK_CMD_ACK] = XPLICIT_XCONNECT_WATCHDOG_BYTE;

		mNetwork->send(packet);

		if (packet.cmd[XPLICIT_NETWORK_CMD_BAN] == NETWORK_CMD_BAN)
		{
			ComponentManager::get_singleton_ptr()->add<Player::PopupComponent>([]()-> void {
				IRR->closeDevice();
				}, vector2di(Xplicit::Player::XPLICIT_DIM.Width / 3.45,
					Xplicit::Player::XPLICIT_DIM.Height / 4),
					Player::POPUP_TYPE::BANNED, "BanPopup");
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
		{
			ComponentManager::get_singleton_ptr()->add<Player::PopupComponent>([]()-> void {
				IRR->closeDevice();
				}, vector2di(Xplicit::Player::XPLICIT_DIM.Width / 3.45,
					Xplicit::Player::XPLICIT_DIM.Height / 4),
					Player::POPUP_TYPE::KICK, "KickPopup");
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] == NETWORK_CMD_SPAWN)
		{
			static NetworkPacket packet{};
			packet = mNetwork->get();

			auto plyers = ComponentManager::get_singleton_ptr()->all_of<Xplicit::Player::LocalPlayerComponent>("LocalPlayerComponent");

			for (std::size_t index = 0UL; index < plyers.size(); ++index)
			{
				if (plyers[index]->id() == packet.public_hash)
					return;
			}

			ComponentManager::get_singleton_ptr()->add<Xplicit::Player::LocalPlayerComponent>(packet.public_hash);

			/*! invalidate command right there. */
			packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] == NETWORK_CMD_INVALID;
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_SHUTDOWN] == NETWORK_CMD_SHUTDOWN)
		{
			if (packet.hash == mHash)
			{
				if (!ComponentManager::get_singleton_ptr()->get<Player::PopupComponent>("ConnShutdown"))
				{
					ComponentManager::get_singleton_ptr()->add<Player::PopupComponent>([]()-> void {
						std::exit(0);
					}, vector2di(Xplicit::Player::XPLICIT_DIM.Width / 2.8,
							Xplicit::Player::XPLICIT_DIM.Height / 2.8),
							Player::POPUP_TYPE::SHUTDOWN,
							"ConnShutdown");

					return;
				}
			}
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP)
		{
			auto plyers = ComponentManager::get_singleton_ptr()->all_of<Xplicit::Player::LocalPlayerComponent>("LocalPlayerComponent");

			for (int ply = 0; ply < plyers.size(); ++ply)
			{
				if (packet.public_hash == plyers[ply]->id())
				{
					ComponentManager::get_singleton_ptr()->remove(plyers[ply]);
					break;
				}
			}
		}

		/* did we lost connection to peer? */
		if (mNetwork->is_reset())
		{
			++mResetCnt;

			/* count down */
			if (mResetCnt > XPLICIT_MAX_RESETS)
			{
				if (!ComponentManager::get_singleton_ptr()->get<Player::PopupComponent>("ResetPopup"))
				{
					ComponentManager::get_singleton_ptr()->add<Player::PopupComponent>([]()-> void {
						std::exit(0);
					},
						vector2di(Xplicit::Player::XPLICIT_DIM.Width / 2.8,
							Xplicit::Player::XPLICIT_DIM.Height / 2.8),
						Player::POPUP_TYPE::SHUTDOWN, "ResetPopup");

				}
			}
		}
	}

}