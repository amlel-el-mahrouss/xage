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
	//! @brief Loading screen class; initialize your game components here, refer to 'on_join' or 'on_timeout' method.
	class LoadingScreenComponent final : public Component
	{
	public:
		explicit LoadingScreenComponent() noexcept;
		~LoadingScreenComponent() override;

	public:
		XPLICIT_COPY_DEFAULT(LoadingScreenComponent);

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

	public:
		//! define any game data here...
		void on_timeout() noexcept {}
		void on_join() noexcept {}

	private:
		NetworkComponent* mNetwork; //! network connection.
		std::int64_t mTimeout; //! timeout since we sent our first begin packet.

	private:
		static bool StartLoad;

	};
}