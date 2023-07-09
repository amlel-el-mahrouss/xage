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

#include <CLua/CLua.hpp>

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
			if (ComponentSystem::get_singleton_ptr()->get<PopupComponent>("ResetPopup") == nullptr)
			{
				ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
					if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(VK_RETURN))
						RENDER->closeDevice();
					}, Vector<float>(XPLICIT_DIM.X / 3.45,
						XPLICIT_DIM.Y / 4),
						POPUP_TYPE::NETWORK, "ResetPopup");
			}

			return;
		}

		NetworkPacket packet;
		if (!mNetwork->read(packet)) return;

		if (packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
		{
			if (!ComponentSystem::get_singleton_ptr()->get<PopupComponent>("KickPopup"))
			{
				ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
					if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(VK_RETURN))
						RENDER->closeDevice();
					}, Vector<float>(XPLICIT_DIM.X / 3.45,
						XPLICIT_DIM.Y / 4),
						POPUP_TYPE::KICK, "KickPopup");
			}
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_BAN] == NETWORK_CMD_BAN)
		{
			if (!ComponentSystem::get_singleton_ptr()->get<PopupComponent>("BanPopup"))
			{
				ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
					if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(VK_RETURN))
						RENDER->closeDevice();
					}, Vector<float>(XPLICIT_DIM.X / 3.45,
						XPLICIT_DIM.Y / 4),
						POPUP_TYPE::BANNED, "BanPopup");
			}
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] == NETWORK_CMD_SPAWN)
		{
			NetworkPacket packet{};
			packet = mNetwork->get();

			const auto players = ComponentSystem::get_singleton_ptr()->all_of<Xplicit::Player::LocalHumanoidComponent>("LocalHumanoidComponent");

			for (std::size_t index = 0UL; index < players.size(); ++index)
			{
				if (players[index]->id() == packet.public_hash)
					return;
			}

#ifdef XPLICIT_DEBUG
			XPLICIT_INFO("World:Login [EVENT]");
#endif

			ComponentSystem::get_singleton_ptr()->add<Xplicit::Player::LocalHumanoidComponent>(packet.public_hash);
			Lua::CLuaStateManager::get_singleton_ptr()->run_string("World:Login()");

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
				if (!ComponentSystem::get_singleton_ptr()->get<PopupComponent>("ConnShutdown"))
				{
					ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
						if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(VK_RETURN))
							RENDER->closeDevice();
					}, Vector<float>(XPLICIT_DIM.X / 2.8,
							XPLICIT_DIM.Y / 2.8),
							POPUP_TYPE::SHUTDOWN,
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