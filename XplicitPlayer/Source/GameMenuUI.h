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
#include <NMath.h>

#include <Component.h>
#include <NetworkComponent.h>

/* Xplicit Engine default skin */

namespace Xplicit::Player
{
	extern dimension2du XPLICIT_DIM;
}

namespace Xplicit::Player
{
	enum class POPUP_TYPE
	{
		NETWORK,
		DOWNLOADING,
		BANNED,
		SHUTDOWN,
		KICK,
	};

	class PopupComponent final : public Component
	{
	public:
		PopupComponent() = delete;

	public:
		PopupComponent(const std::function<void()>& on_click, const vector2di pos = vector2di(0, 0), 
			const POPUP_TYPE shutdown_type = POPUP_TYPE::NETWORK, const char* id = "Popup") noexcept;

		virtual ~PopupComponent();

	public:
		PopupComponent& operator=(const PopupComponent&) = default;
		PopupComponent(const PopupComponent&) = default;

	public:
		virtual COMPONENT_TYPE type() noexcept override;
		virtual const char* name() noexcept override;
		virtual void update() override;
		
	private:
		using FunctionPopup = std::function<void()>;

	private:
		FunctionPopup mClicked;
		String mPopupId;
		ITexture* mTex;
		vector2di mPos;

	};

	class HudComponent final : public Component
	{
	public:
		HudComponent() = delete;

	public:
		explicit HudComponent(const std::int64_t& publicHash);
		virtual ~HudComponent();

	public:
		HudComponent& operator=(const HudComponent&) = default;
		HudComponent(const HudComponent&) = default;

	public:
		virtual const char* name() noexcept override { return "HUD"; }
		virtual COMPONENT_TYPE type() noexcept override { return COMPONENT_GUI; }

	public:
		virtual void update() override;

	private:
		NetworkComponent* mNetwork;
		std::int64_t mPublicHash;
		std::int64_t mHealth;

	};

	class UIThemeSchemeManager final
	{
	public:
		UIThemeSchemeManager() = default;
		~UIThemeSchemeManager() = default;

	public:
		XPLICIT_COPY_DEFAULT(UIThemeSchemeManager);

	public:
		std::tuple<std::vector<Color<float>>, 
			std::vector<Vector<float>>> set_white_scheme() noexcept;

		std::tuple<std::vector<Color<float>>, 
			std::vector<Vector<float>>> set_dark_scheme() noexcept;

	};
}