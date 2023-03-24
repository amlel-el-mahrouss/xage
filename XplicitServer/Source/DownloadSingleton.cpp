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

#include "DownloadSingleton.h"

/**
* @file
*/

namespace Xplicit::Network
{
	DownloadSingleton::DownloadSingleton()
		: m_socket(std::make_unique<Socket>(SOCKET_TYPE::TCP))
	{
		XPLICIT_ASSERT(m_socket);
	}

	DownloadSingleton::~DownloadSingleton()
	{

	}

	DownloadSingleton* DownloadSingleton::get_singleton_ptr()
	{
		DownloadSingleton* singleton = nullptr;

		if (!singleton)
			singleton = new DownloadSingleton();

		return singleton;
	}
}