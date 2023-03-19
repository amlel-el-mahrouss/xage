/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: XDPDownloader.cpp
 *			Purpose: XDP Downloader, includes a Event &&
 *			Listener.
 *
 * =====================================================================
 */

#include "XDP.h"

/**
* @file
*/

namespace Xplicit::XDP
{
	void XDPEvent::operator()() {}

	const char* XDPEvent::name() noexcept { return ("XDPEvent"); }
	
	XDPListener::XDPListener()
		: m_socket(Xplicit::Details::SOCKET_TYPE::TCP)
	{
		XPLICIT_ASSERT(m_socket);
	}

	const char* XDPListener::name() noexcept { return ("XDPListener"); }
	
	void XDPListener::update(EventTypePtr ptr_event)
	{
		if (this->get().is_ready())
			this->get()(this->m_socket._Socket);
	}
}