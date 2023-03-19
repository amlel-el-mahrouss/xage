/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: XDPDownloader.h
 *			Purpose: XDP Downloader, includes a Event &&
 *			Listener.
 *
 * =====================================================================
 */

#pragma once

#include "SDK.h"

namespace Xplicit::XDP
{
	class XDPEvent;
	class XDPListener;

	class XDPEvent final : public Event
	{
	public:
		XDPEvent() = default;
		virtual ~XDPEvent() = default;

		XDPEvent& operator=(const XDPEvent&) = default;
		XDPEvent(const XDPEvent&) = default;

		virtual const char* name() noexcept override;
		virtual void operator()() override;

	};


	class XDPListener final : public EventListener
	{
	public:
		XDPListener(Socket listeningSocket);

		virtual ~XDPListener() = default;

		XDPListener& operator=(const XDPListener&) = default;
		XDPListener(const XDPListener&) = default;

		virtual const char* name() noexcept override;
		virtual void update(EventTypePtr ptr_event) override;

		XDPDownloadTask& get() { return m_task; }

	private:
		XDPDownloadTask m_task;
		Socket m_socket;

		friend XDPEvent;

	};
}