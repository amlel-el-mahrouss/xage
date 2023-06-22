/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#pragma once

#include "CommonInclude.h"

namespace Xplicit
{
	class PlayerTimeoutEvent final : public Event
	{
	public:
		explicit PlayerTimeoutEvent();
		~PlayerTimeoutEvent() override;

		const char* name() noexcept override;
		void operator()() override;

		XPLICIT_COPY_DEFAULT(PlayerTimeoutEvent);

	private:
		NetworkServerComponent* mNetwork;
		std::int32_t mCounter;

	};
}