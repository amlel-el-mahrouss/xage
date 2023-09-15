/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "MenuUI.h"
#include "Application.h"

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> XPLICIT_TO_WCHAR;

namespace XPX
{
	/*
	 * This is a popup class
	 * used to create pop-up according to a state.
	 */
	
	PopupComponent::PopupComponent(
		const std::function<void()>& on_click, 
		const POPUP_TYPE popup_type, 
		const char* id,
		const char* msg) noexcept
		:
			mClicked(on_click),
			mPopupId(id),
			mHudFrame(new ImGUI::UIFrame()),
			mOk(L"Leave"),
			mTitle(L"Disconnected")
	{
		XPLICIT_ASSERT(mClicked);
		XPLICIT_ASSERT(!mPopupId.empty());

		switch (popup_type)
		{
		case POPUP_TYPE::KICK:
			mText = platform_string(msg ? msg : "You have been kicked.");
			break;
		case POPUP_TYPE::NETWORK:
			mText = platform_string("Connection has timed-out.\nPress OK to exit.");
			break;
		case POPUP_TYPE::SHUTDOWN:
			mText = platform_string("The server has shutdown.");
			break;
		case POPUP_TYPE::CHALLENGE:
			mText = platform_string("Challenge failed by peer.");
			break;
		case POPUP_TYPE::BANNED:
			mText = platform_string("You have been banned.");
			break;
		case POPUP_TYPE::LEAVE:
		{
			mText = platform_string("Leave the game?");
			mTitle = platform_string("Warning");

			mOk.label(L"Yes");

			break;
		}
		default:
			ComponentSystem::get_singleton_ptr()->remove(this);
			return;
		}

		mHudFrame->W = 504;
		mHudFrame->H = 288;
		
		mHudFrame->BackgroundColor.setAlpha(255);
		
		mHudFrame->BackgroundColor.setRed(0x0F);
		mHudFrame->BackgroundColor.setGreen(0x0F);
		mHudFrame->BackgroundColor.setBlue(0x0F);

		mHudFrame->X = ImGUI::JustifyBy(1.7, ImGUI::CenterOf(XPLICIT_DIM.X));
		mHudFrame->Y = ImGUI::JustifyBy(1.7, ImGUI::CenterOf(XPLICIT_DIM.Y));

		mOk->W = 504;
		mOk->H = 41;

		mOk->X = mHudFrame->X;
		mOk->Y = mHudFrame->Y + mHudFrame->H - mOk->H;

		mOk->BackgroundColor.setAlpha(255);
	}
	
	PopupComponent::~PopupComponent()
	{
		delete mHudFrame;
	}
	
