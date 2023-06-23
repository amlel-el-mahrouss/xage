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

#include "LocalMenuEvent.h"
#include "Application.h"

#define LOCAL_MENU_TWEEN_START (8)
#define LOCAL_MENU_TWEEN_END (2)
#define LOCAL_MENU_TWEENING (0.1f)

namespace Xplicit::Player
{
	constexpr int XPLICIT_TIMEOUT_MENU = 2000;

	LocalMenuEvent::LocalMenuEvent(const std::int64_t& hash)
		:
			mNetwork(nullptr),
			mButtonNoHover(nullptr),
			mButtonHover(nullptr),
			mHash(hash),
			mTimeout(0UL),
			mMenu(nullptr),
			mShutdown(false),
			mEnabled(false)
	{
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);

		String frame_path = "menu_overlay.png";

		mMenu = IRR->getVideoDriver()->getTexture(frame_path.c_str());

		frame_path.clear();

		frame_path += "menu_hover.png";

		mButtonHover = IRR->getVideoDriver()->getTexture(frame_path.c_str());

		frame_path.clear();

		frame_path += "menu_no_hover.png";

		mButtonNoHover = IRR->getVideoDriver()->getTexture(frame_path.c_str());
	}

	LocalMenuEvent::~LocalMenuEvent() 
	{
		if (mMenu)
			(void)mMenu->drop();

		if (mButtonHover)
			(void)mButtonHover->drop();

		if (mButtonNoHover)
			(void)mButtonNoHover->drop();
	}

	const char* LocalMenuEvent::name() noexcept { return ("LocalMenuEvent"); }

	void LocalMenuEvent::enable(const bool enable) noexcept
	{
		mEnabled = enable;
	}

	bool LocalMenuEvent::enabled() const noexcept { return mEnabled; }

	void LocalMenuEvent::operator()()
	{
		if (!mNetwork)
			return;

		if (mShutdown)
		{
			NetworkPacket packet;
			mNetwork->read(packet);

			if (packet.cmd[XPLICIT_NETWORK_CMD_ACK] == NETWORK_CMD_ACK)
				std::exit(0);

			return;
		}
		
		static float tween_start = LOCAL_MENU_TWEEN_START;
		static float posOfMenu = 1.5;

		if (KB->key_down(KEY_ESCAPE) && 
			mTimeout < 0)
		{
			tween_start = LOCAL_MENU_TWEEN_START;

			mEnabled = !mEnabled;
			mTimeout = XPLICIT_TIMEOUT_MENU;
		}

		/*
			menu is being open
		*/

		if (mEnabled)
		{
			IRR->getVideoDriver()->draw2DImage(mButtonHover,
				vector2di(30, XPLICIT_DIM.Height / posOfMenu),
				rect(0, 0, 63, 42),
				nullptr,
				SColor(255, 255, 255, 255),
				true);

			if (tween_start > LOCAL_MENU_TWEEN_END)
				tween_start -= LOCAL_MENU_TWEENING;

			if (KB->key_down(KEY_KEY_Y))
			{
				NetworkPacket packet;
				packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;

				mNetwork->set_channel(XPLICIT_CHANNEL_DATA);
				mNetwork->send(packet);

				mShutdown = true;
				mEnabled = false;
			}
			else if (KB->key_down(KEY_KEY_N))
			{
				mEnabled = false;
			}
		}
		/*
			menu is being closed/inactive
		*/
		else
		{
			IRR->getVideoDriver()->draw2DImage(mButtonNoHover,
				vector2di(30, XPLICIT_DIM.Height / posOfMenu),
				rect(0, 0, 63, 42),
				nullptr,
				SColor(255, 255, 255, 255),
				true);

			if (tween_start < LOCAL_MENU_TWEEN_START)
				tween_start += LOCAL_MENU_TWEENING;
		}

		if (tween_start < LOCAL_MENU_TWEEN_START)
		{
			IRR->getVideoDriver()->draw2DImage(mMenu, 
				vector2di(XPLICIT_DIM.Width / 2.8,
				XPLICIT_DIM.Height / tween_start));
		}

		--mTimeout;
	}
}