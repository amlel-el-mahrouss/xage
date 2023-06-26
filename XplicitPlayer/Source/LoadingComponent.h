/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <NetworkComponent.h>
#include <Uri.h>

#include "GameMenuUI.h"

namespace Xplicit::Player
{
	class LoadingComponent final : public Component
	{
	public:
		LoadingComponent();
		~LoadingComponent() override;

	public:
		XPLICIT_COPY_DEFAULT(LoadingComponent);

		bool should_update() noexcept override { return mEnabled; }
		void update() override;
		void connect(Utils::UriParser& ip);

		// resets the timeout and run..
		// in case of a watchdog..
		void reset() noexcept;

	private:
		ITexture* mTexture;
		NetworkComponent* mNetwork;

		std::int64_t mTimeout; /* Network Timeout, incremented on each connection failure. */
		bool mEnabled; /* Should we seek for a connection? */

	};
}