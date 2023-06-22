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
		
		PopupComponent(const std::function<void()>& on_click, const vector2di pos = vector2di(0, 0), 
			const POPUP_TYPE shutdown_type = POPUP_TYPE::NETWORK, const char* id = "Popup") noexcept;

		~PopupComponent() override;
		
		PopupComponent& operator=(const PopupComponent&) = default;
		PopupComponent(const PopupComponent&) = default;
		
		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;
		void update() override;
		
	private:
		using FunctionPopup = std::function<void()>;
		
		FunctionPopup mClicked;
		String mPopupId;
		ITexture* mTex;
		vector2di mPos;

	};

	class LocalHudComponent final : public Component
	{
	public:
		LocalHudComponent() = delete;
		
		explicit LocalHudComponent(const std::int64_t& publicHash);
		~LocalHudComponent() override;
		
		LocalHudComponent& operator=(const LocalHudComponent&) = default;
		LocalHudComponent(const LocalHudComponent&) = default;

		const char* name() noexcept override { return "HUD"; }
		COMPONENT_TYPE type() noexcept override { return COMPONENT_GUI; }
		
		void update() override;

	private:
		NetworkComponent* mNetwork;
		std::int64_t mPublicHash;
		std::int64_t mHealth;

	};

	namespace Nixxon
	{
		class UIThemeSchemeManager final
		{
		public:
			UIThemeSchemeManager() = default;
			~UIThemeSchemeManager() = default;

			XPLICIT_COPY_DEFAULT(UIThemeSchemeManager);

			static std::tuple<std::vector<Color<float>>,
				std::vector<Vector<float>>> get_white_scheme() noexcept;

			static std::tuple<std::vector<Color<float>>,
				std::vector<Vector<float>>> get_dark_scheme() noexcept;

		};

		class UIWidget
		{
		public:
			UIWidget() = default;
			virtual ~UIWidget() = default;

			XPLICIT_COPY_DEFAULT(UIWidget);

			virtual const char* name() = 0;
			virtual void update() = 0;

		};
	}
}