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
	
	XDPListener::XDPListener(Socket listeningSocket)
		: m_socket(listeningSocket)
	{

	}

	const char* XDPListener::name() noexcept { return ("XDPListener"); }
	
	void XDPListener::update(EventTypePtr ptr_event)
	{
		std::jthread thrd([&]()-> void {
			if (this->get())
				this->get()(this->m_socket);
		});

		thrd.detach();
	}
}