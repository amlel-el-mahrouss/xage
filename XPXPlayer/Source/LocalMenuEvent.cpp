/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalMenuEvent.h"
#include "Application.h"

namespace XPX
{
	constexpr int XPLICIT_TIMEOUT_MENU = 2000;

	LocalMenuEvent::LocalMenuEvent() noexcept
		:
			mNetwork(nullptr),
			mTimeout(0UL),
			mEnabled(false),
			mPopup(nullptr)
	{
		/* resource loading */
		mNetwork = ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);
	}

	LocalMenuEvent::~LocalMenuEvent() = default;

	const char* LocalMenuEvent::name() noexcept { return ("LocalMenuEvent"); }

	void LocalMenuEvent::enable(const bool enable) noexcept { mEnabled = enable; }

	bool LocalMenuEvent::enabled() const noexcept { return mEnabled; }

	void LocalMenuEvent::operator()()
	{
		if (!mNetwork)
			return;

		static float pos_menu = 14;

		if (KEYBOARD->key_down(KEY_F9) &&
			mTimeout < 0 && 
			!mPopup)
		{
			mEnabled = true;
			mTimeout = XPLICIT_TIMEOUT_MENU;

			mPopup = ComponentSystem::get_singleton_ptr()->add<PopupComponent>(
				[&]() {
					NetworkPacket packet{};

					packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
					mNetwork->set_channel(XPLICIT_CHANNEL_DATA);

					mNetwork->send(packet);

					mEnabled = false;

					std::exit(0);
			}, POPUP_TYPE::LEAVE, "LeavePopup");
		}

		/* menu is being open */
		if (mEnabled)
		{
			if (KEYBOARD->key_down(KEY_ESCAPE))
			{
				ComponentSystem::get_singleton_ptr()->remove(mPopup);

				mPopup = nullptr;
				mEnabled = false;
			}
		}

		--mTimeout;
	}
}