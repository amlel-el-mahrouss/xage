/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <NetworkComponent.h>
#include <Event.h>

#include "GWEN.h"

namespace Xplicit::Player
{
	class XPLICIT_API LocalMenuEvent final : public Event
	{
	public:
		LocalMenuEvent() = delete;

	public:
		explicit LocalMenuEvent(Gwk::Controls::Canvas* pCanvas) noexcept;
		~LocalMenuEvent() override;

		LocalMenuEvent& operator=(const LocalMenuEvent&) = default;
		LocalMenuEvent(const LocalMenuEvent&) = default;

	public:
		void enable(const bool enable = false) noexcept;
		bool enabled() const noexcept;

	public:
		void operator()() override;
		const char* name() noexcept;

	public:
		bool in_chat() const noexcept;

	private:
		Gwk::Controls::TextBoxMultiline* mChatBox;
		Gwk::Controls::TextBox* mInputChat;
		
	public:
		NetworkComponent* mNetwork;

	private:
		ITexture* mButtonNoHover;
		ITexture* mButtonHover;
		ITexture* mMenu;

	private:
		std::int64_t mTimeout;
		bool mEnabled;

	};
}