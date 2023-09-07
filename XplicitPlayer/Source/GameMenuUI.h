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
#include <CommonEngine.h>
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
		LEAVE,
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
		BasicString<PChar> mText;
		BasicString<PChar> mTitle;
		ImGUI::UIFrame* mHudFrame;
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
		std::array<ImGUI::UIFrame, XPLICIT_MAX_ELEMENTS_INVENTORY> mInventorySlots;
		std::size_t mSelectedSlot;

		NetworkComponent* mNetwork;
		ImGUI::UIFrame* mHudFrame;

	private:
		std::int64_t mPublicHash;
		std::int64_t mHealth;

	};

	class LocalFrameComponent final : public Lua::CLuaClass
	{
	public:
		LocalFrameComponent(const char* parent, const char* name);
		~LocalFrameComponent() = default;

	public:
		XPLICIT_COPY_DEFAULT(LocalFrameComponent);

	public:
		const char* name();

	public:
		static void update(ClassPtr klass) noexcept;
		static bool should_update();

	private:
		ImGUI::UIFrame mFrame;

	};
}