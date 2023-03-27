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
	FileStream::FileStream(const char* bytes, size_t len)
	{
		this->set(bytes, len);
	}

	FileStream::~FileStream() {}

	int FileStream::set(const char* bytes, size_t len)
	{
		if (!bytes ||
			len < 1)
			return 1;

		m_byteList.clear();
		m_byteList.reserve(len);

		for (size_t i = 0; i < m_byteList.size(); ++i)
		{
			m_byteList[i] = bytes[i];
		}

		return 0;
	}

	const char* FileStream::get() noexcept
	{
		if (m_byteList.empty())
			return "(null)";

		return m_byteList.data();
	}

	size_t FileStream::size() noexcept { return m_byteList.size(); }

	TaskStream::TaskStream() : m_bReady(false), m_fileList() {}

	TaskStream::~TaskStream() {}

	void TaskStream::add(FileStream* file)
	{
		if (file)
			m_fileList.push_back(file);
	}

	bool TaskStream::remove(FileStream* file)
	{
		if (file)
		{
			auto it = std::find(m_fileList.cbegin(), m_fileList.cend(), file);

			if (it != m_fileList.cend())
			{
				m_fileList.erase(it);
				return true;
			}
		}

		return false;
	}

	void TaskStream::operator()(Socket& socket, const bool isCompressed)
	{
		if (!m_bReady) 
			return;

		if (socket != XPLICIT_SOCKET_ERROR)
		{
			std::vector<int> data, data_tmp;

			data.push_back(XPLICIT_STREAM_MAG_0);
			data.push_back(XPLICIT_STREAM_MAG_1);
			data.push_back(XPLICIT_STREAM_MAG_2);
			data.push_back(isCompressed);
			data.push_back(XPLICIT_STREAM_VERSION);

			data.push_back('\r');
			data.push_back('\n');

			Thread worker([&]() -> void
				{
					for (size_t file_index = 0; file_index < m_fileList.size(); ++file_index)
					{
						for (size_t data_index = 0; data_index < m_fileList[file_index]->size(); ++data_index)
						{
							data_tmp.push_back(m_fileList[file_index]->get()[data_index]);
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

			/* Clear status */
			m_bReady = false;
		}
	}

	bool TaskStream::is_ready() noexcept { return m_bReady; }

	TaskStream::operator bool() noexcept { return this->is_ready(); }

	void TaskStream::set(const bool ready) noexcept { m_bReady = true; }
}