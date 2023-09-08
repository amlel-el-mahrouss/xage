/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#include "ImGUI.h"

namespace Xplicit::ImGUI
{

	UIButton::UIButton(const PChar* text)
		: mText(text)
	{
		m_pFrame = new UIFrame();
		XPLICIT_ASSERT(m_pFrame);

		m_pFont = UIFont::get_font(UIFont::get_label_path().c_str());

		m_pFrame->W = 185;
		m_pFrame->H = 48;

		m_pFrame->X = 0;
		m_pFrame->Y = 0;


		Hover = []() {};
		LeftClicked = []() {};
		RightClicked = []() {};
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

		if (m_pFrame->in_region())
		{
			m_pFrame->update(m_pFrame->BackgroundHoverColor);

			try
			{
				if (KB->left_down())
					LeftClicked();

				if (KB->right_down())
					RightClicked();

				Hover();
			}
			catch (const std::bad_function_call& err)
			{
				XPLICIT_INFO("Bad Funciton call!!! Please check your lambdas");
			}
		}
		else
		{
			m_pFrame->update(m_pFrame->BackgroundColor);
		}

		//! TODO: SIMD String class
		m_pFont->draw(mText.c_str(), irr::core::recti(position2di(m_pFrame->X, m_pFrame->Y),
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
		mBox->BackgroundColor.setAlpha(255);

		mBox->BackgroundHoverColor = mBox->BackgroundHoverColor;

		mSelection = new UIFrame();

		mSelection->H = 38;
		mSelection->W = 0;

		mSelection->BackgroundColor.setRed(0x00);
		mSelection->BackgroundColor.setGreen(0x89);
		mSelection->BackgroundColor.setBlue(0xED);
		mSelection->BackgroundColor.setAlpha(127);

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
		this->mSelection->update(this->mSelection->BackgroundColor);

		if (this->mBox->in_region() &&
			KB->left_down())
		{
			this->mShallEdit = !this->mShallEdit;
		}
		else if (!this->mBox->in_region() &&
			KB->left_down())
		{
			this->mShallEdit = false;
		}

		if (this->mShallEdit)
		{
			if (auto key = KB->key_down();
				key <= 255 && key > 0)
			{
				if (key == KEY_BACK)
				{
					this->mSelection->W -= 12;

					if (this->mText.size() > 0)
					{
						this->mText.erase(this->mText.size() - 1);
						std::this_thread::sleep_for(std::chrono::milliseconds(130));
					}
				}

				if (isxdigit(key) ||
					isalpha(key) ||
					isprint(key))
				{
					if (this->mText.size() > (this->mBox->W / 12))
						return;

					this->mText += tolower(key);

					// we double check the size, so that the selection bar doesn't overflow...
					if (this->mText.size() <= (this->mBox->W / 12))
						this->mSelection->W += 12;

					std::this_thread::sleep_for(std::chrono::milliseconds(130));
				}
			}

		}

		if (!mText.empty())
			UIFont::get_label_font()->draw(mText.c_str(), irr::core::recti(vector2di(mSelection->X, mSelection->Y), dimension2di(0, 0)), mSelection->TextColor, false, false);
		else
			UIFont::get_label_font()->draw(mPlaceholder.c_str(), irr::core::recti(vector2di(mSelection->X, mSelection->Y), dimension2di(0, 0)), mSelection->TextColor, false, false);
	}

	UICheckBox::UICheckBox()
	{
		this->mCheckBox = new UIFrame();
		XPLICIT_ASSERT(mCheckBox);
	
		mCheckBox->BackgroundColor.setRed(0x0D);
		mCheckBox->BackgroundColor.setGreen(0x0D);
		mCheckBox->BackgroundColor.setBlue(0x0D);
		mCheckBox->BackgroundColor.setAlpha(255);

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
			if (KB->left_down())
			{
				this->Checked = !this->Checked;
				std::this_thread::sleep_for(std::chrono::milliseconds(130));
			}
		}

		mCheckBox->update(mCheckBox->BackgroundColor);

		if (this->Checked)
		{
			RENDER->getVideoDriver()->draw2DLine(vector2di(mCheckBox->X + 12, mCheckBox->Y + 30), 
				vector2di(mCheckBox->X + 32, mCheckBox->Y + 11),
				SColor(0xFF, 0x43, 0xA0, 0x47));

			RENDER->getVideoDriver()->draw2DLine(vector2di(mCheckBox->X + 3, mCheckBox->Y + 21),
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

		mFrame->set(title);
		mFrame->set_pos(0, 0);

		mFrame->set_size(XPLICIT_DEFAULT_PROPGRID_DIM, 
			RENDER->getVideoDriver()->getScreenSize().Height, 
			RENDER->getVideoDriver()->getScreenSize().Height);
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