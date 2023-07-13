/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#pragma once

#include "ServerConfig.h"

namespace Xplicit
{
	class TimeoutEvent final : public Event
	{
	public:
		explicit TimeoutEvent();
		~TimeoutEvent() override;

		const char* name() noexcept override;
		void operator()() override;

		XPLICIT_COPY_DEFAULT(TimeoutEvent);

	private:
		NetworkServerComponent* mNetwork;
		std::int32_t mCounter;

	};
}