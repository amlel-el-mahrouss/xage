/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: CoreUI.h
 *			Purpose: Client UI
 *
 * =====================================================================
 */

#pragma once

#include <ApplicationContext.h>
#include <NetworkComponent.h>
#include <Component.h>

namespace Xplicit::Client
{
	extern dimension2du XPLICIT_DIM;
}

namespace Xplicit::CoreUI
{
	enum class POPUP_TYPE
	{
		NETWORK_ERROR,
		WAITING_ACK,
		SHUTDOWN,
		KICK,
	};

	class Popup final : public Component
	{
	public:
		Popup() = delete;

	public:
		Popup(const std::function<void()>& on_click, const vector2di pos = vector2di(0, 0), 
			const POPUP_TYPE shutdown_type = POPUP_TYPE::NETWORK_ERROR, const char* id = "Popup") noexcept;

		virtual ~Popup();

		Popup& operator=(const Popup&) = default;
		Popup(const Popup&) = default;

	public:
		virtual void update() override;
		virtual const char* name() noexcept override;
		virtual INSTANCE_TYPE type() noexcept override;

	private:
		std::function<void()> m_onClick;
		ITexture* m_pTexture;

	private:
		String m_strPopupId;
		vector2di m_vecPos;

	};

	class HUD final : public Component
	{
	public:
		HUD();
		virtual ~HUD();

		HUD& operator=(const HUD&) = delete;
		HUD(const HUD&) = delete;

		virtual const char* name() noexcept override { return "HUD"; }
		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_GUI; }

		virtual void update() override;

	private:
		NetworkComponent* m_network;

	private:
		std::int64_t m_health;

	};

	class CoreUIDialog : public Component
	{
	public:
		CoreUIDialog(const char* name, const char* texture) 
			:
			mName(name), 
			mTexture(IRR->getVideoDriver()->getTexture(texture)),
			mFontUrbanist(nullptr)
		{
			String fnt = XPLICIT_ENV("APPDATA");
			fnt += "/Data/Urbanist.xml";

			mFontUrbanist = IRR->getGUIEnvironment()->getFont(fnt.c_str());
		}

		~CoreUIDialog()
		{
			if (mTexture)
				mTexture->drop();
		}

	public:
		XPLICIT_COPY_DELETE(CoreUIDialog);

	public:
		virtual const char* name() noexcept override { XPLICIT_ASSERT(!mName.empty()); return mName.c_str(); }
		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_GUI; }

		virtual void update() override 
		{
			static float tween_start = 8;

			if (tween_start > 1.5)
				tween_start -= 0.01f;

			static const float final_pos = 2.8;

			if (tween_start < 8)
			{
				IRR->getVideoDriver()->draw2DImage(mTexture, vector2di(
					Xplicit::Client::XPLICIT_DIM.Width / 1.5,
					Xplicit::Client::XPLICIT_DIM.Height / tween_start));
			}
		}

	protected:
		IGUIFont* mFontUrbanist;
		ITexture* mTexture;

	private:
		String mName;
		String mText;

	};

	bool alert(const char* message);
}