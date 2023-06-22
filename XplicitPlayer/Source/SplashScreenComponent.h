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
#include "GameMenuUI.h"

namespace Xplicit::Player
{
	class SplashScreenComponent final : public Component
	{
	public:
		SplashScreenComponent();
		~SplashScreenComponent() override;

		XPLICIT_COPY_DEFAULT(SplashScreenComponent);

		bool should_update() noexcept override { return mEnabled; }
		void update() override;
		void connect(const char* ip);

		// resets the timeout and run..
		// in case of a watchdog..
		void reset() noexcept;

	private:
		ITexture* mTexture; /* Texture to show when loading the game.. */
		NetworkComponent* mNetwork; /* Network instance */
		std::int64_t mTimeout; /* Network Timeout. */
		bool mEnabled; /* Should we seek for a connection? */

	};
}