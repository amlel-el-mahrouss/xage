/*
 * =====================================================================
 *
 *			XAGEPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Root.h>
#include <NMath.h>
#include <ImGUI.h>
#include <Enums.h>
#include <Component.h>
#include <NetworkComponent.h>

namespace XPX
{
	enum class POPUP_TYPE
	{
		NETWORK,
		CHALLENGE,
		BANNED,
		SHUTDOWN,
		LEAVE,
		KICK,
	};

	class MessageComponent final : public Component
	{
	public:
		MessageComponent() = delete;
		
	public:
		explicit MessageComponent(const std::function<void()>& on_click,
			const POPUP_TYPE shutdown_type = POPUP_TYPE::NETWORK, 
			const char* id = "POPUP", 
			const char* msg = "") noexcept;

		~MessageComponent() override;
		
	public:
		XPLICIT_COPY_DEFAULT(MessageComponent);
		
	public:
		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

		static bool should_update() noexcept;
		static void update(ClassPtr class_ptr);
		
	private:
		using FunctionPopup = std::function<void()>;

	private:
		BasicString<PChar> mTitle;
		BasicString<PChar> mText;

	private:
		ImGUI::UIFrame* mHudFrame;

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
		std::array<ImGUI::UIFrame, XPX_MAX_WEAPONS> mInventorySlots;
		std::size_t mSelectedSlot;

		NetworkComponent* mNetwork;
		ImGUI::UIFrame* mFrameParent;
		ImGUI::UIFrame* mHudFrame;

	private:
		std::int64_t mPublicHash;
		std::int64_t mHealth;

	};

	class ChatBoxStyleDark final
	{
	public:
		static ImGUI::ImColor get_frame_color() noexcept { return { 0x41, 0x41, 0x41, 150 }; }
		static ImGUI::ImColor get_text_rgn_color() noexcept { return { 0x1A, 0x1A, 0x1A, 150 }; }

	};
}