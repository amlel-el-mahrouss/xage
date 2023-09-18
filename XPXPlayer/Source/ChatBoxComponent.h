/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "MenuUI.h"

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

		static bool should_update();
		static void update(ClassPtr selfcls);

	private:
		std::queue<String> mChatQueue;
		NetworkComponent* mNetwork;
		ImGUI::UIFrame mChatFrame;
		ImGUI::UIEditBox mTextBox;
		String mUsername;

	};
}