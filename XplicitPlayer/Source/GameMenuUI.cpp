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
	
	PopupComponent::PopupComponent(
		const std::function<void()>& on_click, 
		const vector2di pos, 
		const POPUP_TYPE popup_type, 
		const char* id) noexcept
		:
	mClicked(on_click),
	mPos(pos),
	mPopupId(id)
	{
		XPLICIT_ASSERT(mClicked);
		XPLICIT_ASSERT(!mPopupId.empty());

		String path;

		switch (popup_type)
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
		default:
			break;
		}

		mTex = IRR->getVideoDriver()->getTexture(path.c_str());

		XPLICIT_ASSERT(mTex);

		if (!mTex)
			throw EngineError();
	}
	
	PopupComponent::~PopupComponent()
	{
		if (mTex)
			mTex->drop();
	}
	
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
	LocalHudComponent::LocalHudComponent(const std::int64_t& publicHash)
		: mHealth(0),
		  mPublicHash(publicHash),
		  mNetwork(nullptr)
	{
		XPLICIT_ASSERT(publicHash != -1);

		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);
	}

	LocalHudComponent::~LocalHudComponent() = default;

	void LocalHudComponent::update()
	{
		if (!mNetwork)
			return;

		const auto& packet = mNetwork->get();
	
		if (packet.cmd[XPLICIT_NETWORK_CMD_DAMAGE] == NETWORK_CMD_DAMAGE)
			mHealth = packet.health;
	}

	namespace Nixxon
	{
		std::tuple<std::vector<Color<float>>,
			std::vector<Vector<float>>> UIThemeSchemeManager::get_white_scheme() noexcept
		{
			std::tuple<std::vector<Color<float>>, std::vector<Vector<float>>> scheme;

			/* return scheme */
			return scheme;
		}

		std::tuple<std::vector<Color<float>>,
			std::vector<Vector<float>>> UIThemeSchemeManager::get_dark_scheme() noexcept
		{
			std::tuple<std::vector<Color<float>>, std::vector<Vector<float>>> scheme;

			/* return scheme */
			return scheme;
		}
	}
}