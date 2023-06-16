/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: CoreUI.h
 *			Purpose: Client UI
 *
 * =====================================================================
 */

#pragma once

#include <Root.h>
#include <NetworkComponent.h>
#include <Component.h>

/* Xplicit Engine Prebuilt dialogs engine */

namespace Xplicit::Player
{
	extern dimension2du XPLICIT_DIM;
}

namespace Xplicit::Player
{
	enum class POPUP_TYPE
	{
		NETWORK_ERROR,
		WAITING_ACK,
		SHUTDOWN,
		KICK,
	};

	class PopupComponent final : public Component
	{
	public:
		PopupComponent() = delete;

	public:
		PopupComponent(const std::function<void()>& on_click, const vector2di pos = vector2di(0, 0), 
			const POPUP_TYPE shutdown_type = POPUP_TYPE::NETWORK_ERROR, const char* id = "Popup") noexcept;

		virtual ~PopupComponent();

		PopupComponent& operator=(const PopupComponent&) = default;
		PopupComponent(const PopupComponent&) = default;

	public:
		virtual INSTANCE_TYPE type() noexcept override;
		virtual const char* name() noexcept override;
		virtual void update() override;
		
	private:
		std::function<void()> mClicked;
		ITexture* m_pTexture;

	private:
		String m_strPopupId;
		vector2di m_vecPos;

	};

	class HUDComponent final : public Component
	{
	public:
		HUDComponent() = delete;

	public:
		explicit HUDComponent(const std::int64_t& publicHash);
		virtual ~HUDComponent();

		HUDComponent& operator=(const HUDComponent&) = delete;
		HUDComponent(const HUDComponent&) = delete;

		virtual const char* name() noexcept override { return "HUD"; }
		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_GUI; }

	public:
		virtual void update() override;

	private:
		NetworkComponent* mNetwork;

	private:
		std::int64_t mPublicHash;
		std::int64_t mHealth;

	};

	class NotificationComponent : public Component
	{
	public:
		NotificationComponent(const char* caption, const char* text);
		virtual ~NotificationComponent();

	public:
		XPLICIT_COPY_DELETE(NotificationComponent);

	public:
		virtual const char* name() noexcept override;
		virtual INSTANCE_TYPE type() noexcept override;

	public:
		virtual void update() override;
		void dispatch() noexcept;

	private:
		ITexture* mTexture;

	private:
		std::int64_t mCountdown;
		gui::IGUIFont* mFnt;
		String mName;
		bool mStart;

	};
}