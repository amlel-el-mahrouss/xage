/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: LocalResetEvent.cpp
 *			Purpose: RST timeout event.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalNetworkMonitorEvent.h"
#include "Application.h"
#include "CoreUI.h"

namespace Xplicit::Player
{
	constexpr const int XPLICIT_MAX_RESETS = 10000; // Max RST

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
		if (!mNetwork)
			return;

		auto& packet = mNetwork->get();

		if (packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] == NETWORK_CMD_SPAWN)
		{
			ComponentManager::get_singleton_ptr()->add<Xplicit::Player::LocalPlayerComponent>(packet.public_hash);
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP)
		{
			if (mHash == packet.hash)
			{
				if (!ComponentManager::get_singleton_ptr()->get<CoreUI::Popup>("StopPopup"))
				{
					/* connection closed by peer. */
					ComponentManager::get_singleton_ptr()->add<CoreUI::Popup>([]()-> void {
						// close on click
						IRR->closeDevice();
						}, vector2di(Xplicit::Player::XPLICIT_DIM.Width / 2.8,
							Xplicit::Player::XPLICIT_DIM.Height / 4),
							CoreUI::POPUP_TYPE::SHUTDOWN,
							"ResetConnPopup");

					return;
				}
			}
			
			auto plyers = ComponentManager::get_singleton_ptr()->all_of<Xplicit::Player::LocalPlayerComponent>("LocalPlayerComponent");

			for (int ply = 0; ply < plyers.size(); ++ply)
			{
				if (packet.public_hash == plyers[ply]->id())
				{
					ComponentManager::get_singleton_ptr()->remove(plyers[ply]);
				}
			}
		}

		/* lost connection to peer. */
		if (mNetwork->is_reset())
		{
			++mResetCnt;

			if (mResetCnt > XPLICIT_MAX_RESETS)
			{
				if (!ComponentManager::get_singleton_ptr()->get<CoreUI::Popup>("ResetPopup"))
				{
					ComponentManager::get_singleton_ptr()->add<CoreUI::Popup>([]()-> void {
						IRR->closeDevice();
						}, vector2di(Xplicit::Player::XPLICIT_DIM.Width / 3.45, Xplicit::Player::XPLICIT_DIM.Height / 4), CoreUI::POPUP_TYPE::NETWORK_ERROR, "ResetPopup");

				}
			}
		}
	}

}