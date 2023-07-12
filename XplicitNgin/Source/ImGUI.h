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
		void update();
		UIFrame* get() const { return m_pFrame; }

	public:
		UIFrame* operator->() const;

	private:
		UIFont::FontPtr m_pFont{ nullptr };
		BasicString<wchar_t> mText{ L"" };
		UIFrame* m_pFrame{ nullptr };
		std::uint64_t m_iFadeIn{ 0 };

	};

	class XPLICIT_API UIEditBox final
	{
	public:
		explicit UIEditBox(const wchar_t* placeHolder);
		~UIEditBox();

	public:
		XPLICIT_COPY_DEFAULT(UIEditBox);

	public:
		void set_text(const wchar_t*);
		void set_pos(const int X, const int Y);
		Vector<int> get_pos() noexcept;

	public:
		void update();

	private:
		BasicString<wchar_t> mPlaceholder{ L"..." };
		BasicString<wchar_t> mText{ L"" };
		bool mShallEdit{ false };

	private:
		UIFrame* mSelection;
		UIFrame* mBox;

	};
}