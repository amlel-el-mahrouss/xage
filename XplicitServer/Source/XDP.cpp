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
	
	XDPListener::XDPListener(Socket socket)
		: m_socket(socket)
	{
		XPLICIT_ASSERT(socket != SOCKET_ERROR);
	}

	const char* XDPListener::name() noexcept { return ("XDPListener"); }
	
	void XDPListener::update(EventTypePtr ptr_event)
	{
		std::thread thrd([&]()-> void {
			if (this->get().is_ready())
				this->get()(this->m_socket);
		});

		thrd.detach();
	}
}