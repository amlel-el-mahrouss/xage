/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: DownloadProtocol.cpp
 *			Purpose: Xplicit Stream Protocol
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "DownloadStreamCore.h"
#include "Crc32.h"

namespace Xplicit::Network
{
	StreamFile::StreamFile(const char* bytes, size_t len)
	{
		this->set(bytes, len);
	}

	StreamFile::~StreamFile() {}

	int StreamFile::set(const char* bytes, size_t len)
	{
		if (!bytes ||
			len < 1)
			return 1;

		m_bytes.clear();
		m_bytes.reserve(len);

		for (size_t i = 0; i < m_bytes.size(); ++i)
		{
			m_bytes[i] = bytes[i];
		}

		return 0;
	}

	const char* StreamFile::get() noexcept
	{
		if (m_bytes.empty())
			return "(null)";

		return m_bytes.data();
	}

	size_t StreamFile::size() noexcept { return m_bytes.size(); }

	DownloadTask::DownloadTask()
		: m_bReady(false), m_files()
	{
	
	}

	DownloadTask::~DownloadTask()
	{

	}

	void DownloadTask::add(StreamFile* file)
	{
		if (file)
			m_files.push_back(file);
	}

	bool DownloadTask::remove(StreamFile* file)
	{
		if (file)
		{
			auto it = std::find(m_files.cbegin(), m_files.cend(), file);

			if (it != m_files.cend())
			{
				m_files.erase(it);
				return true;
			}
		}

		return false;
	}

	void DownloadTask::operator()(Socket& socket, const bool compressed)
	{
		if (socket != SOCKET_ERROR)
		{
			std::vector<int> data, data_tmp;

			data.push_back(XPLICIT_PROTO_MAG_0);
			data.push_back(XPLICIT_PROTO_MAG_1);
			data.push_back(XPLICIT_PROTO_MAG_2);
			data.push_back(compressed);
			data.push_back(XPLICIT_PROTO_VERSION);

			data.push_back('\r');
			data.push_back('\n');

			Thread worker([&]() -> void
				{
					for (size_t file_index = 0; file_index < m_files.size(); ++file_index)
					{
						for (size_t data_index = 0; data_index < m_files[file_index]->size(); ++data_index)
						{
							data_tmp.push_back(m_files[file_index]->get()[data_index]);
						}

						data.push_back('\r');
						data.push_back('\n');
						data.push_back('\r');
						data.push_back('\n');
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

			socket.send<int*>(data_tmp.data(), data.size());

			/* Set status as non-ready. */
			m_bReady = false;
		}
	}

	bool DownloadTask::is_ready() noexcept { return m_bReady; }

	DownloadTask::operator bool() noexcept { return this->is_ready(); }

	void DownloadTask::set(const bool ready) noexcept { m_bReady = true; }
}