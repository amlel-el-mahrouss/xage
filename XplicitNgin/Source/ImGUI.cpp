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

	UIButton::UIButton()
	{
		m_pFrame = new UIFrame();
		XPLICIT_ASSERT(m_pFrame);

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
}