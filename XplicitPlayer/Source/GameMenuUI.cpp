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

		String path = "overlay.png";

		switch (popup_type)
		{
		case POPUP_TYPE::KICK:
			break;
		case POPUP_TYPE::NETWORK:
			break;
		case POPUP_TYPE::SHUTDOWN:
			break;
		case POPUP_TYPE::TELEPORTING:
			break;
		case POPUP_TYPE::BANNED:
			break;
		default:
			break;
		}

		mTex = RENDER->getVideoDriver()->getTexture(path.c_str());

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
		RENDER->getVideoDriver()->draw2DImage(mTex, mPos);

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
		:
		  mNetwork(ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent")),
		  mPublicHash(publicHash),
		  mHealth(0),
		  mOverlay(RENDER->getVideoDriver()->getTexture("hit_overlay.png"))
	{
		// verify hash.
		XPLICIT_ASSERT(mPublicHash != XPLICIT_INVALID_HASH);

		// verify textures.
		XPLICIT_ASSERT(mNetwork);
		XPLICIT_ASSERT(mOverlay);
	}

	LocalHudComponent::~LocalHudComponent() = default;

	void LocalHudComponent::update()
	{
		if (!mNetwork)
			return;

		auto& packet = mNetwork->get();
	
		if (packet.cmd[XPLICIT_NETWORK_CMD_DAMAGE] == NETWORK_CMD_DAMAGE)
		{
			mHealth = packet.health;

			RENDER->getVideoDriver()->draw2DImage(mOverlay, vector2di(0, 0), rect(0, 0, 1280, 720),
				nullptr,
				SColor(255, 255, 255, 255),
				true);

			packet.cmd[XPLICIT_NETWORK_CMD_DAMAGE] = NETWORK_CMD_DAMAGE;
		}
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