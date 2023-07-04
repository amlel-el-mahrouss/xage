/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalNetworkMonitorEvent.h"
#include "LocalHumanoidComponent.h"
#include "Application.h"
#include "GameMenuUI.h"

#include <lua/lua.hpp>

namespace Xplicit::Player
{
	constexpr int XPLICIT_MAX_RESETS = 150; // Max resets allowed before connection drop

	LocalNetworkMonitorEvent::LocalNetworkMonitorEvent(const std::int64_t& priv, const std::int64_t& publ)
		:
			mNetwork(nullptr),
			mResetCount(0),
			mPublicHash(publ),
			mHash(priv)
	{
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");

		// just in case.
		XPLICIT_ASSERT(mNetwork);
	}

	LocalNetworkMonitorEvent::~LocalNetworkMonitorEvent() = default;

	const char* LocalNetworkMonitorEvent::name() noexcept { return ("LocalNetworkMonitorEvent"); }

	void LocalNetworkMonitorEvent::operator()()
	{
		if (!mNetwork) return;

		/* did we lost connection to peer? */
		if (mNetwork->is_reset())
		{
			ComponentManager::get_singleton_ptr()->add<PopupComponent>([]()-> void {
				if (KB->key_down())
					RENDER->closeDevice();
				}, vector2di(XPLICIT_DIM.X / 3.45,
					XPLICIT_DIM.Y / 4),
					POPUP_TYPE::NETWORK, "BanPopup");

			return;
		}

		NetworkPacket packet;
		if (!mNetwork->read(packet)) return;

		if (packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
		{
			if (!ComponentManager::get_singleton_ptr()->get<PopupComponent>("KickPopup"))
			{
				ComponentManager::get_singleton_ptr()->add<PopupComponent>([]()-> void {
					if (KB->key_down())
						RENDER->closeDevice();
					}, vector2di(XPLICIT_DIM.X / 3.45,
						XPLICIT_DIM.Y / 4),
						POPUP_TYPE::KICK, "KickPopup");
			}
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_BAN] == NETWORK_CMD_BAN)
		{
			if (!ComponentManager::get_singleton_ptr()->get<PopupComponent>("BanPopup"))
			{
				ComponentManager::get_singleton_ptr()->add<PopupComponent>([]()-> void {
					if (KB->key_down())
						RENDER->closeDevice();
					}, vector2di(XPLICIT_DIM.X / 3.45,
						XPLICIT_DIM.Y / 4),
						POPUP_TYPE::BANNED, "BanPopup");
			}
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] == NETWORK_CMD_SPAWN)
		{
			NetworkPacket packet{};
			packet = mNetwork->get();

			const auto players = ComponentManager::get_singleton_ptr()->all_of<Xplicit::Player::LocalHumanoidComponent>("LocalHumanoidComponent");

			for (std::size_t index = 0UL; index < players.size(); ++index)
			{
				if (players[index]->id() == packet.public_hash)
					return;
			}

			XPLICIT_INFO("LocalHumanoid:Join [EVENT]");

			ComponentManager::get_singleton_ptr()->add<Xplicit::Player::LocalHumanoidComponent>(packet.public_hash);
			Lua::XLuaStateManager::get_singleton_ptr()->run_string("Engine:Join()");

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
						if (KB->key_down())
							RENDER->closeDevice();
					}, vector2di(XPLICIT_DIM.X / 2.8,
							XPLICIT_DIM.Y / 2.8),
							POPUP_TYPE::SHUTDOWN,
							"ConnShutdown");

				}

				mNetwork = nullptr;
			}
			else
			{
				const auto players = ComponentManager::get_singleton_ptr()->all_of<LocalHumanoidComponent>("LocalHumanoidComponent");

				for (int ply = 0; ply < players.size(); ++ply)
				{
					if (packet.public_hash == players[ply]->id())
					{
						XPLICIT_INFO("LocalHumanoid:Leave [EVENT]");
						Lua::XLuaStateManager::get_singleton_ptr()->run_string("Engine:Leave()");

						ComponentManager::get_singleton_ptr()->remove(players[ply]);
						break;
					}
				}
			}
		}
	}

}