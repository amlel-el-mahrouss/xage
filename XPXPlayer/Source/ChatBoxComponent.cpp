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
		mTextBox(platform_string("Type something...").c_str())
	{
		mChatFrame.X = 10;
		mChatFrame.Y = 30;

		mChatFrame.W = 105;
		mChatFrame.H = 58;

		mTextBox.set_size(103, 7);
		mTextBox.set_pos(11, mChatFrame.Y - 1);

		mChatFrame.BackgroundHoverColor = ChatBoxStyleDark::get_frame_color();
		mChatFrame.BackgroundColor = ChatBoxStyleDark::get_frame_color();
	}
	
	ChatBoxComponent::~ChatBoxComponent()
	{

	}

	void ChatBoxComponent::update(ClassPtr selfcls)
	{
		ChatBoxComponent* self = (ChatBoxComponent*)selfcls;

		if (self)
		{
			XPLICIT_ASSERT(IsValidHeapPtr(self));

			if (self->mNetwork->get().channel == XPLICIT_CHANNEL_CHAT)
			{
				NetworkPacketChat chat_pckt = *(NetworkPacketChat*)&self->mNetwork->get();

				String buffer;
				buffer.reserve(192);

				buffer += chat_pckt.username;
				buffer += ": ";
				buffer += chat_pckt.buffer;

				self->mChatQueue.push(buffer);
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