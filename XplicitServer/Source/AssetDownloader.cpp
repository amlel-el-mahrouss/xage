/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: AssetDownloader.cpp
 *			Purpose: Asset from the network
 *
 * =====================================================================
 */

#include "AssetDownloader.h"

/**
* @file
*/

namespace Xplicit::ContentSync
{
	void AssetEvent::operator()() {}

	const char* AssetEvent::name() noexcept { return ("AssetEvent"); }
	
	AssetListener::AssetListener(NetworkDownloadTask* pTask)
		: m_socket(Xplicit::Details::SOCKET_TYPE::TCP), m_task(pTask)
	{
		XPLICIT_ASSERT(m_socket);
		XPLICIT_ASSERT(m_task);
	}

	NetworkDownloadTask* AssetListener::get() noexcept { return m_task; }
	NetworkDownloadTask** AssetListener::address_of() noexcept { return &m_task; }

	const char* AssetListener::name() noexcept { return ("AssetListener"); }
	
	void AssetListener::update(EventTypePtr ptr_event)
	{
		(void)ptr_event;

		if (!this->get())
			return;

		if (this->get()->is_ready())
		{
			(*this->get())(m_socket._Socket);
			delete this->get();
		}
	}
}