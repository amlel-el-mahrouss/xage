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
#include "LocalCharacterComponent.h"
#include "GameUI.h"
#include "App.h"

constexpr short XPX_MAX_TIMEOUT_MACRO = 250;

namespace XPX
{
	static short XPX_MAX_TIMEOUT = XPX_MAX_TIMEOUT_MACRO;

	LocalNetworkMonitorEvent::LocalNetworkMonitorEvent(const std::int64_t& priv, 
														const std::int64_t& publ)
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
				if (ComponentSystem::get_singleton_ptr()->add<MessageComponent>([]()-> void {
					std::exit(255);
					}, POPUP_TYPE::NETWORK, "ResetPopup"))
				{
					ComponentSystem::get_singleton_ptr()->remove(mNetwork);
					mNetwork = nullptr;

					return;
				}
			}
		}
		else
		{
			XPX_MAX_TIMEOUT = XPX_MAX_TIMEOUT_MACRO;
		}

		NetworkPacket packet = mNetwork->get();

		if (packet.channel != XPLICIT_CHANNEL_DATA)
			return;

		if (packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
		{
			if (packet.hash == mHash)
			{
				if (!ComponentSystem::get_singleton_ptr()->get<MessageComponent>("KickPopup"))
				{
					if (ComponentSystem::get_singleton_ptr()->add<MessageComponent>([]()-> void {
						std::exit(255);
						}, POPUP_TYPE::KICK, 
						"KickPopup", 
						packet.additional_data[0] != 0 ? packet.additional_data : "You have been kicked from the lobby."))
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
			if (!ComponentSystem::get_singleton_ptr()->get<MessageComponent>("BanPopup"))
			{
				ComponentSystem::get_singleton_ptr()->add<MessageComponent>([]()-> void {
					std::exit(60);
					}, POPUP_TYPE::BANNED, "BanPopup");
			}
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_SHUTDOWN] == NETWORK_CMD_SHUTDOWN ||
			packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP)
		{
			if (packet.hash == mHash ||
				packet.public_hash == mPublicHash)
			{
				if (!ComponentSystem::get_singleton_ptr()->get<MessageComponent>("ConnShutdown"))
				{
					ComponentSystem::get_singleton_ptr()->add<MessageComponent>([]()-> void {
						std::exit(128);
					}, POPUP_TYPE::SHUTDOWN,
							"ConnShutdown");

				}

				mNetwork = nullptr;

				return;
			}
		}
	}

}