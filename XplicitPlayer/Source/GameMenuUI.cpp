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
#include "Application.h"

namespace Xplicit::Player
{
	/*
	 * This is a popup class
	 * used to create pop-up according to a state.
	 */

	// constructor
	PopupComponent::PopupComponent(const std::function<void()>& onClick, const vector2di pos, const POPUP_TYPE popupType, const char* id) noexcept
		: mClicked(onClick), mPos(pos), mPopupId(id)
	{
		XPLICIT_ASSERT(onClick);
		XPLICIT_ASSERT(!mPopupId.empty());

		String path;

		switch (popupType)
		{
		case POPUP_TYPE::KICK:
			path += "Kicked.png";
			break;
		case POPUP_TYPE::NETWORK:
			path += "NetworkError.png";
			break;
		case POPUP_TYPE::SHUTDOWN:
			path += "Shutdown.png";
			break;
		case POPUP_TYPE::DOWNLOADING:
			path += "Download.png";
			break;
		case POPUP_TYPE::BANNED:
			path += "Banned.png";
			break;
			break;
		}

		mTex = IRR->getVideoDriver()->getTexture(path.c_str());

		XPLICIT_ASSERT(mTex);

		if (!mTex)
			throw EngineError();
	}

	// c++ destructor
	PopupComponent::~PopupComponent()
	{
		if (mTex)
			mTex->drop();
	}

	// update function
	void PopupComponent::update()
	{
		IRR->getVideoDriver()->draw2DImage(mTex, mPos);

		if (KB->key_down(KEY_RETURN))
			mClicked();
	}

	const char* PopupComponent::name() noexcept
	{
		return mPopupId.c_str();
	}

	COMPONENT_TYPE PopupComponent::type() noexcept
	{
		return COMPONENT_GUI;
	}
	
	/* Heads up display */
	HudComponent::HudComponent(const std::int64_t& publicHash)
		: mHealth(0),
		  mPublicHash(publicHash),
		  mNetwork(nullptr)
	{
		XPLICIT_ASSERT(publicHash != -1);

		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);
	}

	HudComponent::~HudComponent() = default;

	void HudComponent::update()
	{
		if (!mNetwork)
			return;

		const auto& packet = mNetwork->get();
	
		if (packet.cmd[XPLICIT_NETWORK_CMD_DAMAGE] == NETWORK_CMD_DAMAGE)
			mHealth = packet.health;
	}


	std::tuple<std::vector<Color<float>>, 
		std::vector<Vector<float>>> UIThemeSchemeManager::set_white_scheme() noexcept
	{
		std::tuple<std::vector<Color<float>>, std::vector<Vector<float>>> scheme;

		/* return scheme */
		return scheme;
	}

	std::tuple<std::vector<Color<float>>, 
		std::vector<Vector<float>>> UIThemeSchemeManager::set_dark_scheme() noexcept
	{
		std::tuple<std::vector<Color<float>>, std::vector<Vector<float>>> scheme;

		/* return scheme */
		return scheme;
	}
}