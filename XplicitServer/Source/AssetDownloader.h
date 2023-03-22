/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: AssetDownloader.h
 *			Purpose: Assets from the network
 *
 * =====================================================================
 */

#pragma once

#include "SDK.h"

namespace Xplicit::ContentSync
{
	class AssetEvent;
	class AssetListener;

	class AssetEvent final : public Event
	{
	public:
		AssetEvent() = default;
		virtual ~AssetEvent() = default;

		AssetEvent& operator=(const AssetEvent&) = default;
		AssetEvent(const AssetEvent&) = default;

		virtual const char* name() noexcept override;
		virtual void operator()() override;

	};

	class AssetListener final : public EventListener
	{
	public:
		AssetListener();

		virtual ~AssetListener() = default;

		AssetListener& operator=(const AssetListener&) = default;
		AssetListener(const AssetListener&) = default;

		virtual const char* name() noexcept override;
		virtual void update(EventTypePtr ptr_event) override;

		NetworkDownloadTask* get() noexcept;
		NetworkDownloadTask** address_of() noexcept;

	private:
		Xplicit::Details::Socket m_socket;
		NetworkDownloadTask* m_task;

		friend AssetEvent;

	};
}