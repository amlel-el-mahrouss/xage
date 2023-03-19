/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: DownloadProtocol.h
 *			Purpose: Xplicit download protocol
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
		PACKED_STRUCT
		(
			class XPLICIT_API XDPDownloadPacket
			{
			public:
				char magic[XPLICIT_TCP_MAG_COUNT];
				char version;
				bool compressed;

			};
		);
	}

	class XPLICIT_API XDPDownloadFile final
	{
	public:
		XDPDownloadFile();
		~XDPDownloadFile();

	public:
		XDPDownloadFile(char* bytes, size_t len);

		XDPDownloadFile& operator=(const XDPDownloadFile&) = default;
		XDPDownloadFile(const XDPDownloadFile&) = default;

	public:
		int set(char* bytes, size_t len);
		char* get() noexcept;
		size_t size() noexcept;

	private:
		std::vector<char> m_bytes;

	};

	class XPLICIT_API XDPDownloadTask final
	{
	public:
		XDPDownloadTask();
		~XDPDownloadTask();

		XDPDownloadTask& operator=(const XDPDownloadTask&) = delete;
		XDPDownloadTask(const XDPDownloadTask&) = delete;

		void add(XDPDownloadFile* file);
		bool remove(XDPDownloadFile* file);

		void operator()(Socket& socket, const bool compressed = false);

		operator bool() noexcept;
		void set(const bool ready = false) noexcept;

	private:
		std::vector<XDPDownloadFile*> m_files;
		bool m_ready;

	};
}