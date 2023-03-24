/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: DownloadProtocol.cpp
 *			Purpose: Xplicit asset protocol
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "NetworkDownload.h"
#include "Crc32.h"

namespace Xplicit::ContentSync
{
	NetworkSharedFile::NetworkSharedFile(char* bytes, size_t len)
	{
		this->set(bytes, len);
	}

	NetworkSharedFile::~NetworkSharedFile() {}

	int NetworkSharedFile::set(char* bytes, size_t len)
	{
		if (!bytes)
			return -1;

		if (len < 1)
			return -2;

		m_arrBytes.clear();
		m_arrBytes.reserve(len);

		for (size_t i = 0; i < m_arrBytes.size(); ++i)
		{
			m_arrBytes[i] = bytes[i];
		}

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("[XDPDownloadFile::set] Call returned 0.");
#endif

		return 0;
	}

	char* NetworkSharedFile::get() noexcept
	{
		XPLICIT_ASSERT(!m_arrBytes.empty());
		return m_arrBytes.data();
	}

	size_t NetworkSharedFile::size() noexcept { return m_arrBytes.size(); }

	NetworkDownloadTask::NetworkDownloadTask()
		: m_bReady(false), m_vFiles()
	{
	
	}

	NetworkDownloadTask::~NetworkDownloadTask()
	{

	}

	void NetworkDownloadTask::add(NetworkSharedFile* file)
	{
		if (file)
			m_vFiles.push_back(file);
	}

	bool NetworkDownloadTask::remove(NetworkSharedFile* file)
	{
		if (file)
		{
			auto it = std::find_if(m_vFiles.cbegin(), m_vFiles.cend(),
			[&](NetworkSharedFile* ptr) -> bool
			{
				return file == ptr;
			});

			if (it != m_vFiles.cend())
				return true;
		}

		return false;
	}

	void NetworkDownloadTask::operator()(Socket& socket, const bool compressed)
	{
		if (socket != SOCKET_ERROR)
		{
			std::vector<int> data, data_tmp;

			data.push_back(XPLICIT_TCP_MAG_0);
			data.push_back(XPLICIT_TCP_MAG_1);
			data.push_back(XPLICIT_TCP_VERSION);

			data.push_back(compressed);

			std::jthread worker = std::jthread([&]() -> void
				{
					for (size_t i = 0; i < m_vFiles.size(); ++i)
					{
						for (size_t y = 0; y < m_vFiles[y]->size(); ++y)
						{
							data_tmp.push_back(m_vFiles[i]->get()[y]);
						}

						// add these 5 characters, to separate each archives.

						data.push_back('!');
						data.push_back('<');
						data.push_back('A');
						data.push_back('R');
						data.push_back('>');
					}
			});

			// wait for the worker thread to finish.
			worker.join();

			int crc = xplicit_crc32(reinterpret_cast<char*>(data_tmp.data()), data_tmp.size());
			data.push_back(crc);

			for (size_t i = 0; i < data_tmp.size(); ++i)
			{
				data.push_back(data_tmp[i]);
			}

			::send(socket, reinterpret_cast<char*>(data_tmp.data()), data.size(), 0);

			/* Set status as non-ready. */
			m_bReady = false;
		}
	}

	bool NetworkDownloadTask::is_ready() noexcept { return m_bReady; }

	NetworkDownloadTask::operator bool() noexcept { return this->is_ready(); }

	void NetworkDownloadTask::set(const bool ready) noexcept { m_bReady = true; }
}