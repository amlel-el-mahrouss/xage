/*
 * =====================================================================
 *
 *			XAGEPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "GameUI.h"
#include "App.h"

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> XPLICIT_TO_WCHAR;

namespace XPX
{
	MessageComponent::MessageComponent(
		const std::function<void()>& on_click, 
		const POPUP_TYPE popup_type, 
		const char* id,
		const char* msg) noexcept
		:
			mClicked(on_click),
			mPopupId(id),
			mHudFrame(new ImGUI::UIFrame()),
			mOk(L"Yes"),
			mTitle(Bites::XPLICIT_APP_NAME)
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
			mText = platform_string("Leave?");
			mTitle = platform_string("Lobby");

			break;
		}
		default:
			ComponentSystem::get_singleton_ptr()->remove(this);
			return;
		}

		mHudFrame->W = 504;
		mHudFrame->H = 288;
		
		mHudFrame->BackgroundColor.A = (255);
		mHudFrame->BackgroundColor.R = (0x0F);
		mHudFrame->BackgroundColor.G = (0x0F);
		mHudFrame->BackgroundColor.B = (0x0F);

		mHudFrame->X = ImGUI::JustifyBy(1.4, ImGUI::CenterOf(XPLICIT_MIN_WIDTH));
		mHudFrame->Y = ImGUI::JustifyBy(1.7, ImGUI::CenterOf(XPLICIT_MIN_HEIGHT));

		mOk->W = 504;
		mOk->H = 41;

		mOk->X = mHudFrame->X;
		mOk->Y = (mHudFrame->Y + mHudFrame->H) - mOk->H;
	}
	
	MessageComponent::~MessageComponent()
	{
		delete mHudFrame;
	}
	
	bool MessageComponent::should_update() noexcept { return true; }

	void MessageComponent::update(ClassPtr class_ptr)
	{
		auto* self = static_cast<MessageComponent*>(class_ptr);

		self->mHudFrame->update(self->mHudFrame->BackgroundColor);
		self->mOk.update();

		if (self->mOk.LeftClicked)
			self->mClicked();

		self->mHudFrame->SpriteBatch->Begin();

		XMFLOAT2 pos(self->mHudFrame->X + self->mHudFrame->W / 2, self->mHudFrame->Y + 30);
		XMFLOAT4 clr(self->mHudFrame->TextColor.R, self->mHudFrame->TextColor.G, self->mHudFrame->TextColor.B, self->mHudFrame->TextColor.A);

		auto origin = ImGUI::UIFontHelper::get_label_font()->MeasureString(self->mTitle.c_str()) / 2.f;

		ImGUI::UIFontHelper::get_label_font()->DrawString(self->mHudFrame->SpriteBatch.get(),
			self->mTitle.c_str(),
			XMLoadFloat2(&pos),
			XMLoadFloat4(&clr),
			0.f, origin);

		origin = ImGUI::UIFontHelper::get_label_font()->MeasureString(self->mText.c_str()) / 2.f;

		pos = XMFLOAT2(self->mHudFrame->X + self->mHudFrame->W / 2, self->mHudFrame->Y + 100);
		
		ImGUI::UIFontHelper::get_label_font()->DrawString(self->mHudFrame->SpriteBatch.get(),
			self->mText.c_str(),
			XMLoadFloat2(&pos),
			XMLoadFloat4(&clr),
			0.f, origin);

		self->mHudFrame->SpriteBatch->End();
	}

	const char* MessageComponent::name() noexcept
	{
		return mPopupId.c_str();
	}

	COMPONENT_TYPE MessageComponent::type() noexcept
	{
		return COMPONENT_GUI;
	}
	
	/* Heads up display */
	HUDComponent::HUDComponent(const std::int64_t& publicHash)
		:
		  mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent")),
		  mPublicHash(publicHash),
		  mHealth(XPX_DEFAULT_HEALTH),
		  mHudFrame(new ImGUI::UIFrame()),
		  mFrameParent(new ImGUI::UIFrame()),
		  mSelectedSlot(-1)
	{
		// verify hash and network component.
		XPLICIT_ASSERT(mPublicHash != XPLICIT_INVALID_HASH);
		XPLICIT_ASSERT(mNetwork);
		XPLICIT_ASSERT(mHudFrame);

		mHudFrame->BackgroundColor.R = 75;
		mHudFrame->BackgroundColor.G = 165;
		mHudFrame->BackgroundColor.B = 84;
		mHudFrame->BackgroundColor.A = 255;
		
		mHudFrame->BackgroundHoverColor.R = 75;
		mHudFrame->BackgroundHoverColor.G = 165;
		mHudFrame->BackgroundHoverColor.B = 84;

		mHudFrame->H = 10;
		mHudFrame->W = 0;

		mHudFrame->X = 10;
		mHudFrame->Y = 10;

		mFrameParent->H = 30;
		mFrameParent->W = XPLICIT_MIN_WIDTH;

		mFrameParent->BackgroundColor.R = 40;
		mFrameParent->BackgroundColor.G = 40;
		mFrameParent->BackgroundColor.B = 40;
		mFrameParent->BackgroundHoverColor.A = 50;

		mFrameParent->X = 0;
		mFrameParent->Y = 0;

		std::size_t x_off = 10UL;
		std::size_t y_off = XPLICIT_MIN_WIDTH - 74UL;

		for (auto & mInventorySlot : mInventorySlots)
		{
			mInventorySlot.BackgroundHoverColor.A = (50);
			mInventorySlot.BackgroundColor.A = (70);

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

		if (!self ||
			!self->mNetwork)
			return;

		NetworkPacket packet = self->mNetwork->get();

		packet.cmd[XPLICIT_NETWORK_CMD_LCLICK] = KEYBOARD->left_down() ? NETWORK_CMD_LCLICK : NETWORK_CMD_INVALID;
		packet.cmd[XPLICIT_NETWORK_CMD_RCLICK] = KEYBOARD->right_down() ? NETWORK_CMD_RCLICK : NETWORK_CMD_INVALID;

		for (size_t i = 0; i < XPX_MAX_WEAPONS; ++i)
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
				else if (KEYBOARD->left_down() &&
					self->mSelectedSlot == i)
					self->mSelectedSlot = -1;

				self->mInventorySlots[i].update(self->mInventorySlots[i].BackgroundHoverColor);
			}

			if (self->mSelectedSlot == i)
			{
				packet.id = i;
				packet.cmd[XPLICIT_NETWORK_CMD_SLOT] = NETWORK_CMD_SLOT;

				self->mNetwork->send(packet);

				self->mNetwork->get().cmd[XPLICIT_NETWORK_CMD_SLOT] = NETWORK_CMD_INVALID;

				self->mInventorySlots[i].update(self->mInventorySlots[i].BackgroundHoverColor);
			}
			else
			{
				self->mInventorySlots[i].update(self->mInventorySlots[i].BackgroundColor);
			}

		}

		self->mFrameParent->update(self->mFrameParent->BackgroundHoverColor);
		self->mHudFrame->update(self->mHudFrame->BackgroundColor);

		if (packet.cmd[XPLICIT_NETWORK_CMD_DAMAGE] == NETWORK_CMD_DAMAGE)
			self->mHealth = packet.health;

		self->mHudFrame->W = self->mHealth * 2;
	}
}