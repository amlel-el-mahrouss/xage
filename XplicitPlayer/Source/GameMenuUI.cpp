/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: CoreUI.cpp
 *			Purpose: Client UI
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "GameMenuUI.h"

/* Application framework. */
#include "Application.h"

namespace Xplicit::Player
{
	/*
	 * This is a popup class
	 * used to create pop-up according to a state.
	 */

	// constructor
	PopupComponent::PopupComponent(const std::function<void()>& onClick, const vector2di pos, const POPUP_TYPE popupType, const char* id) noexcept
		: mClicked(onClick), m_vecPos(pos), m_strPopupId(id)
	{
		XPLICIT_ASSERT(onClick);
		XPLICIT_ASSERT(!m_strPopupId.empty());

		String path;

		switch (popupType)
		{
		case POPUP_TYPE::KICK:
			path += "Kicked.png";
			break;
		case POPUP_TYPE::NETWORK_ERROR:
			path += "NetworkError.png";
			break;
		case POPUP_TYPE::SHUTDOWN:
			path += "Shutdown.png";
			break;
		case POPUP_TYPE::WAITING_ACK:
			path += "Download.png";
			break;
		}

		m_pTexture = IRR->getVideoDriver()->getTexture(path.c_str());

		XPLICIT_ASSERT(m_pTexture);

		if (!m_pTexture)
			throw EngineError();
	}

	// c++ destructor
	PopupComponent::~PopupComponent()
	{
		if (m_pTexture)
			m_pTexture->drop();
	}

	// update function
	void PopupComponent::update()
	{
		IRR->getVideoDriver()->draw2DImage(m_pTexture, m_vecPos);

		if (KB->key_down(KEY_END))
		{
			mClicked();
		}
	}

	const char* PopupComponent::name() noexcept
	{
		return m_strPopupId.c_str();
	}

	PopupComponent::INSTANCE_TYPE PopupComponent::type() noexcept
	{
		return PopupComponent::INSTANCE_GUI;
	}
	
	/* Heads up display */
	HUDComponent::HUDComponent(const std::int64_t& publicHash)
		: mHealth(0),
		  mPublicHash(publicHash),
		  mNetwork(nullptr)
	{
		XPLICIT_ASSERT(publicHash != -1);

		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);
	}

	HUDComponent::~HUDComponent() = default;

	void HUDComponent::update()
	{
		if (!mNetwork)
			return;

		auto packet = mNetwork->get();
	
		if (packet.cmd[XPLICIT_NETWORK_CMD_DAMAGE] == NETWORK_CMD_DAMAGE)
			mHealth = packet.health;
	}

	NotificationComponent::NotificationComponent(const char* caption, const char* text)
		:
		mName(caption),
		mTexture(IRR->getVideoDriver()->getTexture("Notification.png")),
		mStart(true),
		mCountdown(3000)
	{
		XPLICIT_GET_DATA_DIR(dir);

		String path = dir;
		path += "\\Urbanist.xml";

		mFnt = IRR->getGUIEnvironment()->getFont(path.c_str());
	}

	NotificationComponent::~NotificationComponent()
	{
		if (mTexture)
			mTexture->drop();
	}

	void NotificationComponent::update()
	{
		static float tweenStart = 8;

		if (mStart)
		{
			if (tweenStart > 1.5)
				tweenStart -= 0.01f;
		}
		else
		{
			if (tweenStart > 1.5)
				tweenStart += 0.01f;

			if (tweenStart >= 8)
			{
				ComponentManager::get_singleton_ptr()->remove(this);
				return;
			}
		}

		static const float final_pos = 2.8;

		if (tweenStart < 8)
		{
			IRR->getVideoDriver()->draw2DImage(mTexture, vector2di(
				Xplicit::Player::XPLICIT_DIM.Width / tweenStart,
				Xplicit::Player::XPLICIT_DIM.Height / 1.5)
			);
		}

		if (tweenStart == final_pos)
		{
			mFnt->draw("Test Notification", recti(vector2di(
				Xplicit::Player::XPLICIT_DIM.Width / tweenStart,
				Xplicit::Player::XPLICIT_DIM.Height / 1.5), dimension2di(0, 0)), SColor(0xFFB800), true, true);
		}

		--mCountdown;

		if (mCountdown < 1)
			this->dispatch();
	}

	void NotificationComponent::dispatch() noexcept { mStart = false; }


	const char* NotificationComponent::name() noexcept { XPLICIT_ASSERT(!mName.empty()); return mName.c_str(); }

	NotificationComponent::INSTANCE_TYPE NotificationComponent::type() noexcept { return INSTANCE_GUI; }
}