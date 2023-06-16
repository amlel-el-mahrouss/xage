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

#define XPLICIT_STREAM_MAG_0 ('X')
#define XPLICIT_STREAM_MAG_1 ('S')
#define XPLICIT_STREAM_MAG_2 ('P')

#define XPLICIT_STREAM_MAG_COUNT (3U)
#define XPLICIT_STREAM_VERSION   (5U)

/* 
 * This file handles resource streaming for the Xplicit game engine.
 * When a user asks for an item (mesh, sound, material...)
 * DSC downloads it.
 */

namespace Xplicit::Network
{
	using ByteArray = std::vector<char>;

	class XPLICIT_API FileStreamWriter final
	{
	public:
		FileStreamWriter() = delete;

	public:
		FileStreamWriter(const char* bytes, size_t len);
		~FileStreamWriter();

		XPLICIT_COPY_DELETE(FileStreamWriter);

	public:
		const char* get() noexcept;
		size_t size() noexcept;

	private:
		int write(const char* bytes, size_t len);

	private:
		ByteArray mByteList;

	};

	using FileStreamPtr = std::unique_ptr<FileStreamWriter>;

	class XPLICIT_API FileTaskStream final
	{
	public:
		FileTaskStream();
		~FileTaskStream();

	public:
		XPLICIT_COPY_DELETE(FileTaskStream);

	public:
		bool remove(FileStreamWriter* fsw);
		void add(FileStreamWriter* fsw);

	public:
		void operator()(Socket& socket, const bool isCompressed);
		operator bool() noexcept;
		
	public:
		void set(const bool ready) noexcept;
		bool ready() noexcept;
		
	private:
		std::vector<FileStreamWriter*> mFileList;

	private:
		bool mReady;

	};
}
