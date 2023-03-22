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

#define XPLICIT_TCP_MAG_0 ('X')
#define XPLICIT_TCP_MAG_1 ('D')

#define XPLICIT_TCP_MAG_COUNT (2U)
#define XPLICIT_TCP_VERSION (1)

 /// <summary>
 /// This namespace contains everything from file downloading, to replication.
 /// We mainly use that to send assets to the player.
 /// </summary>
namespace Xplicit::ContentSync
{
	namespace Details
	{
		PACKED_STRUCT
		(
			class XPLICIT_API DownloadPacket
			{
			public:
				char magic[XPLICIT_TCP_MAG_COUNT];
				char version;
				bool compressed;

			};
		);
	}

	/// <summary>
	/// A file shared on the network.
	/// </summary>
	class XPLICIT_API NetworkSharedFile final
	{
	public:
		NetworkSharedFile() = delete;

	public:
		~NetworkSharedFile();

	public:
		NetworkSharedFile(char* bytes, size_t len);

		XPLICIT_COPY_DEFAULT(NetworkSharedFile);

	public:
		int set(char* bytes, size_t len);
		size_t size() noexcept;
		char* get() noexcept;

	private:
		std::vector<char> m_bytes;

	};

	class XPLICIT_API NetworkDownloadTask final
	{
	public:
		NetworkDownloadTask();
		~NetworkDownloadTask();

		XPLICIT_COPY_DELETE(NetworkDownloadTask);

		void add(NetworkSharedFile* file);
		bool remove(NetworkSharedFile* file);

		operator bool() noexcept;
		bool is_ready() noexcept;
		void set(const bool ready = false) noexcept;
		void operator()(Socket& socket, const bool compressed = false);

	private:
		std::vector<NetworkSharedFile*> m_files;
		bool m_ready;

	};
}