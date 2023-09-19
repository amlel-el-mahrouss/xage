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