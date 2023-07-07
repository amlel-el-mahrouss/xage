/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <NetworkComponent.h>
#include <Root.h>
#include <Uri.h>

#include "GameMenuUI.h"

namespace Xplicit::Player
{
	class LoadingComponent final : public Component
	{
	public:
		explicit LoadingComponent();
		~LoadingComponent() override;

	public:
		XPLICIT_COPY_DEFAULT(LoadingComponent);

	public:
		bool should_update() noexcept override { return mEnabled; }
		void update() override;

	public:
		// starts to connect to specified IP:PORT.
		void connect(Utils::UriParser& ip);

		// resets the timeout and run..
		// in case of a watchdog..
		void reset() noexcept;

	private:
		irr::video::ITexture* mTexture;

	private:
		NetworkComponent* mNetwork;
		std::int64_t mTimeout; /* Network Timeout, incremented on each connection failure. */
		bool mEnabled; /* Should we seek for a connection? */

	};
}