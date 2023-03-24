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

		XPLICIT_COPY_DELETE(AssetEvent);

		virtual const char* name() noexcept override;
		virtual void operator()() override;

	};

	class AssetListener final : public EventListener
	{
	public:
		AssetListener(NetworkDownloadTask* pTask);

		virtual ~AssetListener() = default;

		XPLICIT_COPY_DELETE(AssetListener);

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