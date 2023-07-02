/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Corporation, all rights reserved.
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

	LocalMenuEvent::LocalMenuEvent(Gwk::Controls::Canvas* pCanvas) noexcept
		:
			mNetwork(nullptr),
			mButtonNoHover(nullptr),
			mButtonHover(nullptr),
			mTimeout(0UL),
			mMenu(nullptr),
			mEnabled(false),
			mChatBox(new Gwk::Controls::TextBoxMultiline(pCanvas)),
			mInputChat(new Gwk::Controls::TextBox(pCanvas))
	{
		/* resource loading */
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);

		String frame_path = "network_leave.png";

		mMenu = RENDER->getVideoDriver()->getTexture(frame_path.c_str());

		frame_path.clear();
		frame_path += "menu_hover.png";

		mButtonHover = RENDER->getVideoDriver()->getTexture(frame_path.c_str());

		frame_path.clear();
		frame_path += "menu_no_hover.png";

		mButtonNoHover = RENDER->getVideoDriver()->getTexture(frame_path.c_str());

		if (pCanvas)
		{
			mInputChat->SetPos(10, 10);
			mInputChat->SetSize(300, 20);
			mInputChat->SetTextColor(Gwk::Color(255, 255, 255, 255));

			mChatBox->SetPos(10, 30);
			mChatBox->SetSize(300, 300);
			mChatBox->SetText("Press / to chat.");
			mChatBox->SetTextColor(Gwk::Color(255, 255, 255, 255));

			mChatBox->SetEditable(false);
		}
	}

	LocalMenuEvent::~LocalMenuEvent() 
	{
		if (mMenu)
			(void)mMenu->drop();

		if (mButtonHover)
			(void)mButtonHover->drop();

		if (mButtonNoHover)
			(void)mButtonNoHover->drop();
	
		if (mChatBox)
			delete mChatBox;

		if (mInputChat)
			delete mInputChat;
	}

	const char* LocalMenuEvent::name() noexcept { return ("LocalMenuEvent"); }

	bool LocalMenuEvent::in_chat() const noexcept
	{
		return mInputChat->IsFocussed();
	}

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
		static float posOfMenu = 12;

		if (KB->key_down(KEY_F3) &&
			mTimeout < 0)
		{
			tween_start = LOCAL_MENU_TWEEN_START;

			mEnabled = !mEnabled;
			mTimeout = XPLICIT_TIMEOUT_MENU;
		}

		/* menu is being open */
		if (mEnabled)
		{
			RENDER->getVideoDriver()->draw2DImage(mButtonHover,
				vector2di(30, XPLICIT_DIM.Y / posOfMenu),
				rect(0, 0, 63, 42),
				nullptr,
				SColor(255, 255, 255, 255),
				true);

			if (tween_start > LOCAL_MENU_TWEEN_END)
				tween_start -= LOCAL_MENU_TWEENING;

			if (KB->key_down(KEY_RETURN))
			{
				NetworkPacket packet{};

				packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
				mNetwork->set_channel(XPLICIT_CHANNEL_DATA);

				mNetwork->send(packet);
				
				mEnabled = false;

				std::exit(0);
			}
			else if (KB->key_down(KEY_ESCAPE))
			{
				mEnabled = false;
			}
		}
		/*
			menu is being closed/inactive
		*/
		else
		{
			RENDER->getVideoDriver()->draw2DImage(mButtonNoHover,
				vector2di(30, XPLICIT_DIM.Y / posOfMenu),
				rect(0, 0, 63, 42),
				nullptr,
				SColor(255, 255, 255, 255),
				true);

			if (tween_start < LOCAL_MENU_TWEEN_START)
				tween_start += LOCAL_MENU_TWEENING;
		}

		if (tween_start < LOCAL_MENU_TWEEN_START)
		{
			RENDER->getVideoDriver()->draw2DImage(mMenu, 
				vector2di(XPLICIT_DIM.X / 2.8,
				XPLICIT_DIM.Y / tween_start));
		}

		--mTimeout;
	}
}