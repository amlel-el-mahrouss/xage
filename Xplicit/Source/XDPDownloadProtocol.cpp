/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: XDPDownload.cpp
 *			Purpose: XDP download protocol
 *
 * =====================================================================
 */

#include "XDPDownloadProtocol.h"

namespace Xplicit::XDP
{
	XDPDownloadManager::XDPDownloadManager()
	{

	}

	XDPDownloadManager::~XDPDownloadManager()
	{

	}

	void XDPDownloadManager::set(char* file_bytes, size_t len)
	{
		if (!file_bytes)
			return;

		if (len < 1)
			return;

		m_bytes.clear();
		m_bytes.reserve(len);

		for (size_t i = 0; i < m_bytes.size(); ++i)
		{
			m_bytes[i] = file_bytes[i];
		}

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("[XDPDownloadManager] Added file to DownloadScheduler");
#endif
	}

	char* XDPDownloadManager::get() noexcept
	{
		XPLICIT_ASSERT(!m_bytes.empty());
		return m_bytes.data(); 
	}
}