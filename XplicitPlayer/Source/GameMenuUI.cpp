/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
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
		const Ogre::Vector2 pos, 
		const POPUP_TYPE popup_type, 
		const char* id) noexcept
		:
			mClicked(on_click),
			mPos(pos),
			mPopupId(id)
	{
		XPLICIT_ASSERT(mClicked);
		XPLICIT_ASSERT(!mPopupId.empty());

		String path = "";

		switch (popup_type)
		{
		case POPUP_TYPE::KICK:
			path = "network_kick.png";
			break;
		case POPUP_TYPE::NETWORK:
			path = "network_timeout.png";
			break;
		case POPUP_TYPE::SHUTDOWN:
			path = "network_shutdown.png";
			break;
		case POPUP_TYPE::TELEPORTING:
			path = "network_teleport.png";
			break;
		case POPUP_TYPE::BANNED:
			path = "network_ban.png";
			break;
		case POPUP_TYPE::CHALLENGE:
			path = "network_challenge.png";
			break;
		default:
			break;
		}

		if (!path.empty())
		{
		}
		else
		{
			XPLICIT_INFO("Invalid popup! destroying current instance...");
			ComponentManager::get_singleton_ptr()->remove(this);
		}
	}
	
	PopupComponent::~PopupComponent()
	{

	}
	
	void PopupComponent::update()
	{
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
		:
		  mNetwork(ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent")),
		  mPublicHash(publicHash),
		  mHealth(0),
		  mTimeout(0)
	{
		// verify hash.
		XPLICIT_ASSERT(mPublicHash != XPLICIT_INVALID_HASH);

		// verify textures.
		XPLICIT_ASSERT(mNetwork);
	}

	LocalHudComponent::~LocalHudComponent() = default;

	void LocalHudComponent::update()
	{
		if (!mNetwork)
			return;

		auto& packet = mNetwork->get();
	
		if (packet.health != mHealth)
		{
			mHealth = packet.health;


			mTimeout = 255;
		}

		if (mTimeout < 1)
			return;
		--mTimeout;
	}
}