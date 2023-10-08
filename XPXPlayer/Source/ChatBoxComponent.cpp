/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "ChatBoxComponent.h"

#define XPX_CHAT_MESSAGE_SIZE (192)

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
		mChatFrame.H = 310;

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

		if (self && 
			IsValidHeapPtr(self))
		{
			if (self->mNetwork->get().channel == XPLICIT_CHANNEL_CHAT &&
				self->mNetwork->get().cmd[XPLICIT_NETWORK_CMD_CHAT] == NETWORK_CMD_CHAT)
			{
				self->mNetwork->get().cmd[XPLICIT_NETWORK_CMD_CHAT] = NETWORK_CMD_INVALID;

				NetworkPacket packet_recv_message = self->mNetwork->get();

				register String chat_buffer = "";

				chat_buffer.reserve(XPX_CHAT_MESSAGE_SIZE);

				for (size_t i = 0; i < XPX_CHAT_MESSAGE_SIZE; ++i)
				{
					chat_buffer[i] = packet_recv_message.additional_data[i];
				}

				self->mChatQueue.push(chat_buffer);
			}

			self->mChatFrame.update(self->mChatFrame.BackgroundColor);
			self->mTextBox.update();

			if (KEYBOARD->key_down(KEY_RETURN))
			{
				NetworkPacket packet_send_msg{};

				packet_send_msg.channel = XPLICIT_CHANNEL_CHAT;
				packet_send_msg.cmd[XPLICIT_NETWORK_CMD_CHAT] = NETWORK_CMD_CHAT;

				memcpy(packet_send_msg.additional_data, self->mTextBox.get_text().data(), self->mTextBox.get_text().size());
			}
		}
	}

	bool ChatBoxComponent::should_update()
	{
		return true;
	}
}