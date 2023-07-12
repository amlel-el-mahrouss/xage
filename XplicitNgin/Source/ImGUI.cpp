/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "ImGUI.h"

namespace Xplicit::ImGUI
{

	UIButton::UIButton(const wchar_t* text)
		: mText(text)
	{
		m_pFrame = new UIFrame();
		XPLICIT_ASSERT(m_pFrame);

		m_pFont = UIFont::get_font(UIFont::get_label_path().c_str());

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

	void UIButton::update() noexcept
	{
		if (!m_pFrame)
			return;

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
	}
}