/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: XDPDownloadProtocol.h
 *			Purpose: XDP download protocol
 *
 * =====================================================================
 */

#pragma once

#include "Crc32.h"
#include "Xplicit.h"
#include "NetworkProtocol.h"

#ifndef XPLICIT_TCP_PORT
#define XPLICIT_TCP_PORT (60002)
#endif // ifndef XPLICIT_TCP_PORT

#define XPLICIT_TCP_MAG_0 ('X')
#define XPLICIT_TCP_MAG_1 ('D')

#define XPLICIT_TCP_MAG_COUNT (2U)
#define XPLICIT_TCP_VERSION (1)

namespace Xplicit::XDP
{
	namespace Details
	{
		class XPLICIT_API XDPDownloadPacket
		{
		public:
			char magic[XPLICIT_TCP_MAG_COUNT];

		public:
			int32_t file_size;
			int32_t version;
			int32_t crc32;

		};
	}

	class XPLICIT_API XDPDownloadManager final
	{
	public:
		XDPDownloadManager();
		~XDPDownloadManager();

		XDPDownloadManager& operator=(const XDPDownloadManager&) = default;
		XDPDownloadManager(const XDPDownloadManager&) = default;

	public:
		void set(char* file_bytes, size_t len);
		char* get() noexcept;

	private:
		std::vector<char> m_bytes;

	};
}