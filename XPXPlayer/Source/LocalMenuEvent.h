/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <NetworkComponent.h>
#include <Event.h>
#include <Root.h>

#include "UserInterface.h"

namespace XPX
{
	class LocalMenuEvent final : public Event
	{
	public:
		explicit LocalMenuEvent() noexcept;
		~LocalMenuEvent() override;

		LocalMenuEvent& operator=(const LocalMenuEvent&) = default;
		LocalMenuEvent(const LocalMenuEvent&) = default;

	public:
		void enable(const bool enable = false) noexcept;
		bool enabled() const noexcept;

	public:
		void operator()() override;
		const char* name() noexcept;

	private:
		NetworkComponent* mNetwork;
		PopupComponent* mPopup;

	private:
		std::int64_t mTimeout;
		bool mEnabled;

	};
}