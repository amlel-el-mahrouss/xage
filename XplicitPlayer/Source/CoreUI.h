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

namespace Xplicit::Player
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

	class Notification : public Component
	{
	public:
		Notification(const char* name, const char* texture) 
			:
			mName(name), 
			mTexture(IRR->getVideoDriver()->getTexture(texture)),
			mStart(true),
			mCountdown(3000)
		{}

		virtual ~Notification()
		{
			if (mTexture)
				mTexture->drop();
		}

	public:
		XPLICIT_COPY_DELETE(Notification);

	public:
		virtual const char* name() noexcept override { XPLICIT_ASSERT(!mName.empty()); return mName.c_str(); }
		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_GUI; }

		virtual void update() override 
		{
			static float tweenStart = 8;

			if (mStart)
			{
				if (tweenStart > 1.5)
					tweenStart -= 0.01f;
			}
			else
			{
				if (tweenStart > 1.5)
					tweenStart += 0.01f;

				if (tweenStart >= 8)
				{
					ComponentManager::get_singleton_ptr()->remove(this);
					return;
				}
			}

			static const float final_pos = 2.8;

			if (tweenStart < 8)
			{
				IRR->getVideoDriver()->draw2DImage(mTexture, vector2di(
					Xplicit::Player::XPLICIT_DIM.Width / 1.5,
					Xplicit::Player::XPLICIT_DIM.Height / tweenStart)
				);
			}

			--mCountdown;

			if (mCountdown < 1)
				this->dispatch();
		}

		void dispatch() noexcept { mStart = false; }

	private:
		ITexture* mTexture;

	private:
		std::int64_t mCountdown;
		String mName;
		bool mStart;

	};
}