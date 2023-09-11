/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
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

namespace XPX
{
	extern Vector<float> XPLICIT_DIM;
}

namespace XPX
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
			const POPUP_TYPE shutdown_type = POPUP_TYPE::NETWORK, const char* id = "POPUP", const char* msg = "You have been kicked.") noexcept;

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

	class HUDComponent final : public Component
	{
	public:
		HUDComponent() = delete;
		
		explicit HUDComponent(const std::int64_t& publicHash);
		~HUDComponent() override;
		
		HUDComponent& operator=(const HUDComponent&) = default;
		HUDComponent(const HUDComponent&) = default;

		const char* name() noexcept override { return "HUDComponent"; }
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

	class RectComponent final : public Lua::CLuaClass
	{
	public:
		RectComponent(const char* parent, const char* name);
		~RectComponent() = default;

	public:
		XPLICIT_COPY_DEFAULT(RectComponent);

	public:
		const char* name();

	public:
		static void update(ClassPtr klass) noexcept;
		static bool should_update();

	private:
		ImGUI::UIFrame mFrame;

	};
}