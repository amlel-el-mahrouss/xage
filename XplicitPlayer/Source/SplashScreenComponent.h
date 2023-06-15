/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: LoadingComponent.h
 *			Purpose: Loading Component
 *
 * =====================================================================
 */

#pragma once

#include <NetworkComponent.h>

#include "CoreUI.h"

namespace Xplicit::Player
{
	class SplashScreenComponent final : public Component
	{
	public:
		SplashScreenComponent();
		virtual ~SplashScreenComponent();

		SplashScreenComponent& operator=(const SplashScreenComponent&) = default;
		SplashScreenComponent(const SplashScreenComponent&) = default;

		virtual bool should_update() noexcept override { return mEnable; }
		virtual void update() override;
		void connect(const char* ip);

		// resets the timeout and run..
		// in case of a watchdog..
		void reset() noexcept;

	private:
		irr::video::ITexture* mTexture; /* Texture to show when loading the game.. */
		NetworkComponent* mNetwork; /* Network instance */

	private:
		int64_t mTimeout; /* Network Timeout. */
		bool mEnable; /* Should we seek for a connection? */

	};
}