	void PopupComponent::update(ClassPtr class_ptr)
	{
		auto* self = static_cast<PopupComponent*>(class_ptr);

		self->mHudFrame->update(self->mHudFrame->BackgroundColor);

		ImGUI::UIFont::get_label_font()->draw(self->mTitle.c_str(), recti(vector2di(((self->mHudFrame->X + self->mHudFrame->W / 2)), self->mHudFrame->Y + 30),
			dimension2d(0, 0)), 
			self->mHudFrame->TextColor, 
			true, 
			true);

		ImGUI::UIFont::get_label_font()->draw(self->mText.c_str(), recti(vector2di(((self->mHudFrame->X + self->mHudFrame->W / 2)), self->mHudFrame->Y + 100),
			dimension2d(0, 0)),
			self->mHudFrame->TextColor,
			true,
			true);

		self->mOk.update();

		if (self->mOk.LeftClicked)
		{
			self->mClicked();
		}
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
	HUDComponent::HUDComponent(const std::int64_t& publicHash)
		:
		  mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent")),
		  mPublicHash(publicHash),
		  mHealth(0),
		  mHudFrame(new ImGUI::UIFrame()),
			mFrameParent(new ImGUI::UIFrame()),
		  mSelectedSlot(-1)
	{
		// verify hash and network component.
		XPLICIT_ASSERT(mPublicHash != XPLICIT_INVALID_HASH);
		XPLICIT_ASSERT(mNetwork);
		XPLICIT_ASSERT(mHudFrame);

		mHudFrame->BackgroundColor.setRed(75);
		mHudFrame->BackgroundColor.setGreen(165);
		mHudFrame->BackgroundColor.setBlue(84);
		mHudFrame->BackgroundColor.setAlpha(225);
		
		mHudFrame->BackgroundHoverColor.setRed(75);
		mHudFrame->BackgroundHoverColor.setGreen(165);
		mHudFrame->BackgroundHoverColor.setBlue(84);
		mHudFrame->BackgroundHoverColor.setAlpha(225);

		mHudFrame->H = 10;
		mHudFrame->W = 0;

		mHudFrame->X = 10;
		mHudFrame->Y = 10;

		mFrameParent->H = 30;
		mFrameParent->W = CAD->getVideoDriver()->getScreenSize().Width;

		mFrameParent->BackgroundHoverColor.setAlpha(50);

		mFrameParent->X = 0;
		mFrameParent->Y = 0;

		std::size_t x_off = 10UL;
		std::size_t y_off = CAD->getVideoDriver()->getScreenSize().Height - 74UL;

		for (auto & mInventorySlot : mInventorySlots)
		{
			mInventorySlot.BackgroundHoverColor.setAlpha(50);
			mInventorySlot.BackgroundColor.setAlpha(70);

			mInventorySlot.W = mInventorySlot.H = 64;
			mInventorySlot.X = x_off;
			mInventorySlot.Y = y_off;

			x_off += 68UL;
		}
	}

	HUDComponent::~HUDComponent()
	{
		delete mHudFrame;
	}

	void HUDComponent::update(void* class_ptr)
	{
		auto* self = (HUDComponent*)class_ptr;

		if (!self->mNetwork)
			return;

		auto& packet = self->mNetwork->get();

		packet.cmd[XPLICIT_NETWORK_CMD_LCLICK] = KEYBOARD->left_down() ? NETWORK_CMD_LCLICK : NETWORK_CMD_INVALID;
		packet.cmd[XPLICIT_NETWORK_CMD_RCLICK] = KEYBOARD->right_down() ? NETWORK_CMD_RCLICK : NETWORK_CMD_INVALID;

		for (size_t i = 0; i < XPLICIT_MAX_ELEMENTS_INVENTORY; ++i)
		{
			if (self->mSelectedSlot != i)
			{
				char to_ascii = i + 48 + 1;

				if (KEYBOARD->key_down(to_ascii))
					self->mSelectedSlot = i;

			}

			if (self->mInventorySlots[i].in_region())
			{
				if (KEYBOARD->left_down() &&
					self->mSelectedSlot != i)
					self->mSelectedSlot = i;

				self->mInventorySlots[i].update(self->mInventorySlots[i].BackgroundHoverColor);
			}
			else
			{
				self->mInventorySlots[i].update(self->mInventorySlots[i].BackgroundColor);
			}

			if (self->mSelectedSlot == i)
			{
				packet.id = i;
				packet.cmd[XPLICIT_NETWORK_CMD_SLOT] = NETWORK_CMD_SLOT;

				self->mNetwork->send(packet);

				self->mNetwork->get().cmd[XPLICIT_NETWORK_CMD_SLOT] = NETWORK_CMD_INVALID;

				CAD->getVideoDriver()->draw2DRectangleOutline(recti(vector2di(self->mInventorySlots[i].X , self->mInventorySlots[i].Y),
					dimension2di(self->mInventorySlots[i].W, self->mInventorySlots[i].H)), irr::video::SColor(255, 0x00, 0x94, 0xFF));
			}
		}

		self->mFrameParent->update(self->mFrameParent->BackgroundHoverColor);
		self->mHudFrame->update(self->mHudFrame->BackgroundColor);

		if (packet.health != self->mHealth)
		{
			self->mHealth = packet.health;
			self->mHudFrame->W = self->mHealth * 2;
		}
	}

	RectComponent::RectComponent(const char* parent, const char* name)
		: Lua::CLuaClass((String(parent) + "." + name))
	{
		this->insert("ClassName", name);
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

	bool RectComponent::should_update() { return true; }

	const char* RectComponent::name() 
	{
        auto rect_name = this->index_as_string("ClassName");
		return ((!this->index_as_string("ClassName").empty()) ? rect_name.c_str() : "RectComponent");
	}

	void RectComponent::update(ClassPtr klass) noexcept
	{
		RectComponent* msg = (RectComponent*)klass;

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