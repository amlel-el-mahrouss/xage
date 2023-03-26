/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: NetworkDownload.h
 *			Purpose: Xplicit Stream Protocol.
 *
 * =====================================================================
 */

#pragma once

#include "Crc32.h"
#include "Xplicit.h"

#include "SocketWrapper.h"
#include "NetworkProtocol.h"

#ifndef XPLICIT_PROTO_PORT
#define XPLICIT_PROTO_PORT (60002)
#endif // ifndef XPLICIT_PROTO_PORT

#define XPLICIT_PROTO_MAG_0 ('X')
#define XPLICIT_PROTO_MAG_1 ('S')
#define XPLICIT_PROTO_MAG_2 ('P')

#define XPLICIT_PROTO_MAG_COUNT (3U)
#define XPLICIT_PROTO_VERSION (2U)

namespace Xplicit::Network
{
	class XPLICIT_API StreamFile final
	{
	public:
		StreamFile() = delete;

	public:
		StreamFile(const char* bytes, size_t len);
		~StreamFile();

		XPLICIT_COPY_DELETE(StreamFile);

	public:
		int set(const char* bytes, size_t len);
		const char* get() noexcept;
		size_t size() noexcept;

	private:
		std::vector<char> m_bytes;

	};

	class XPLICIT_API DownloadTask final
	{
	public:
		DownloadTask();
		~DownloadTask();

		XPLICIT_COPY_DELETE(DownloadTask);

	public:
		void add(StreamFile* file);
		bool remove(StreamFile* file);

	public:
		operator bool() noexcept;
		bool is_ready() noexcept;
		void set(const bool ready) noexcept;
		void operator()(Socket& socket, const bool compressed);

	private:
		std::vector<StreamFile*> m_files;
		bool m_bReady;

	};
}
