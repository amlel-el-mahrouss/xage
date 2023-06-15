/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
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

#define XPLICIT_STREAM_MAG_0 ('D')
#define XPLICIT_STREAM_MAG_1 ('S')
#define XPLICIT_STREAM_MAG_2 ('C')

#define XPLICIT_STREAM_MAG_COUNT (3U)
#define XPLICIT_STREAM_VERSION (4U)

/* 
 * This file handles resource streaming for the xplicit game engine.
 * When a user asks for an item (mesh, sound, material...)
 * DSC downloads it.
 */

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
		ByteArray mByteList;

	};

	using FileStreamPtr = std::unique_ptr<FileStream>;

	class XPLICIT_API FileTaskStream final
	{
	public:
		FileTaskStream();
		~FileTaskStream();

		XPLICIT_COPY_DELETE(FileTaskStream);

	public:
		void add(FileStream* file);
		bool remove(FileStream* file);

	public:
		void operator()(Socket& socket, const bool isCompressed);
		operator bool() noexcept;
		
	public:
		void set(const bool ready) noexcept;
		bool ready() noexcept;
		
	private:
		std::vector<FileStream*> mFileList;
		bool mReady;

	};
}
