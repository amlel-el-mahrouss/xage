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
		explicit UIButton(const wchar_t* text);
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
		UIFont::FontPtr m_pFont{ nullptr };
		BasicString<wchar_t> mText{ L"" };
		UIFrame* m_pFrame{ nullptr };
		std::uint64_t m_iFadeIn{ 0 };

	};
}