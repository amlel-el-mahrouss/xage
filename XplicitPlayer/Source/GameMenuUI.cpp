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

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> XPLICIT_TO_WCHAR;

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
			mOk(L"OK")
	{
		XPLICIT_ASSERT(mClicked);
		XPLICIT_ASSERT(!mPopupId.empty());

		switch (popup_type)
		{
		case POPUP_TYPE::KICK:
			mText = platform_string("You have been kicked by the server.\nPress OK to exit client.");
			break;
		case POPUP_TYPE::NETWORK:
			mText = platform_string("The connection has timed-out.\nPress OK to exit client.");
			break;
		case POPUP_TYPE::SHUTDOWN:
			mText = platform_string("The server shut-down.\nPress OK to exit client.");
			break;
		case POPUP_TYPE::CHALLENGE:
			mText = platform_string("Challenge failed by peer.\nPress OK to exit client.");
			break;
		case POPUP_TYPE::BANNED:
			mText = platform_string("You have been banned, play-xplicit.com/appeals.\nPress OK to exit client.");
			break;
		case POPUP_TYPE::LEAVE:
		{
			mText = platform_string("Leave the game?");
			mOk.label(L"Yes");

			break;
		}
		default:
			ComponentSystem::get_singleton_ptr()->remove(this);
			return;
		}

		mOk.LeftClicked = mClicked;

		mHudFrame->W = 504;
		mHudFrame->H = 288;
		
		mHudFrame->BackgroundColor.setAlpha(255);
		
		mHudFrame->BackgroundColor.setRed(0x0F);
		mHudFrame->BackgroundColor.setGreen(0x0F);
		mHudFrame->BackgroundColor.setBlue(0x0F);

		mHudFrame->X = ImGUI::JustifyBy<1.7>(ImGUI::CenterOf(XPLICIT_DIM.X));
		mHudFrame->Y = ImGUI::JustifyBy<1.7>(ImGUI::CenterOf(XPLICIT_DIM.Y));

		mOk->W = 504;
		mOk->H = 41;

		mOk->X = mHudFrame->X;
		mOk->Y = mHudFrame->Y + mHudFrame->H - mOk->H;

		mOk->BackgroundColor.setAlpha(255);
	}
	
	PopupComponent::~PopupComponent()
	{
		if (mHudFrame)
			delete mHudFrame;
	}
	
	void PopupComponent::update(ClassPtr class_ptr)
	{
		PopupComponent* _this = static_cast<PopupComponent*>(class_ptr);

		_this->mHudFrame->update(_this->mHudFrame->BackgroundColor);

		//! Draw text

		//! Title
		ImGUI::UIFont::get_title_font()->draw(L"NOTICE", recti(vector2di(((_this->mHudFrame->X + _this->mHudFrame->W / 2)), _this->mHudFrame->Y + 30),
			dimension2d(0, 0)), 
			_this->mHudFrame->TextColor, 
			true, 
			true);

		//! label
		ImGUI::UIFont::get_label_font()->draw(_this->mText.c_str(), recti(vector2di(((_this->mHudFrame->X + _this->mHudFrame->W / 2)), _this->mHudFrame->Y + 100),
			dimension2d(0, 0)),
			_this->mHudFrame->TextColor,
			true,
			true);

		_this->mOk.update();
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

	LocalFrameComponent::LocalFrameComponent(const char* parent, const char* name)
		: Lua::CLuaClass((String(parent) + "." + name))
	{
		this->insert("Name", name);
		this->insert("Parent", parent);

		this->insert("Left", "0");
		this->insert("Top", "0");
		this->insert("Right", "0");
		this->insert("Bottom", "0");

		this->insert("X", "0");
		this->insert("Y", "0");

		this->insert("Red", "255");
		this->insert("Green", "255");
		this->insert("Blue", "255");
		this->insert("Alpha", "255");
	}

	bool LocalFrameComponent::should_update() { return true; }

	const char* LocalFrameComponent::name() 
	{
		return ((!this->index_as_string("Name").empty()) ? this->index_as_string("Name").c_str() : "LocalFrameComponent"); 
	}

	void LocalFrameComponent::update(ClassPtr klass) noexcept
	{
		LocalFrameComponent* msg = (LocalFrameComponent*)klass;

		int x = msg->index_as_number("Left");
		int y = msg->index_as_number("Top");
		int x2 = msg->index_as_number("Right");
		int y2 = msg->index_as_number("Bottom");

		msg->mFrame.W = x2 - x;
		msg->mFrame.H = y2 - y;

		msg->mFrame.X = msg->index_as_number("X");
		msg->mFrame.Y = msg->index_as_number("Y");

		msg->mFrame.update(ImGUI::ImColor(msg->index_as_number("Alpha"), 
			msg->index_as_number("Red"),
			msg->index_as_number("Green"), 
			msg->index_as_number("Blue")));
	}
}