/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Client.h
 *			Purpose: Client Logic
 *
 * =====================================================================
 */

#pragma once

#include <ApplicationContext.h>
#include <NetworkInstance.h>

namespace Xplicit::Client
{
	class LoadingInstance : public Instance
	{
	public:
		LoadingInstance();
		virtual ~LoadingInstance();

		LoadingInstance& operator=(const LoadingInstance&) = default;
		LoadingInstance(const LoadingInstance&) = default;

		virtual void update() override;
		void connect(const char* ip);

		// resets the timeout and run..
		// in case of a watchdog..
		void reset() noexcept;

	private:
		irr::video::ITexture* m_logo_tex; /* Texture to show when loading the game.. */
		NetworkInstance* m_network; /* Network instance */
		int64_t m_timeout;
		bool m_run; /* Should we seek for a connection? */

	};
}