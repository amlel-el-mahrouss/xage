/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Root.h>
#include <NMath.h>
#include <Component.h>
#include <NetworkComponent.h>

namespace Xplicit::Player
{
	extern Vector<float> XPLICIT_DIM;
}

namespace Xplicit::Player
{
	enum class POPUP_TYPE
	{
		NETWORK,
		TELEPORTING,
		BANNED,
		SHUTDOWN,
		KICK,
	};

	class PopupComponent final : public Component
	{
	public:
		PopupComponent() = delete;
		
		PopupComponent(const std::function<void()>& on_click, const vector2di pos = vector2di(0, 0), 
			const POPUP_TYPE shutdown_type = POPUP_TYPE::NETWORK, const char* id = "EnginePopup") noexcept;

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

		const char* name() noexcept override { return "LocalHudComponent"; }
		COMPONENT_TYPE type() noexcept override { return COMPONENT_GUI; }
		
		void update() override;

	private:
		NetworkComponent* mNetwork;
		ITexture* mOverlay;

	private:
		std::int64_t mPublicHash;
		std::int64_t mTimeout;
		std::int64_t mHealth;

	};
}