/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "ChatBoxComponent.h"

namespace XPX
{
	ChatBoxComponent::ChatBoxComponent(const char* username)
		:
		mUsername(username),
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent")),
		mTextBox(platform_string("...").c_str())
	{
		mChatFrame.X = 10;
		mChatFrame.Y = 40;

		mChatFrame.W = 512;
		mChatFrame.H = 210;

		mTextBox.set_size(512, 30);
		mTextBox.set_pos(10, mChatFrame.Y + mChatFrame.H);

		mChatFrame.BackgroundHoverColor = ChatBoxStyleDark::get_frame_color();
		mChatFrame.BackgroundColor = ChatBoxStyleDark::get_frame_color();
	}
	
	ChatBoxComponent::~ChatBoxComponent() = default;

	bool ChatBoxComponent::is_typing() noexcept { return mTextBox.focus(); }

	void ChatBoxComponent::update(ClassPtr cls)
	{
		ChatBoxComponent* self = (ChatBoxComponent*)cls;

		if (self)
		{
			XPLICIT_ASSERT(IsValidHeapPtr(self));

			if (self->mNetwork->get().channel == XPLICIT_CHANNEL_CHAT)
			{
				NetworkPacketChat chat_pckt = *(NetworkPacketChat*)&self->mNetwork->get();

				register String chat_buffer = "";

				chat_buffer.reserve(192);

				chat_buffer += chat_pckt.username;
				chat_buffer += ": ";
				chat_buffer += chat_pckt.buffer;

				self->mChatQueue.push(chat_buffer);
			}

			self->mChatFrame.update(self->mChatFrame.BackgroundColor);
			self->mTextBox.update();
		}
	}

	bool ChatBoxComponent::should_update()
	{
		return true;
	}
}