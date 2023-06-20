/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: LocalMenuEvent.cpp
 *			Purpose: Client menu
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
	LocalMenuEvent::LocalMenuEvent(const int64_t& hash)
		: mNetwork(nullptr), mEnabled(false), mMenu(nullptr), mTimeout(0), mHash(hash)
	{
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);

		XPLICIT_GET_DATA_DIR(data_dir);
		std::string frame_path = data_dir;
		frame_path += "\\Textures\\Leave.png";

		mMenu = IRR->getVideoDriver()->getTexture(frame_path.c_str());
	}

	LocalMenuEvent::~LocalMenuEvent() 
	{
		if (mMenu)
			mMenu->drop();
	}

	const char* LocalMenuEvent::name() noexcept { return ("LocalMenuEvent"); }

	void LocalMenuEvent::enable(const bool enable) noexcept
	{
		mEnabled = enable;
	}

	bool LocalMenuEvent::enabled() noexcept { return mEnabled; }

	static const int XPLICIT_TIMEOUT_MENU = 2000;

	void LocalMenuEvent::operator()()
	{
		if (!mNetwork)
			return;

		static float tweenStart = LOCAL_MENU_TWEEN_START;

		if (KB->key_down(KEY_ESCAPE) && mTimeout < 0)
		{
			tweenStart = LOCAL_MENU_TWEEN_START;

			mEnabled = !mEnabled;
			mTimeout = XPLICIT_TIMEOUT_MENU;
		}

		if (mEnabled)
		{
			if (tweenStart > LOCAL_MENU_TWEEN_END)
				tweenStart -= LOCAL_MENU_TWEENING;

			if (KB->key_down(KEY_KEY_Y))
			{
				NetworkPacket stopPacket{};

				stopPacket.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
				stopPacket.hash = mHash;

				stopPacket.size = sizeof(NetworkPacket);

				mNetwork->send(stopPacket);
				
				mEnabled = false;

				IRR->closeDevice();
			}
			else if (KB->key_down(KEY_KEY_N))
			{
				mEnabled = false;
			}
		}
		else
		{
			if (tweenStart < LOCAL_MENU_TWEEN_START)
				tweenStart += LOCAL_MENU_TWEENING;
		}

		if (tweenStart < LOCAL_MENU_TWEEN_START)
		{
			IRR->getVideoDriver()->draw2DImage(mMenu, 
				vector2di(Xplicit::Player::XPLICIT_DIM.Width / 2.8,
				Xplicit::Player::XPLICIT_DIM.Height / tweenStart));
		}

		--mTimeout;
	}
}