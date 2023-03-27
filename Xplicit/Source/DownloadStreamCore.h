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

#ifndef XPLICIT_STREAM_PORT
#define XPLICIT_STREAM_PORT (60002)
#endif // ifndef XPLICIT_STREAM_PORT

#define XPLICIT_STREAM_MAG_0 ('S')
#define XPLICIT_STREAM_MAG_1 ('T')
#define XPLICIT_STREAM_MAG_2 ('R')

#define XPLICIT_STREAM_MAG_COUNT (3U)
#define XPLICIT_STREAM_VERSION (3U)

namespace Xplicit::Network
{
	using ByteArray = std::vector<char>;

	class XPLICIT_API FileStream final
	{
	public:
		FileStream() = delete;

	public:
		FileStream(const char* bytes, size_t len);
		~FileStream();

		XPLICIT_COPY_DELETE(FileStream);

	public:
		const char* get() noexcept;
		size_t size() noexcept;

	private:
		int set(const char* bytes, size_t len);

	private:
		ByteArray m_byteList;

	};

	using FileStreamPtr = std::unique_ptr<FileStream>;

	class XPLICIT_API TaskStream final
	{
	public:
		TaskStream();
		~TaskStream();

		XPLICIT_COPY_DELETE(TaskStream);

	public:
		void add(FileStream* file);
		bool remove(FileStream* file);

	public:
		operator bool() noexcept;
		
		bool is_ready() noexcept;

		void set(const bool ready) noexcept;
		
		void operator()(Socket& socket, const bool isCompressed);

	private:
		std::vector<FileStream*> m_fileList;
		bool m_bReady;

	};
}
