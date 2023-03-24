/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: DownloadSingleton.h
 *			Purpose: CDP Singleton
 *
 * =====================================================================
 */

#pragma once

#include <SocketWrapper.h>
#include <NetworkDownload.h>

namespace Xplicit::Network
{
	class DownloadSingleton final
	{
	private:
		DownloadSingleton();

	public:
		~DownloadSingleton();

		XPLICIT_COPY_DELETE(DownloadSingleton);

	public:
		static DownloadSingleton* get_singleton_ptr();

	private:
		std::unique_ptr<Socket> m_socket;

	};
}