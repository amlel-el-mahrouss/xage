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
		: mText(text), Hover(false), LeftClicked(false), RightClicked(false)
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

		m_pFrame->TextColor.setAlpha(m_iFadeIn);
		m_pFrame->BackgroundColor.setAlpha(m_iFadeIn);
		m_pFrame->BackgroundHoverColor.setAlpha(m_iFadeIn);

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

		//! TODO: SIMD String class
		m_pFont->draw(mText.c_str(), recti(position2di(m_pFrame->X, m_pFrame->Y),
			dimension2d(m_pFrame->W, m_pFrame->H)), m_pFrame->TextColor, true, true);

		if (m_iFadeIn < 255)
			++m_iFadeIn;
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

		mBox->BackgroundColor.setRed(0x0D);
		mBox->BackgroundColor.setGreen(0x0D);
		mBox->BackgroundColor.setBlue(0x0D);
		mBox->BackgroundColor.setAlpha(0x50);

		mBox->BackgroundHoverColor = mBox->BackgroundHoverColor;

		mSelection = new UIFrame();

		mSelection->H = 38;
		mSelection->W = 0;

		mSelection->BackgroundColor.setRed(0x00);
		mSelection->BackgroundColor.setGreen(0x89);
		mSelection->BackgroundColor.setBlue(0xED);
		mSelection->BackgroundColor.setAlpha(0x50);

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
		this->mBox->update(this->mBox->BackgroundColor);

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

		UIFontHelper::get_properties_font()->draw(mText.c_str(), recti(vector2di(mBox->X + 5, mBox->Y), dimension2di(0, 0)), mSelection->TextColor, false, false);
	}

	UICheckBox::UICheckBox()
	{
		this->mCheckBox = new UIFrame();
		XPLICIT_ASSERT(mCheckBox);
	
		mCheckBox->BackgroundColor.setRed(0x0D);
		mCheckBox->BackgroundColor.setGreen(0x0D);
		mCheckBox->BackgroundColor.setBlue(0x0D);
		mCheckBox->BackgroundColor.setAlpha(0x50);

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
			RENDERER->getVideoDriver()->draw2DLine(vector2di(mCheckBox->X + 12, mCheckBox->Y + 30),
				vector2di(mCheckBox->X + 32, mCheckBox->Y + 11),
				SColor(0xFF, 0x43, 0xA0, 0x47));

			RENDERER->getVideoDriver()->draw2DLine(vector2di(mCheckBox->X + 3, mCheckBox->Y + 21),
				vector2di(mCheckBox->X + 12, mCheckBox->Y + 30),
				SColor(0xFF, 0x43, 0xA0, 0x47));
		}
	}

	UIWindow::UIWindow()
		: mParent(nullptr)
	{
		mHead.mBody = new UIFrame();
		mBody.mBody = new UIFrame();

		XPLICIT_ASSERT(mBody.mBody && mHead.mBody);
	
		mHead.mBody->BackgroundColor.set(0xFF, 0x31, 0x31, 0x31);
		mHead.mBody->BackgroundHoverColor.set(0xFF, 0x31, 0x31, 0x31);

		mBody.mBody->BackgroundColor.set(0xFF, 0x1C, 0x1C, 0x1C);
		mBody.mBody->BackgroundHoverColor.set(0xFF, 0x26, 0x26, 0x26);
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

		mFrame->set_size(XPLICIT_DEFAULT_PROPGRID_DIM, 
			RENDERER->getVideoDriver()->getScreenSize().Height,
			RENDERER->getVideoDriver()->getScreenSize().Height);
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
		{
			elem->update();
		}
	}
}