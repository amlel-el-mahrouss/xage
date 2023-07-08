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
		const Vector<float> pos,
		const POPUP_TYPE popup_type, 
		const char* id) noexcept
		:
			mClicked(on_click),
			mPos(pos),
			mPopupId(id),
			mTex(nullptr)
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
		default:
			break;
		}

		if (!path.empty())
		{
			mTex = RENDER->getVideoDriver()->getTexture(path.c_str());

			XPLICIT_ASSERT(mTex);

			if (!mTex)
				throw EngineError();
		}
		else
		{
			XPLICIT_INFO("Invalid popup! destroying current instance...");
			ComponentSystem::get_singleton_ptr()->remove(this);
		}
	}
	
	PopupComponent::~PopupComponent()
	{
		if (mTex)
			mTex->drop();
	}
	
	void PopupComponent::update(void* class_ptr)
	{
		PopupComponent* _this = (PopupComponent*)class_ptr;

		if (!_this->mTex)
			return;

		RENDER->getVideoDriver()->draw2DImage(_this->mTex, irr::core::position2di(_this->mPos.X, _this->mPos.Y));
		_this->mClicked();
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
		  mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent")),
		  mPublicHash(publicHash),
		  mHealth(0),
		  mTimeout(0)
	{
		// verify hash and network component.
		XPLICIT_ASSERT(mPublicHash != XPLICIT_INVALID_HASH);
		XPLICIT_ASSERT(mNetwork);
	}

	LocalHudComponent::~LocalHudComponent() = default;

	void LocalHudComponent::update(void* class_ptr)
	{
		LocalHudComponent* _this = (LocalHudComponent*)class_ptr;

		if (!_this->mNetwork)
			return;

		auto& packet = _this->mNetwork->get();
	
		if (packet.health != _this->mHealth)
		{
			_this->mHealth = packet.health;
			_this->mTimeout = 255;
		}

		if (_this->mTimeout < 1)
			return;

		--_this->mTimeout;
	}
}