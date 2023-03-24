/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: NetworkDownload.h
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

#define XPLICIT_TCP_MAG_0 ('C')
#define XPLICIT_TCP_MAG_1 ('D')
#define XPLICIT_TCP_MAG_2 ('P')

#define XPLICIT_TCP_MAG_COUNT (3U)
#define XPLICIT_TCP_VERSION (2)

 /// <summary>
 /// Content Delivery namespace, used to send files over TCP.
 /// </summary>
 
namespace Xplicit::Network
{
	namespace Details
	{
		class XPLICIT_API NetworkFilePacket
		{
		public:
			char8_t magic[XPLICIT_TCP_MAG_COUNT];
			char8_t compress;
			char8_t version;

			int32_t crc32;

		};
	}

	/// <summary>
	/// A file sent over the network
	/// </summary>
	class XPLICIT_API NetworkSharedFile final
	{
	public:
		NetworkSharedFile() = delete;

	public:
		NetworkSharedFile(const char* bytes, size_t len);
		~NetworkSharedFile();

		XPLICIT_COPY_DELETE(NetworkSharedFile);

	public:
		int set(const char* bytes, size_t len);
		const char* get() noexcept;
		size_t size() noexcept;

	private:
		std::vector<char> m_bytes;

	};

	class XPLICIT_API NetworkDownloadTask final
	{
	public:
		NetworkDownloadTask();
		~NetworkDownloadTask();

		XPLICIT_COPY_DELETE(NetworkDownloadTask);

	public:
		void add(NetworkSharedFile* file);
		bool remove(NetworkSharedFile* file);

	public:
		operator bool() noexcept;
		bool is_ready() noexcept;

		void set(const bool ready) noexcept;
		void operator()(Socket& socket, const bool compressed);

	private:
		std::vector<NetworkSharedFile*> m_files;
		bool m_bReady;

	};
}
