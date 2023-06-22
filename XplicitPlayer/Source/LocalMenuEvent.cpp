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
			mEnabled(false)
	{
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);

		XPLICIT_GET_DATA_DIR(data_dir);
		String frame_path = data_dir;
		frame_path += "\\menu_overlay.png";

		mMenu = IRR->getVideoDriver()->getTexture(frame_path.c_str());

		frame_path.clear();

		frame_path = data_dir;
		frame_path += "\\menu_hover.png";

		mButtonHover = IRR->getVideoDriver()->getTexture(frame_path.c_str());

		frame_path.clear();

		frame_path = data_dir;
		frame_path += "\\menu_no_hover.png";

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
		
		static float tween_start = LOCAL_MENU_TWEEN_START;
		static float posOfMenu = 1;

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
				rect<s32>(0, 0, 63, 42),
				nullptr,
				SColor(255, 255, 255, 255),
				true);

			if (tween_start > LOCAL_MENU_TWEEN_END)
				tween_start -= LOCAL_MENU_TWEENING;

			if (KB->key_down(KEY_KEY_Y))
			{
				mEnabled = false;
				IRR->closeDevice();
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
				rect<s32>(0, 0, 63, 42),
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