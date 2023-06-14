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

#include "DSCProtocol.h"
#include "Crc32.h"

namespace Xplicit::Network
{
	FileStream::FileStream(const char* bytes, size_t len)
		: mByteList()
	{
		this->set(bytes, len);
	}

	FileStream::~FileStream() {}

	int FileStream::set(const char* bytes, size_t len)
	{
		if (!bytes ||
			len < 1)
			return 1;

		mByteList.clear();
		mByteList.reserve(len);

		for (size_t i = 0; i < mByteList.size(); ++i)
		{
			mByteList[i] = bytes[i];
		}

		return 0;
	}

	const char* FileStream::get() noexcept
	{
		if (mByteList.empty())
			return "(null)";

		return mByteList.data();
	}

	size_t FileStream::size() noexcept { return mByteList.size(); }

	/* Task stream class */

	FileTaskStream::FileTaskStream() 
		: mReady(false), mFileList() 
	{}

	FileTaskStream::~FileTaskStream() = default;

	void FileTaskStream::add(FileStream* file)
	{
		if (file)
			mFileList.push_back(file);
	}

	bool FileTaskStream::remove(FileStream* file)
	{
		if (file)
		{
			auto it = std::find(mFileList.cbegin(), mFileList.cend(), file);

			if (it != mFileList.cend())
			{
				mFileList.erase(it);
				return true;
			}
		}

		return false;
	}

	void FileTaskStream::operator()(Socket& socket, const bool isCompressed)
	{
		if (!mReady) 
			return;

		if (socket != XPLICIT_SOCKET_ERROR)
		{
			std::vector<std::int32_t> data, data_tmp;

			data.push_back(XPLICIT_STREAM_MAG_0);
			data.push_back(XPLICIT_STREAM_MAG_1);
			data.push_back(XPLICIT_STREAM_MAG_2);
			data.push_back(isCompressed);
			data.push_back(XPLICIT_STREAM_VERSION);

			data.push_back('\r');
			data.push_back('\n');

			Thread worker([&]() -> void
				{
					for (size_t file_index = 0; file_index < mFileList.size(); ++file_index)
					{
						for (size_t data_index = 0; data_index < mFileList[file_index]->size(); ++data_index)
						{
							data_tmp.push_back(mFileList[file_index]->get()[data_index]);
						}

						data.push_back('\r');
						data.push_back('\n');
						data.push_back('\r');
						data.push_back('\n');
					}
			});

			int crc = xplicit_crc32(reinterpret_cast<char*>(data_tmp.data()), data_tmp.size());

			data.push_back(crc);

			for (size_t i = 0; i < data_tmp.size(); ++i)
			{
				data.push_back(data_tmp[i]);
			}

			// wait for the worker thread to finish.
			if (worker.joinable())
				worker.join();

			socket.send<std::int32_t*>(data_tmp.data(), data.size());

			/* Clear status */
			mReady = false;
		}
	}

	bool FileTaskStream::ready() noexcept { return mReady; }

	FileTaskStream::operator bool() noexcept { return this->ready(); }

	void FileTaskStream::set(const bool ready) noexcept { mReady = true; }
}