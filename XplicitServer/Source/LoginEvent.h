/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "CommonInclude.h"
#include "HumanoidComponent.h"

namespace Xplicit
{
	class LoginEvent final : public Event
	{
	public:
		explicit LoginEvent();
		~LoginEvent() override;
		
	public:
		LoginEvent& operator=(const LoginEvent&) = default;
		LoginEvent(const LoginEvent&) = default;
		
	public:
		const char* name() noexcept override;
		void operator()() override;
		
	public:
		const size_t& size() const noexcept;

	private:
		void handle_leave_event() noexcept;
		void handle_join_event() noexcept;

	private:
		std::vector<HumanoidComponent*> mPlayers;
		NetworkServerComponent* mNetwork;
		std::size_t mPlayerCount;

	};
}