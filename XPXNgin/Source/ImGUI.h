/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

//! ImGUI Base Classes
#include "ImGuiBase.h"
#include <NMath.h>

namespace XPX::ImGUI
{
	class XPLICIT_API UIInterface
	{
	public:
		UIInterface() = default;
		virtual ~UIInterface() = default;

	public:
		XPLICIT_COPY_DEFAULT(UIInterface);

	public:
		virtual void update() = 0;

	};

	class XPLICIT_API UIButton final : public UIInterface
	{
	public:
		explicit UIButton(const PChar* text);
		~UIButton();

	public:
		XPLICIT_COPY_DEFAULT(UIButton);

	slots:
		bool LeftClicked;
		bool RightClicked;
		bool Hover;

	public:
		void update();
		void label(const PChar* text);
		UIFrame* get() const { return m_pFrame; }

	public:
		UIFrame* operator->() const;

	private:
		UIFontHelper::FontPtr m_pFont{ nullptr };
		BasicString<PChar> mText{ L"" };
		UIFrame* m_pFrame{ nullptr };
		std::uint64_t m_iFadeIn{ 0 };

	};

	class XPLICIT_API UIEditBox final : public UIInterface
	{
	public:
		explicit UIEditBox(const PChar* placeHolder);
		~UIEditBox();

	public:
		XPLICIT_COPY_DEFAULT(UIEditBox);

	public:
		void set_text(const PChar*);

		void set_pos(const int X, const int Y);

		void set_size(const int w, const int h)
		{
			if (!mBox)
				return;

			mBox->W = w;
			mBox->H = h;
			
			if (!mSelection)
				return;

			mSelection->H = h;
		}

		Vector<int> get_pos() noexcept;

	public:
		bool focus() noexcept { return mShallEdit; }

	public:
		void update() override;

	private:
		BasicString<PChar> mPlaceholder{ L"..." };
		BasicString<PChar> mText{ L"" };
		bool mShallEdit{ false };

	private:
		int64_t mCooldown{ 0 };
		UIFrame* mSelection;
		UIFrame* mBox;

	};

	class XPLICIT_API UICheckBox final : public UIInterface
	{
	public:
		explicit UICheckBox();
		~UICheckBox();

	public:
		XPLICIT_COPY_DEFAULT(UICheckBox);

	public:
		void update() override;

	public:

		void set_pos(const int x, const int y)
		{
			if (x < 1 ||
				y < 1)
				return;

			this->X = x;
			this->Y = y;
		}

		void set_size(const int w, const int h)
		{
			this->W = w;
			this->H = h;
		}


	public:
		bool Checked{ true };

	private:
		int32_t H{ 41 };
		int32_t W{ 41 };

	public:
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

	inline float JustifyBy(float formula, const float& x)
	{
		return ((x) / (formula));
	}

	class XPLICIT_API UIWindow final : public UIInterface
	{
	public:
		UIWindow();
		~UIWindow();

	public:
		XPLICIT_COPY_DELETE(UIWindow);

	public:
		class UIContainer
		{
		private:
			UIFrame* mBody{ nullptr };
			friend UIWindow;

		};

		void set_pos(const int x, const int y)
		{
			if (x < 1 ||
				y < 1)
				return;

			mHead.mBody->X = x;
			mHead.mBody->Y = y;
			mBody.mBody->X = x;
			mBody.mBody->Y = y + mHead.mBody->H;
		}

		void set_size(const int w, const int bodyH, const int toolbarH)
		{
			mBody.mBody->W = w;
			mBody.mBody->H = bodyH;

			mHead.mBody->W = w;
			mHead.mBody->H = toolbarH;
		}

		float get_width() { return mBody.mBody->W + mHead.mBody->W; }

		float get_height() { return mBody.mBody->H + mHead.mBody->H; }

		float get_x() { return mBody.mBody->X + mHead.mBody->X; }

		float get_y() { return mBody.mBody->Y + mHead.mBody->Y; }

		void set_title(const char* title) { mTitle = platform_string(title); }

		void update() override
		{
			if (!mHead.mBody ||
				!mBody.mBody)
				return;

			mBody.mBody->update(mBody.mBody->BackgroundColor);

			mHead.mBody->update(mHead.mBody->BackgroundColor);

			UIFontHelper::get_properties_font()->draw(mTitle.c_str(),
				recti(vector2di(mHead.mBody->X + 5, mHead.mBody->Y - 2), dimension2du(0, 0)),
				SColor(255, 255, 255, 255),
				false, false);
		}

		bool in_region() noexcept
		{
			return mBody.mBody->in_region();
		}

	private:
		BasicString<PChar> mTitle;
		UIWindow* mParent;
		UIContainer mHead;
		UIContainer mBody;

	};

#define XPLICIT_UIPROPGRID_HEIGHT_EDITBOX 30

	class XPLICIT_API UIPropGrid final : public UIInterface
	{
	public:
		UIPropGrid(const char* title);
		~UIPropGrid();

	public:
		void insert(UIEditBox* uiPtr)
		{
			if (uiPtr)
			{
				uiPtr->set_pos(mFrame->get_x() + 10, mFrame->get_y() + mPosY);
				uiPtr->set_size(120, XPLICIT_UIPROPGRID_HEIGHT_EDITBOX);

				mPosY += 10;

				this->mElements.push_back(uiPtr);
			}
		}

		void insert(UICheckBox* uiPtr)
		{
			if (uiPtr)
			{
				uiPtr->X = mFrame->get_x();
				uiPtr->Y = mFrame->get_y() + mPosY;

				mPosY += 5;

				this->mElements.push_back(uiPtr);
			}
		}

		UIInterface* operator[](const std::size_t& index)
		{
			return mElements[index];
		}

	public:
		void update() override;

	private:
		std::vector<UIInterface*> mElements;
		std::size_t mPosY;
		UIWindow* mFrame;

	};
}