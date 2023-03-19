/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: DownloadProtocol.cpp
 *			Purpose: Xplicit download protocol
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "DownloadProtocol.h"
#include "Crc32.h"

namespace Xplicit::XDP
{
	XDPDownloadFile::XDPDownloadFile()
	{

	}

	XDPDownloadFile::~XDPDownloadFile()
	{

	}

	XDPDownloadFile::XDPDownloadFile(char* bytes, size_t len)
	{
		this->set(bytes, len);
	}

	int XDPDownloadFile::set(char* bytes, size_t len)
	{
		if (!bytes)
			return -1;

		if (len < 1)
			return -2;

		m_bytes.clear();
		m_bytes.reserve(len);

		for (size_t i = 0; i < m_bytes.size(); ++i)
		{
			m_bytes[i] = bytes[i];
		}

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("[XDPDownloadFile::set] Call returned 0.");
#endif

		return 0;
	}

	char* XDPDownloadFile::get() noexcept
	{
		XPLICIT_ASSERT(!m_bytes.empty());
		return m_bytes.data(); 
	}

	size_t XDPDownloadFile::size() noexcept { return m_bytes.size(); }

	XDPDownloadTask::XDPDownloadTask() 
	{
	
	}

	XDPDownloadTask::~XDPDownloadTask()
	{

	}

	void XDPDownloadTask::add(XDPDownloadFile* file)
	{
		if (file)
			m_files.push_back(file);
	}

	bool XDPDownloadTask::remove(XDPDownloadFile* file)
	{
		if (file)
		{
			auto it = std::find_if(m_files.cbegin(), m_files.cend(), 
			[&](XDPDownloadFile* ptr) -> bool
			{
				return file == ptr;
			});

			if (it != m_files.cend())
				return true;
		}

		return false;
	}

	void XDPDownloadTask::operator()(Socket& socket, const bool compressed)
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
					for (size_t i = 0; i < m_files.size(); ++i)
					{
						for (size_t y = 0; y < m_files[y]->size(); ++y)
						{
							data_tmp.push_back(m_files[i]->get()[y]);
						}

						// add these 5 characters, to seperate each archives.

						data.push_back('!');
						data.push_back('<');
						data.push_back('A');
						data.push_back('R');
						data.push_back('>');
					}
			});

			int crc = xplicit_crc32(reinterpret_cast<char*>(data_tmp.data()), data_tmp.size());
			data.push_back(crc);

			for (size_t i = 0; i < data_tmp.size(); ++i)
			{
				data.push_back(data_tmp[i]);
			}

			// wait for the worker thread to finish.
			worker.join();

			// finally send.
			::send(socket, reinterpret_cast<char*>(data_tmp.data()), data.size(), 0);
		}
	}

	XDPDownloadTask::operator bool() noexcept { return m_ready; }
	void XDPDownloadTask::set(const bool ready) noexcept { m_ready = true; }
}