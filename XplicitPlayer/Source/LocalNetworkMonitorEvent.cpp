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

#include "LocalNetworkMonitorEvent.h"

#include "Application.h"
#include "GameMenuUI.h"

namespace Xplicit::Player
{
	constexpr int XPLICIT_MAX_RESETS = 150; // Max resets allowed before connection drop

	LocalNetworkMonitorEvent::LocalNetworkMonitorEvent(const std::int64_t& priv, const std::int64_t& publ)
		: mNetwork(nullptr),
			mResetCount(0),
			mPublicHash(publ),
			mHash(priv)
	{
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);
	}

	LocalNetworkMonitorEvent::~LocalNetworkMonitorEvent() = default;

	const char* LocalNetworkMonitorEvent::name() noexcept { return ("LocalNetworkMonitorEvent"); }

	void LocalNetworkMonitorEvent::operator()()
	{
		if (!mNetwork) return;

		const NetworkPacket& packet = mNetwork->get();
		
		if (packet.cmd[XPLICIT_NETWORK_CMD_BAN] == NETWORK_CMD_BAN)
		{
			ComponentManager::get_singleton_ptr()->add<Player::PopupComponent>([]()-> void {
				IRR->closeDevice();
				}, vector2di(XPLICIT_DIM.Width / 3.45,
					XPLICIT_DIM.Height / 4),
					POPUP_TYPE::BANNED, "BanPopup");
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] == NETWORK_CMD_SPAWN)
		{
			NetworkPacket packet{};
			packet = mNetwork->get();

			const auto players = ComponentManager::get_singleton_ptr()->all_of<Xplicit::Player::LocalPlayerComponent>("LocalPlayerComponent");

			for (std::size_t index = 0UL; index < players.size(); ++index)
			{
				if (players[index]->id() == packet.public_hash)
					return;
			}

			ComponentManager::get_singleton_ptr()->add<Xplicit::Player::LocalPlayerComponent>(packet.public_hash);

			/*! invalidate command right there. */
			packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] == NETWORK_CMD_INVALID;
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_SHUTDOWN] == NETWORK_CMD_SHUTDOWN ||
			packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK ||
			packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP)
		{
			if (packet.hash == mHash ||
				packet.public_hash == mPublicHash)
			{
				if (!ComponentManager::get_singleton_ptr()->get<PopupComponent>("ConnShutdown"))
				{
					ComponentManager::get_singleton_ptr()->add<PopupComponent>([]()-> void {
						std::exit(0);
					}, vector2di(XPLICIT_DIM.Width / 2.8,
							XPLICIT_DIM.Height / 2.8),
							POPUP_TYPE::SHUTDOWN,
							"ConnShutdown");

					mNetwork = nullptr;
				}
			}
			else
			{
				const auto players = ComponentManager::get_singleton_ptr()->all_of<Xplicit::Player::LocalPlayerComponent>("LocalPlayerComponent");

				for (int ply = 0; ply < players.size(); ++ply)
				{
					if (packet.public_hash == players[ply]->id())
					{
						ComponentManager::get_singleton_ptr()->remove(players[ply]);
						break;
					}
				}
			}
		}

		/* did we lost connection to peer? */
		if (mNetwork->is_reset())
		{
			++mResetCount;

			if (mResetCount > XPLICIT_MAX_RESETS)
				std::exit(0);
		}
	}

}