/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

//! ImGUI Base Classes
#include "ImGuiUIBase.h"
#include <NMath.h>

namespace Xplicit::ImGUI
{
	class XPLICIT_API UIButton final
	{
	public:
		UIButton();
		~UIButton();

	public:
		XPLICIT_COPY_DEFAULT(UIButton);

	slots:
		std::function<void()> LeftClicked;
		std::function<void()> RightClicked;
		std::function<void()> Hover;

	public:
		void update() noexcept;
		UIFrame* operator->() const;

	private:
		UIFrame* m_pFrame{ nullptr };
		String mText;

	};
}