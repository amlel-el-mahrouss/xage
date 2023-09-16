/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalNetworkMonitorEvent.h"
#include "LocalHumanoidComponent.h"
#include "Application.h"
#include "MenuUI.h"

#include <CLua.hpp>

namespace XPX
{
	static int XPX_MAX_TIMEOUT = 250;

	LocalNetworkMonitorEvent::LocalNetworkMonitorEvent(const std::int64_t& priv, const std::int64_t& publ)
		:
			mNetwork(nullptr),
			mResetCount(0),
			mPublicHash(publ),
			mHash(priv)
	{
		mNetwork = ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");

		// just in case.
		XPLICIT_ASSERT(mNetwork);
	}

	LocalNetworkMonitorEvent::~LocalNetworkMonitorEvent() = default;

	const char* LocalNetworkMonitorEvent::name() noexcept { return ("LocalNetworkMonitorEvent"); }

	void LocalNetworkMonitorEvent::operator()()
	{
		if (!mNetwork) return;

		if (mNetwork->is_reset())
		{
			--XPX_MAX_TIMEOUT;

			if (XPX_MAX_TIMEOUT < 1)
			{
				if (ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
					CAD->closeDevice();
					}, POPUP_TYPE::NETWORK, "ResetPopup"))
				{
					ComponentSystem::get_singleton_ptr()->remove(mNetwork);
					mNetwork = nullptr;

					return;
				}
			}
		}

		NetworkPacket packet = mNetwork->get();

		if (packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
		{
			if (packet.hash == mHash)
			{
				if (!ComponentSystem::get_singleton_ptr()->get<PopupComponent>("KickPopup"))
				{
					if (ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
						CAD->closeDevice();
						}, POPUP_TYPE::KICK, "KickPopup", packet.additional_data[0] != 0 ? packet.additional_data : "You have been kicked."))
					{
						ComponentSystem::get_singleton_ptr()->remove(mNetwork);
						mNetwork = nullptr;

						return;
					}
				}
			}
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_BAN] == NETWORK_CMD_BAN)
		{
			if (!ComponentSystem::get_singleton_ptr()->get<PopupComponent>("BanPopup"))
			{
				ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
					CAD->closeDevice();
					}, POPUP_TYPE::BANNED, "BanPopup");
			}
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] == NETWORK_CMD_SPAWN)
		{
			NetworkPacket networkPacket{};
            networkPacket = mNetwork->get();

			const auto players = ComponentSystem::get_singleton_ptr()->all_of<XPX::LocalHumanoidComponent>("LocalHumanoidComponent");

			for (auto player : players)
			{
				if (player->id() == networkPacket.public_hash)
					return;
			}

#ifdef XPLICIT_DEBUG
			XPLICIT_INFO("world:Login [EVENT]");
#endif

			ComponentSystem::get_singleton_ptr()->add<XPX::LocalHumanoidComponent>(networkPacket.public_hash, false, networkPacket.additional_data);
			Lua::CLuaStateManager::get_singleton_ptr()->run_string("world:Login()");

			/*! invalidate command right there. */
			networkPacket.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_INVALID;
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_SHUTDOWN] == NETWORK_CMD_SHUTDOWN ||
			packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP)
		{
			if (packet.hash == mHash ||
				packet.public_hash == mPublicHash)
			{
				if (!ComponentSystem::get_singleton_ptr()->get<PopupComponent>("ConnShutdown"))
				{
					ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
						CAD->closeDevice();
					}, POPUP_TYPE::SHUTDOWN,
							"ConnShutdown");

				}

				mNetwork = nullptr;
			}
			else
			{
				const auto players = ComponentSystem::get_singleton_ptr()->all_of<LocalHumanoidComponent>("LocalHumanoidComponent");

				for (int ply = 0; ply < players.size(); ++ply)
				{
					if (packet.public_hash == players[ply]->id())
					{
#ifdef XPLICIT_DEBUG
						XPLICIT_INFO("world:Logoff [EVENT]");
#endif

						Lua::CLuaStateManager::get_singleton_ptr()->run_string("world:Logoff()");
						ComponentSystem::get_singleton_ptr()->remove(players[ply]);

						break;
					}
				}
			}
		}
	}

}