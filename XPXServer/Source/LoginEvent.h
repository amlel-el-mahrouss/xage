/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ServerConfig.h"
#include "HumanoidComponent.h"

namespace XPX
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
		void HandleLeave() noexcept;
		void HandleJoin() noexcept;

	private:
		std::vector<HumanoidComponent*> mPlayers;
		NetworkServerComponent* mNetwork;
		std::size_t mPlayerCount;

	};
}