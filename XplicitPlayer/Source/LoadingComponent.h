/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
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
	class LoadingComponent final : public Component
	{
	public:
		LoadingComponent();
		virtual ~LoadingComponent();

		LoadingComponent& operator=(const LoadingComponent&) = default;
		LoadingComponent(const LoadingComponent&) = default;

		virtual bool should_update() noexcept override { return mEnable; }
		virtual void update() override;
		void connect(const char* ip);

		// resets the timeout and run..
		// in case of a watchdog..
		void reset() noexcept;

	private:
		irr::video::ITexture* m_texture; /* Texture to show when loading the game.. */
		NetworkComponent* mNetwork; /* Network instance */

	private:
		int64_t m_timeout; /* Network Timeout. */
		bool mEnable; /* Should we seek for a connection? */

	};
}