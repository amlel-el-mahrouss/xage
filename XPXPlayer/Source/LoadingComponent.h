/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <NetworkComponent.h>
#include <ImGUI.h>
#include <Root.h>
#include <Uri.h>

namespace XPX
{
	class LoadingComponent final : public Component
	{
	public:
		explicit LoadingComponent() noexcept;
		~LoadingComponent() override;

	public:
		XPLICIT_COPY_DEFAULT(LoadingComponent);

	public:
		static bool should_update() noexcept { return StartLoad; }
		static void update(void* class_ptr);

	public:
		/// <summary>
		/// Connects with specific ip:port
		/// </summary>
		/// <param name="ip">the uri containing the ip;</param>
		void connect(Utils::UriParser& ip);

		/// <summary>
		/// reset connection, removes network component.
		/// </summary>
		void reset() noexcept;

	private:
		ITexture* mLoadingTexture;
		NetworkComponent* mNetwork;
		std::int64_t mTimeout; /* Network Timeout, incremented on each dial failure. */

	private:
		static bool StartLoad; /* Should we seek for a connection? */

	};
}