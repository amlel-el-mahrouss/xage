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
		const POPUP_TYPE popup_type, 
		const char* id) noexcept
		:
			mClicked(on_click),
			mPopupId(id),
			mHudFrame(new ImGUI::UIFrame()),
			mOk(L"OK"),
			mCancel(L"CANCEL")
	{
		XPLICIT_ASSERT(mClicked);
		XPLICIT_ASSERT(!mPopupId.empty());

		switch (popup_type)
		{
		case POPUP_TYPE::KICK:
			mText = L"You have been kicked by the server.";
			break;
		case POPUP_TYPE::NETWORK:
			mText = L"Conneciton has timed-out.";
			break;
		case POPUP_TYPE::SHUTDOWN:
			mText = L"Server has shutdown.";
			break;
		case POPUP_TYPE::CHALLENGE:
			mText = L"Challenge failed by peer.";
			break;
		case POPUP_TYPE::TELEPORTING:
			mText = L"This place wants to teleport on another game.\nProceed?";
			break;
		case POPUP_TYPE::BANNED:
			mText = L"You have been banned, play-xplicit.com/appeals.";
			break;
		default:
			ComponentSystem::get_singleton_ptr()->remove(this);
			return;
		}

		mCancel.LeftClicked = []() {};
		mCancel.Hover = []() {};
		mCancel.RightClicked = []() {};

		mOk.LeftClicked = mClicked;
		mOk.Hover = []() {};
		mOk.RightClicked = []() {};

		mHudFrame->W = 386;
		mHudFrame->H = 188;
		
		mHudFrame->BackgroundColor.setAlpha(255);
		
		mHudFrame->BackgroundColor.setRed(0x0F);
		mHudFrame->BackgroundColor.setGreen(0x0F);
		mHudFrame->BackgroundColor.setBlue(0x0F);

		mHudFrame->X = XPLICIT_DIM.X / 3;
		mHudFrame->Y = XPLICIT_DIM.Y / 4;

		mOk->W = 192;
		mOk->H = 54;

		mOk->X = mHudFrame->X;
		mOk->Y = mHudFrame->Y + mHudFrame->H - mOk->H;

		mOk->BackgroundColor.setAlpha(255);

		mOk->BackgroundColor.setRed(0x1C);
		mOk->BackgroundColor.setGreen(0x1C);
		mOk->BackgroundColor.setBlue(0x1C);

		mCancel->X = mOk->X + mOk->W + 2;
		mCancel->Y = mOk->Y;

		mCancel->BackgroundColor.setAlpha(255);

		mCancel->BackgroundColor.setRed(0x1C);
		mCancel->BackgroundColor.setGreen(0x1C);
		mCancel->BackgroundColor.setBlue(0x1C);

		mCancel->W = 192;
		mCancel->H = 54;
	}
	
	PopupComponent::~PopupComponent()
	{
		if (mHudFrame)
			delete mHudFrame;
	}
	
	void PopupComponent::update(void* class_ptr)
	{
		PopupComponent* _this = (PopupComponent*)class_ptr;

		_this->mHudFrame->update(_this->mHudFrame->BackgroundColor);

		//! Draw text

		//! Title
		ImGUI::UIFont::get_title_font()->draw(L"XPLICIT", recti(vector2di(((_this->mHudFrame->X + _this->mHudFrame->W / 2)), _this->mHudFrame->Y + 30),
			dimension2d(0, 0)), 
			_this->mHudFrame->TextColor, 
			true, 
			true);

		//! label
		ImGUI::UIFont::get_label_font()->draw(_this->mText.c_str(), recti(vector2di(((_this->mHudFrame->X + _this->mHudFrame->W / 2)), _this->mHudFrame->Y + 50),
			dimension2d(0, 0)),
			_this->mHudFrame->TextColor,
			true,
			true);

		_this->mOk.update();
		_this->mCancel.update();
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
		  mHudFrame(new ImGUI::UIFrame())
	{
		// verify hash and network component.
		XPLICIT_ASSERT(mPublicHash != XPLICIT_INVALID_HASH);
		XPLICIT_ASSERT(mNetwork);
		XPLICIT_ASSERT(mHudFrame);

		mHudFrame->BackgroundColor.setRed(0x4B);
		mHudFrame->BackgroundColor.setGreen(0xA5);
		mHudFrame->BackgroundColor.setBlue(0x54);
		mHudFrame->BackgroundColor.setAlpha(225);
		
		mHudFrame->BackgroundHoverColor.setRed(0x4B);
		mHudFrame->BackgroundHoverColor.setGreen(0xA5);
		mHudFrame->BackgroundHoverColor.setBlue(0x54);
		mHudFrame->BackgroundHoverColor.setAlpha(225);

		mHudFrame->H = 10;
		mHudFrame->W = 0;

		mHudFrame->X = 10;
		mHudFrame->Y = 10;
	}

	LocalHudComponent::~LocalHudComponent()
	{
		if (mHudFrame)
			delete mHudFrame;
	}

	void LocalHudComponent::update(void* class_ptr)
	{
		LocalHudComponent* _this = (LocalHudComponent*)class_ptr;

		if (!_this->mNetwork)
			return;

		auto& packet = _this->mNetwork->get();
	
		if (packet.health != _this->mHealth)
		{
			_this->mHealth = packet.health;
			_this->mHudFrame->W = _this->mHealth * 2;
		}

		_this->mHudFrame->update(_this->mHudFrame->BackgroundColor);
	}
}