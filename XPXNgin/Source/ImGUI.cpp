/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "ImGUI.h"

namespace XPX::ImGUI
{
	UIButton::UIButton(const PChar* text)
		: mText(text), Hover(false), LeftClicked(false), RightClicked(false), m_iFadeIn(0UL)
	{
		m_pFrame = new UIFrame();
		XPLICIT_ASSERT(m_pFrame);

		m_pFont = UIFontHelper::get_font(UIFontHelper::get_label_path().c_str());

		m_pFrame->W = 185;
		m_pFrame->H = 48;

		m_pFrame->X = 0;
		m_pFrame->Y = 0;
	}

	UIButton::~UIButton()
	{
		if (m_pFrame)
			delete m_pFrame;
	}

	UIFrame* UIButton::operator->() const
	{
		return m_pFrame;
	}

	void UIButton::update()
	{
		if (!m_pFrame)
			return;

		m_pFrame->TextColor.A = m_iFadeIn;
		m_pFrame->BackgroundColor.A = m_iFadeIn;
		m_pFrame->BackgroundHoverColor.A = m_iFadeIn;

		this->Hover = m_pFrame->in_region();

		if (m_pFrame->in_region())
		{
			m_pFrame->update(m_pFrame->BackgroundHoverColor);

			LeftClicked = KEYBOARD->left_down();
			RightClicked = KEYBOARD->right_down();
		}
		else
		{
			m_pFrame->update(m_pFrame->BackgroundColor);
		}

		if (m_iFadeIn < 255)
			++m_iFadeIn;

		m_pFrame->SpriteBatch->Begin();

		auto pos = XMFLOAT2((m_pFrame->X + m_pFrame->W) * 0.742f, m_pFrame->Y + 3);

		XMFLOAT4 clr(m_pFrame->TextColor.R, m_pFrame->TextColor.G
			, m_pFrame->TextColor.B,
			m_pFrame->TextColor.A);

		m_pFont->DrawString(m_pFrame->SpriteBatch.get(),
			mText.c_str(),
			XMLoadFloat2(&pos),
			XMLoadFloat4(&clr),
			0.f);

		m_pFrame->SpriteBatch->End();
	}

	void UIButton::label(const PChar* text)
	{
		this->mText = text;
		XPLICIT_ASSERT(!this->mText.empty());
	}

	UIEditBox::UIEditBox(const PChar* placeHolder)
		: mPlaceholder(placeHolder ? placeHolder : L"..."), mText(L"")
	{
		mBox = new UIFrame();

		mBox->W = 279;
		mBox->H = 38;

		mBox->BackgroundColor.R = (0x0D);
		mBox->BackgroundColor.G = (0x0D);
		mBox->BackgroundColor.B = (0x0D);
		mBox->BackgroundColor.A = (0x50);

		mBox->BackgroundHoverColor = mBox->BackgroundHoverColor;

		mSelection = new UIFrame();

		mSelection->H = 38;
		mSelection->W = 0;

		mSelection->BackgroundColor.R = (0x00);
		mSelection->BackgroundColor.G = (0x89);
		mSelection->BackgroundColor.B = (0xED);
		mSelection->BackgroundColor.A = (0x50);

		mSelection->BackgroundHoverColor = mSelection->BackgroundHoverColor;

		mSelection->X = 2;
		mSelection->Y = 2;
	}

	UIEditBox::~UIEditBox()
	{
		if (this->mBox)
			delete mBox;

		if (this->mSelection)
			delete mSelection;
	}

	void UIEditBox::set_text(const PChar* text)
	{
		if (text)
			mText = text;
	}

	void UIEditBox::set_pos(const int X, const int Y)
	{
		this->mBox->X = X;
		this->mBox->Y = Y;

		this->mSelection->X = X;
		this->mSelection->Y = Y;
	}

	Vector<int> UIEditBox::get_pos() noexcept
	{
		return Vector<int>(this->mBox->X, this->mBox->Y);
	}


