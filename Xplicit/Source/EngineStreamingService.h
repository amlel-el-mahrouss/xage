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
#define XPLICIT_STREAM_PORT (2305)
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

	class XPLICIT_API EngineStreamWriter final
	{
	public:
		EngineStreamWriter() = delete;

	public:
		EngineStreamWriter(const char* bytes, size_t len);
		~EngineStreamWriter();

	public:
		XPLICIT_COPY_DELETE(EngineStreamWriter);

	public:
		const char* get() noexcept;
		size_t size() noexcept;

	private:
		int write(const char* bytes, size_t len);

	private:
		ByteArray mByteList;

	};

	using FileStreamPtr = std::unique_ptr<EngineStreamWriter>;

	class XPLICIT_API EngineNetworkStream final
	{
	public:
		EngineNetworkStream();
		~EngineNetworkStream();

	public:
		XPLICIT_COPY_DELETE(EngineNetworkStream);

	public:
		bool remove(EngineStreamWriter* fsw);
		void add(EngineStreamWriter* fsw);

	public:
		void operator()(Socket& socket, const bool isCompressed);
		operator bool() noexcept;
		
	public:
		void set(const bool ready) noexcept;
		bool ready() noexcept;
		
	private:
		std::vector<EngineStreamWriter*> mFileList;

	private:
		bool mReady;

	};
}
