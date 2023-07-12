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
#include <ImGUI.h>
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
		CHALLENGE,
		TELEPORTING,
		BANNED,
		SHUTDOWN,
		KICK,
	};

	class PopupComponent final : public Component
	{
	public:
		PopupComponent() = delete;
		
	public:
		explicit PopupComponent(const std::function<void()>& on_click,
			const POPUP_TYPE shutdown_type = POPUP_TYPE::NETWORK, const char* id = "POPUP") noexcept;

		~PopupComponent() override;
		
		PopupComponent& operator=(const PopupComponent&) = default;
		PopupComponent(const PopupComponent&) = default;
		
		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

		static bool should_update() { return true;  }
		static void update(void* class_ptr);
		
	private:
		using FunctionPopup = std::function<void()>;

	private:
		ImGUI::UIFrame* mHudFrame;
		BasicString<wchar_t> mText;
		ImGUI::UIButton mCancel;
		ImGUI::UIButton mOk;

	private:
		FunctionPopup mClicked;
		String mPopupId;

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
		
		static void update(void* class_ptr);
		static bool should_update() { return true; }

	private:
		NetworkComponent* mNetwork;
		ImGUI::UIFrame* mHudFrame;

	private:
		std::int64_t mPublicHash;
		std::int64_t mHealth;

	};
}