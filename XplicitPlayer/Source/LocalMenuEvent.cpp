/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalMenuEvent.h"
#include "Application.h"

#define LOCAL_MENU_TWEEN_START	(8)
#define LOCAL_MENU_TWEEN_END	(2)
#define LOCAL_MENU_TWEENING		(0.1f)

namespace Xplicit::Player
{
	constexpr int XPLICIT_TIMEOUT_MENU = 2000;

	LocalMenuEvent::LocalMenuEvent() noexcept
		:
			mNetwork(nullptr),
			mTimeout(0UL),
			mEnabled(false)
	{
		/* resource loading */
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);

		String frame_path = "network_leave.png";


		frame_path.clear();
		frame_path += "menu_hover.png";


		frame_path.clear();
		frame_path += "menu_no_hover.png";

	}

	LocalMenuEvent::~LocalMenuEvent() 
	{

	}

	const char* LocalMenuEvent::name() noexcept { return ("LocalMenuEvent"); }

	void LocalMenuEvent::enable(const bool enable) noexcept { mEnabled = enable; }

	bool LocalMenuEvent::enabled() const noexcept { return mEnabled; }

	void LocalMenuEvent::operator()()
	{
		if (!mNetwork)
			return;
		
		/* F3 key down. */
		if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(VK_F3) &&
			mTimeout < 0)
		{
			mEnabled = true;
		}

		/* The menu is open. */
		if (mEnabled)
		{
			if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(VK_RETURN))
			{
				NetworkPacket packet{};

				packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
				mNetwork->set_channel(XPLICIT_CHANNEL_DATA);

				mNetwork->send(packet);
				
				mEnabled = false;

				std::exit(0);
			}
			else if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(VK_ESCAPE))
			{
				mEnabled = false;
			}
		}

		--mTimeout;
	}
}