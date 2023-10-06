/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "GameUI.h"

#include <NetworkComponent.h>
#include <queue>

namespace XPX
{
	class ChatBoxComponent final : public Component
	{
	public:
		ChatBoxComponent(const char* username);
		~ChatBoxComponent();

	public:
		const char* name() noexcept override { return "ChatBoxComponent"; }

	public:
		static void update(ClassPtr self);
		static bool should_update();

	public:
		bool is_typing() noexcept;

	private:
		std::queue<String> mChatQueue;
		NetworkComponent* mNetwork;
		ImGUI::UIFrame mChatFrame;
		ImGUI::UIEditBox mTextBox;
		String mUsername;

	};
}