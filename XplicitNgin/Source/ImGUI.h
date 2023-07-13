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
		explicit UIButton(const PChar* text);
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
		BasicString<PChar> mText{ L"" };
		UIFrame* m_pFrame{ nullptr };
		std::uint64_t m_iFadeIn{ 0 };

	};

	class XPLICIT_API UIEditBox final
	{
	public:
		explicit UIEditBox(const PChar* placeHolder);
		~UIEditBox();

	public:
		XPLICIT_COPY_DEFAULT(UIEditBox);

	public:
		void set_text(const PChar*);
		void set_pos(const int X, const int Y);
		Vector<int> get_pos() noexcept;

	public:
		void update();

	private:
		BasicString<PChar> mPlaceholder{ L"..." };
		BasicString<PChar> mText{ L"" };
		bool mShallEdit{ false };

	private:
		int64_t mCooldown{ 0 };
		UIFrame* mSelection;
		UIFrame* mBox;

	};

	class XPLICIT_API UICheckBox final
	{
	public:
		explicit UICheckBox();
		~UICheckBox();

	public:
		XPLICIT_COPY_DEFAULT(UICheckBox);

	public:
		void update();

	public:
		bool Checked{ true };

		int32_t H{ 41 };
		int32_t W{ 41 };

		int32_t X{ 0 };
		int32_t Y{ 0 };

	private:
		UIFrame* mCheckBox{ nullptr };

	};

	inline std::int32_t CenterOf(const std::int32_t& x)
	{
		int _tmp_x = x;
		int _x_ref = 0;

		for (int _x = 0; _x < _tmp_x; ++_x)
		{
			if ((_x + _x_ref) >= _tmp_x)
				return _x;

			++_x_ref;
		}

		return 0;
	}

	template <double Formula>
	inline std::int32_t JustifyBy(const std::int32_t& x)
	{
		return ((x) / (Formula));
	}
}