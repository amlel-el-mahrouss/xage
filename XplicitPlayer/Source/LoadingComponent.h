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

#include <ApplicationContext.h>
#include <NetworkComponent.h>

namespace Xplicit::Client
{
	class LoadingComponent final : public Component
	{
	public:
		LoadingComponent();
		virtual ~LoadingComponent();

		LoadingComponent& operator=(const LoadingComponent&) = default;
		LoadingComponent(const LoadingComponent&) = default;

		virtual bool should_update() noexcept override { return m_run; }
		virtual void update() override;
		void connect(const char* ip);

		// resets the timeout and run..
		// in case of a watchdog..
		void reset() noexcept;

	private:
		irr::video::ITexture* m_texture; /* Texture to show when loading the game.. */
		NetworkComponent* m_network; /* Network instance */
		int64_t m_timeout; /* Network Timeout. */
		bool m_run; /* Should we seek for a connection? */

	};
}