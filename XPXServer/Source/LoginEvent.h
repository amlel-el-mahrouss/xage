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
#include "CharacterComponent.h"

namespace XPX
{
	class LoginEvent final : public Event
	{
	public:
		LoginEvent();
		~LoginEvent() override;
		
	public:
		LoginEvent& operator=(const LoginEvent&) = default;
		LoginEvent(const LoginEvent&) = default;
		
	public:
		const char* name() noexcept override;
		void operator()() override;
		
	public:
		const size_t& size() const noexcept;

	public:
		void on_leave() noexcept;
		void on_join() noexcept;

	private:
		std::vector<CharacterComponent*> mPlayers;
		NetworkServerComponent* mNetwork;
		std::size_t mPlayerCount;

	};
}