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

#define XPLICIT_MAX_RESET (1024)

namespace XPX
{
	static int XPLICIT_RESET_COUNT = 0;

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

		/* did we lost connection to peer? */
		if (mNetwork->is_reset())
		{
			ComponentSystem::get_singleton_ptr()->remove(mNetwork);

			if (ComponentSystem::get_singleton_ptr()->get<PopupComponent>("ResetPopup") == nullptr)
			{
				ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
					CAD->closeDevice();
				}, POPUP_TYPE::NETWORK, "ResetPopup");
			}

			return;
		}

		NetworkPacket packet;
		mNetwork->read(packet);

		if (packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
		{
			if (!ComponentSystem::get_singleton_ptr()->get<PopupComponent>("KickPopup"))
			{
				ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
					CAD->closeDevice();
					}, POPUP_TYPE::KICK, "KickPopup", packet.buffer[0] != 0 ? packet.buffer : "You have been kicked.");
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
			NetworkPacket packet{};
			packet = mNetwork->get();

			const auto players = ComponentSystem::get_singleton_ptr()->all_of<XPX::LocalHumanoidComponent>("LocalHumanoidComponent");

			for (std::size_t index = 0UL; index < players.size(); ++index)
			{
				if (players[index]->id() == packet.public_hash)
					return;
			}

#ifdef XPLICIT_DEBUG
			XPLICIT_INFO("World:Login [EVENT]");
#endif

			ComponentSystem::get_singleton_ptr()->add<XPX::LocalHumanoidComponent>(packet.public_hash, false, packet.buffer);
			Lua::CLuaStateManager::get_singleton_ptr()->run_string("World:Login()");

			/*! invalidate command right there. */
			packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] == NETWORK_CMD_INVALID;
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
						XPLICIT_INFO("World:Logoff [EVENT]");
#endif

						Lua::CLuaStateManager::get_singleton_ptr()->run_string("World:Logoff()");
						ComponentSystem::get_singleton_ptr()->remove(players[ply]);

						break;
					}
				}
			}
		}
	}

}