	void UIEditBox::update()
	{
		if (this->mBox->in_region() &&
			KEYBOARD->left_down())
		{
			this->mShallEdit = !this->mShallEdit;
		}
		else if (!this->mBox->in_region() &&
			KEYBOARD->left_down())
		{
			this->mShallEdit = false;
		}

		if (this->mShallEdit)
		{
			static bool is_caps = false;

			auto key = KEYBOARD->key_down();

			if (key)
			{

				mShallEdit = false;

				if (key == KEY_BACK)
				{
					this->mSelection->W -= 12;

					if (this->mText.size() > 0)
					{
						this->mText.erase(this->mText.size() - 1);
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
				}
				else if (key >= KEY_KEY_0 &&
					key <= KEY_KEY_Z ||
					key == KEY_SPACE)
				{
					if (this->mText.size() < (this->mBox->W / 12))
					{
						this->mText += is_caps ? toupper(key) : tolower(key);

						// we double check the size, so that the selection bar doesn't overflow...
						if (this->mSelection->W < this->mBox->W &&
							key != KEY_SPACE)
							this->mSelection->W += 12;

					}

				}
				else if (key == KEY_CAPITAL)
				{
					is_caps = !is_caps;
				}


				mShallEdit = true;
			}
		}

		this->mBox->SpriteBatch->Begin();

		XMFLOAT4 clr(this->mBox->TextColor.R, 
			this->mBox->TextColor.G, this->mBox->TextColor.B,
			this->mBox->TextColor.A);

		auto pos = XMFLOAT2(this->mBox->X + 5, this->mBox->Y);

		ImGUI::UIFontHelper::get_label_font()->DrawString(this->mBox->SpriteBatch.get(),
			this->mText.c_str(),
			XMLoadFloat2(&pos),
			XMLoadFloat4(&clr),
			0.f);

		this->mBox->SpriteBatch->End();

		this->mBox->update(this->mBox->BackgroundColor);
	}

	UICheckBox::UICheckBox()
	{
		this->mCheckBox = new UIFrame();
		XPLICIT_ASSERT(mCheckBox);
	
		mCheckBox->BackgroundColor.R = (0x0D/255);
		mCheckBox->BackgroundColor.G = (0x0D/255);
		mCheckBox->BackgroundColor.B = (0x0D/255);
		mCheckBox->BackgroundColor.A = (0x50/255);

		mCheckBox->W = this->W;
		mCheckBox->H = this->H;

	}

	UICheckBox::~UICheckBox()
	{
		if (this->mCheckBox)
			delete mCheckBox;
	}

	void UICheckBox::update()
	{
		mCheckBox->Y = this->Y;
		mCheckBox->X = this->X;

		if (this->mCheckBox->in_region())
		{
			if (KEYBOARD->left_down())
			{
				this->Checked = !this->Checked;
				std::this_thread::sleep_for(std::chrono::milliseconds(130));
			}
		}

		mCheckBox->update(mCheckBox->BackgroundColor);

		if (this->Checked)
		{
			// TODO, remake check mark.
		}
	}

	UIWindow::UIWindow()
		: mParent(nullptr)
	{
		mHead.mBody = new UIFrame();
		mBody.mBody = new UIFrame();

		XPLICIT_ASSERT(mBody.mBody && mHead.mBody);

		auto bk_head = 0x31;

		mHead.mBody->BackgroundColor = Color<float>(bk_head, bk_head, bk_head);
		mHead.mBody->BackgroundHoverColor = Color<float>(bk_head, bk_head, bk_head);

		auto bk = 0x1C;
		auto bk_hover = 0x26;

		mBody.mBody->BackgroundColor = Color<float>(bk, bk, bk);
		mBody.mBody->BackgroundHoverColor = Color<float>(bk_hover, bk_hover, bk_hover);
	}

	UIWindow::~UIWindow()
	{
		if (mBody.mBody)
			delete mBody.mBody;

		if (mHead.mBody)
			delete mHead.mBody;
	}

	UIPropGrid::UIPropGrid(const char* title)
		: mFrame(new UIWindow()), mPosY(30)
	{
#define XPLICIT_DEFAULT_PROPGRID_DIM 250

		mFrame->set_title(title);
		mFrame->set_pos(0, 0);

		// TODO: set frame size.
	}

	UIPropGrid::~UIPropGrid()
	{
		if (mFrame)
			delete mFrame;
	}

	void UIPropGrid::update()
	{
		if (!this->mFrame)
			return;

		this->mFrame->update();

		for (auto& elem : this->mElements)
			elem->update();
	}
}