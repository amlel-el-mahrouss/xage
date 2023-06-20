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
		virtual ~PlayerTimeoutEvent();

	public:
		virtual const char* name() noexcept override;
		virtual void operator()() override;

	public:
		XPLICIT_COPY_DEFAULT(PlayerTimeoutEvent);

	private:
		NetworkServerComponent* mNetwork;
		std::int32_t mCounter;

	private:
		static const int32_t cycles = 1000;

	};
}