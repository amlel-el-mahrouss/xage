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

#include <NetworkComponent.h>
#include <Component.h>

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
		ITexture* m_texture;
		int64_t m_health;

	};
}